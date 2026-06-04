#ifdef PLATFORM_POSIX

#    include "os.h"
#    include "module.hpp"

#    include <Zydis/Zydis.h>

#    include <algorithm>
#    include <chrono>
#    include <cstring>
#    include <memory>
#    include <ranges>

void CModule::GetModuleInfo(std::string_view module_name)
{
	auto fn = [&](struct dl_phdr_info& info) -> int {
		std::string_view name = info.dlpi_name;

		if (!name.contains(module_name))
			return 0;

		m_base_address = info.dlpi_addr;
		m_module_name  = name.substr(name.find_last_of('/') + 1);

		uintptr_t min_vaddr = std::numeric_limits<uintptr_t>::max();
		uintptr_t max_vaddr = 0;

		for (auto i = 0; i < info.dlpi_phnum; i++)
		{
			const auto address = m_base_address + info.dlpi_phdr[i].p_paddr;
			const auto size               = static_cast<uintptr_t>(info.dlpi_phdr[i].p_memsz);
			const auto type      = info.dlpi_phdr[i].p_type;
			const auto is_dynamic_section = type == PT_DYNAMIC;

			const auto flags = info.dlpi_phdr[i].p_flags;

			const auto is_executable = (flags & PF_X) != 0;
			const auto is_readable   = (flags & PF_R) != 0;
			const auto is_writable   = (flags & PF_W) != 0;

			if (is_dynamic_section)
			{
				DumpExports(reinterpret_cast<void*>(address));
				continue;
			}

			if (type != PT_LOAD)
				continue;

			/*if (info.dlpi_phdr[i].p_paddr == 0)
				continue;*/

			auto* data = reinterpret_cast<std::uint8_t*>(address);

			auto& segment = m_segments.emplace_back();

			segment.address = address;
			segment.data    = std::vector(data, data + size);
			segment.size    = size;

			min_vaddr = std::min(min_vaddr, address);
			max_vaddr = std::max(max_vaddr, address + size);

			if (is_executable)
				segment.flags |= SegFlags::X;
			if (is_readable)
				segment.flags |= SegFlags::R;
			if (is_writable)
				segment.flags |= SegFlags::W;
		}

		m_size = max_vaddr - min_vaddr;

		m_createInterface = GetFunctionByName("CreateInterface").As<CreateInterfaceFn>();

		{
			[[maybe_unused]] plg::Scope scope(m_module_name + "::DumpVTables");
			DumpVtables();
		}
		{
			[[maybe_unused]] plg::Scope scope(m_module_name + "::BuildFunctionIndexAndReferences");
			BuildFunctionIndexAndReferences();
		}
		return 1;
	};

	dl_iterate_phdr(
	[](struct dl_phdr_info* info, size_t, void* data) {
		std::string_view name = info->dlpi_name;

		if (!name.contains(".so"))
			return 0;

		if (name.contains("/metamod/"))
			return 0;

		constexpr std::string_view ROOTBIN = S2SDK_ROOT_BINARY;
		constexpr std::string_view GAMEBIN = S2SDK_GAME_BINARY;

		bool isFromRootBin = name.contains(ROOTBIN);
		bool isFromGameBin = name.contains(GAMEBIN);
		if (!isFromGameBin && !isFromRootBin)
			return 0;

		return (*static_cast<decltype(fn)*>(data))(*info);
	}, &fn);
}

void CModule::DumpExports(void* module_base)
{
    auto dyn = static_cast<ElfW(Dyn)*>(module_base);
    // thanks to https://stackoverflow.com/a/57099317
    auto GetNumberOfSymbolsFromGnuHash = [](ElfW(Addr) gnuHashAddress) {
        // See https://flapenguin.me/2017/05/10/elf-lookup-dt-gnu-hash/ and
        // https://sourceware.org/ml/binutils/2006-10/msg00377.html
        struct Header
        {
            uint32_t nbuckets;
            uint32_t symoffset;
            uint32_t bloom_size;
            uint32_t bloom_shift;
        };

        auto       header         = reinterpret_cast<Header*>(gnuHashAddress);
        const auto bucketsAddress = gnuHashAddress + sizeof(Header) + (sizeof(uintptr_t) * header->bloom_size);

        // Locate the chain that handles the largest index bucket.
        uint32_t lastSymbol    = 0;
        auto     bucketAddress = (uint32_t*)bucketsAddress;
        for (uint32_t i = 0; i < header->nbuckets; ++i)
        {
            uint32_t bucket = *bucketAddress;
            if (lastSymbol < bucket)
            {
                lastSymbol = bucket;
            }
            bucketAddress++;
        }

        if (lastSymbol < header->symoffset)
        {
            return header->symoffset;
        }

        // Walk the bucket's chain to add the chain length to the total.
        const auto chainBaseAddress = bucketsAddress + (sizeof(uint32_t) * header->nbuckets);
        for (;;)
        {
            auto chainEntry = (uint32_t*)(chainBaseAddress + (lastSymbol - header->symoffset) * sizeof(uint32_t));
            lastSymbol++;

            // If the low bit is set, this entry is the end of the chain.
            if (*chainEntry & 1)
            {
                break;
            }
        }

        return lastSymbol;
    };

    ElfW(Sym)*  symbols{};
    ElfW(Word)* hash_ptr{};

    char*       string_table{};
    size_t symbol_count{};

    while (dyn->d_tag != DT_NULL)
    {
        if (dyn->d_tag == DT_HASH)
        {
            hash_ptr     = reinterpret_cast<ElfW(Word)*>(dyn->d_un.d_ptr);
            symbol_count = hash_ptr[1];
        }
        else if (dyn->d_tag == DT_STRTAB)
        {
            string_table = reinterpret_cast<char*>(dyn->d_un.d_ptr);
        }
        else if (!symbol_count && dyn->d_tag == DT_GNU_HASH)
        {
            symbol_count = GetNumberOfSymbolsFromGnuHash(dyn->d_un.d_ptr);
        }
        else if (dyn->d_tag == DT_SYMTAB)
        {
            symbols = reinterpret_cast<ElfW(Sym)*>(dyn->d_un.d_ptr);
        }
        dyn++;
    }

	if (symbols == nullptr)
	{
		return;
	}

    for (auto i = 0; i < symbol_count; i++)
    {
		const auto& symbol = symbols[i];

		if (!symbol.st_name)
        {
            continue;
        }

        if (symbol.st_other != 0)
        {
            continue;
        }

        auto             address = symbol.st_value + m_base_address;
        std::string_view name    = &string_table[symbol.st_name];

        _exports.emplace(name, address);
    }
}

CAddress CModule::GetFunctionByName(std::string_view proc_name) const
{
    if (auto it = _exports.find(proc_name); it != _exports.end())
    {
    	return it->second;
    }
    return {};
}

static std::string demangle(const char* mangled_name)
{
    int    status = -1;
    size_t length = 0;

    std::unique_ptr<char, void (*)(void*)> demangled_ptr(
        abi::__cxa_demangle(mangled_name, nullptr, &length, &status),
        std::free);

    return status == 0 ? std::string(demangled_ptr.get()) : mangled_name;
}

std::vector<CModule::RunTimeTypeInfo> CModule::GetRuntimeTypeInfos() const
{
	std::vector<RunTimeTypeInfo> runtime_typeinfos;

	dl_iterate_phdr(
		[](struct dl_phdr_info* info, size_t, void* data) {
			void* handle;

			if (info->dlpi_name && *info->dlpi_name) {
				handle = dlopen(info->dlpi_name, RTLD_LAZY | RTLD_NOLOAD);
			} else {
				handle = dlopen(nullptr, RTLD_LAZY);
			}

			auto get_vtable = [](void* handle, const char* name) -> CAddress {
				CAddress symbol_address = dlsym(handle, name);
				if (symbol_address.IsValid()) {
					return symbol_address.Offset(0x10);
				}
				return {};
			};

			auto class_typeinfo = get_vtable(handle, "_ZTVN10__cxxabiv117__class_type_infoE");
			auto si_class_typeinfo = get_vtable(handle, "_ZTVN10__cxxabiv120__si_class_type_infoE");
			auto vmi_class_typeinfo = get_vtable(handle, "_ZTVN10__cxxabiv121__vmi_class_type_infoE");

			// Check if all three are valid (complete typeinfo set found)
			if (class_typeinfo.IsValid() && si_class_typeinfo.IsValid() && vmi_class_typeinfo.IsValid()) {
				static_cast<std::vector<RunTimeTypeInfo>*>(data)->emplace_back(class_typeinfo, si_class_typeinfo, vmi_class_typeinfo);
			}

			return 0;
		},
		&runtime_typeinfos
	);

	std::ranges::sort(runtime_typeinfos);
	auto [first, last] = std::ranges::unique(runtime_typeinfos);
	runtime_typeinfos.erase(first, last);

	return runtime_typeinfos;
}

std::vector<CModule::TypeInfo> CModule::GetTypeInfos(std::span<const RunTimeTypeInfo> runtime_typeinfos) const
{
	std::vector<TypeInfo> known_typeinfos;

	auto collect_typeinfos = [&](CAddress root_rtti_vtable) {
		auto instances = FindPtrs(root_rtti_vtable.GetPtr());
		known_typeinfos.reserve(known_typeinfos.size() + instances.size());

		for (const auto& xref : instances) {
			known_typeinfos.emplace_back(xref.As<std::type_info*>());
		}
	};

	for (const auto& [class_typeinfo, si_class_typeinfo, vmi_class_typeinfo] : runtime_typeinfos) {
		collect_typeinfos(vmi_class_typeinfo);
		collect_typeinfos(si_class_typeinfo);
		collect_typeinfos(class_typeinfo);
	}

	std::ranges::sort(known_typeinfos, {}, &TypeInfo::address);
	auto [first, last] = std::ranges::unique(known_typeinfos, {}, &TypeInfo::ti);
	known_typeinfos.erase(first, last);

	return known_typeinfos;
}

void CModule::DumpVtables()
{
	std::vector<RunTimeTypeInfo> runtime_typeinfos = GetRuntimeTypeInfos();

    if (runtime_typeinfos.empty()) [[unlikely]]
    {
        plg::print(LS_ERROR, "Failed to get rtti from any loaded module");
        return;
    }

    std::vector<TypeInfo> known_typeinfos = GetTypeInfos(runtime_typeinfos);

    if (known_typeinfos.empty()) [[unlikely]]
    {
        plg::print(LS_ERROR, "Failed to find any typeinfo instances");
        return;
    }

    const auto min_ti_addr = known_typeinfos.front().address();
    const auto max_ti_addr = known_typeinfos.back().address();

    plg::flat_hash_map<const std::type_info*, VTable*> ti_to_vtable_map;
    ti_to_vtable_map.reserve(known_typeinfos.size() / 2);
    m_vtables.reserve(known_typeinfos.size());

    // bruteforcing vtable
    for (const auto& segment : m_segments)
    {
        if (segment.flags & SegFlags::X)
            continue;

        auto scan_start = segment.address;
        auto scan_end   = scan_start + segment.size;

        for (auto current_addr = scan_start; current_addr < scan_end; current_addr += sizeof(void*))
        {
            auto ptr = *reinterpret_cast<uintptr_t*>(current_addr);

			if (ptr < min_ti_addr || ptr > max_ti_addr)
                continue;

            auto it = std::ranges::lower_bound(known_typeinfos, ptr, {}, &TypeInfo::address);

            if (it == known_typeinfos.end() || it->address() != ptr)
                continue;

            auto offset = *reinterpret_cast<std::intptr_t*>(current_addr - 0x8);
            // offset_to_top: 0 for primary vtable, negative for secondary vtables
            if (offset > 0)
                continue;

            // make it positive to behave the same as windows
            offset = -offset;

			const auto& [type_info] = *it;

            auto start_address = current_addr + 0x8;

            auto vtable = std::make_unique<VTable>(type_info, start_address, demangle(type_info->name()), offset);

            if (offset == 0) [[likely]]
                ti_to_vtable_map[type_info] = vtable.get();

            m_vtables.push_back(std::move(vtable));
        }
    }

    std::vector<const std::type_info*>        worklist;
    plg::flat_hash_set<const std::type_info*> visited;

    for (const auto& vtable_ptr : m_vtables)
    {
        VTable* start_node = vtable_ptr.get();

        worklist.clear();
        worklist.push_back(start_node->type_info);

        visited.clear();
        visited.insert(start_node->type_info);

        while (!worklist.empty())
        {
            const std::type_info* current_ti = worklist.back();
            worklist.pop_back();

            uintptr_t ti_vtable_ptr = *reinterpret_cast<const uintptr_t*>(current_ti);

            auto process_base = [&](const std::type_info* base_ti) {
                if (!visited.contains(base_ti))
                {
                    visited.insert(base_ti);

                    auto it = ti_to_vtable_map.find(base_ti);
                    if (it != ti_to_vtable_map.end())
                    {
                        VTable* parent_vtable = it->second;

                        start_node->children.push_back(parent_vtable);

                        // add to worklist to find its parents
                        worklist.push_back(base_ti);
                    }
                }
            };

            for (const auto& [class_typeinfo, si_class_typeinfo, vmi_class_typeinfo] : runtime_typeinfos)
			{
				if (ti_vtable_ptr == si_class_typeinfo.GetPtr())
				{
					auto* si_type_info = static_cast<const __cxxabiv1::__si_class_type_info*>(current_ti);
					process_base(si_type_info->__base_type);
				}
				else if (ti_vtable_ptr == vmi_class_typeinfo.GetPtr())
				{
					auto* vmi_type_info = static_cast<const __cxxabiv1::__vmi_class_type_info*>(current_ti);
					for (auto i = 0u; i < vmi_type_info->__base_count; ++i)
					{
						const auto& base_info = vmi_type_info->__base_info[i];
						process_base(base_info.__base_type);
					}
				}
			}
        }
    }
}

void CModule::BuildFunctionIndexAndReferences()
{
    uintptr_t exec_start{}, exec_end{}, exec_size{};

    std::vector<const Segment*> data_sections;
    data_sections.reserve(m_segments.size());

    uintptr_t min_data_addr = std::numeric_limits<uintptr_t>::max();
    uintptr_t max_data_addr = 0;

    for (const auto& seg : m_segments)
    {
        if (seg.flags & SegFlags::X)
        {
            if (!exec_start)
            {
                exec_start = seg.address;
                exec_end   = seg.address + seg.size;
                exec_size  = seg.size;
            }
        }
        else
        {
            data_sections.push_back(&seg);
            min_data_addr = std::min(min_data_addr, seg.address);
            max_data_addr = std::max(max_data_addr, seg.address + seg.size);
        }
    }

    auto is_function_pointer = [=](uintptr_t addr) noexcept {
        return exec_start <= addr && addr < exec_end;
    };

    auto is_data_pointer = [&](uintptr_t addr) noexcept {
        if (addr < min_data_addr || addr >= max_data_addr)
            return false;
        return std::ranges::any_of(data_sections, [addr](const Segment* s) noexcept {
            return s->address <= addr && addr < s->address + s->size;
        });
    };

    std::vector<uintptr_t> seen_functions;
    seen_functions.reserve(exec_size / 32);

    // phase1: scan data for function ptrs
    for (const Segment* seg : data_sections)
    {
        const auto seg_end = seg->address + seg->size;
        for (auto current_ptr = seg->address; current_ptr + sizeof(void*) <= seg_end; current_ptr += sizeof(void*))
        {
            const auto potential_addr = *reinterpret_cast<uintptr_t*>(current_ptr);

            if (is_function_pointer(potential_addr))
                seen_functions.push_back(potential_addr);
        }
    }

    // phase2: disassembles the entire executable segment in a single pass to
    // 1. find INT3(0xCC) padding
    // 2. find the potential function entries
    // 3. find pointer references in .text segment

    struct ChunkResult
    {
        std::vector<uintptr_t> functions;
        std::vector<uintptr_t> paddings;
        std::vector<ReferenceEntry> refs;
    };

    const auto num_threads = std::max(1u, std::thread::hardware_concurrency());
    const auto chunk_size  = (exec_size + num_threads - 1) / num_threads;

    std::vector<ChunkResult> chunk_results(num_threads);
    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    // multithreaded solution inspired by the code snippet @angelfor3v3r gave me a long time ago.
    // to be honest i could have used yaxpeax-x86, which is the fastest decoder i have found yet (it takes about 100ms to decode libserver.so .text segment
    // while zydis takes ~450ms), but i dont think it is worth the effort to replace zydis with it,
    // not to mention safetyhook also uses zydis and i use the encoder feature from zydis too.
    // hopefully no one copies or recodes this function in another language and claims they coded it without giving credit 😭🙏

    auto disassemble_chunk = [&](std::uint32_t idx, uintptr_t decode_start, uintptr_t chunk_start, uintptr_t chunk_end) {
        ZydisDecoder decoder{};
        if (ZYAN_FAILED(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64)))
            return;

        auto& result = chunk_results[idx];
        result.functions.reserve(chunk_size / 64);
        result.paddings.reserve(chunk_size / 200);
        result.refs.reserve(chunk_size / 8);

        ZydisDecodedInstruction instr{};

        ZydisInstructionCategory prev_category{};
        ZydisMnemonic            prev_mnemonic{};
        bool                     has_prev = false;

        for (auto ip = decode_start; ip < chunk_end;)
        {
            if (ZYAN_FAILED(ZydisDecoderDecodeInstruction(&decoder, nullptr,
                                                          reinterpret_cast<const void*>(ip), exec_end - ip, &instr)))
            {
                ip++;
                has_prev = false;
                continue;
            }

            const auto length = instr.length;

            // only record results after warm-up phase
            if (ip >= chunk_start)
            {
                if (instr.mnemonic == ZYDIS_MNEMONIC_INT3)
                {
                    result.paddings.push_back(ip);
                    ip += length;

                    while (ip < chunk_end && *reinterpret_cast<const std::uint8_t*>(ip) == 0xCC)
                        ++ip;

                    has_prev = false;
                    continue;
                }

                if (instr.attributes & ZYDIS_ATTRIB_IS_RELATIVE)
                {
                    if (instr.meta.category == ZYDIS_CATEGORY_CALL)
                    {
                        const auto target = ip + length + instr.raw.imm[0].value.s;
                        if (is_function_pointer(target))
                            result.functions.push_back(target);
                    }
                    else if (instr.meta.category == ZYDIS_CATEGORY_UNCOND_BR && instr.opcode == 0xE9 && prev_category != ZYDIS_CATEGORY_CALL)
                    {
                        const auto target = ip + length + instr.raw.imm[0].value.s;

                        // a function entry always aligns to 16 bytes, by doing so we can filter out most of the
                        // jmp label instructions and nullsub_xxx functions
                        if ((target & 15) == 0 && is_function_pointer(target))
                        {
                            // detect tail calls by checking stack cleanup before jmp instruction.
                            // there are other ways to detect tail call function but it is
                            // very likely to have false positive so we dont add it here
                            if (has_prev && (prev_category == ZYDIS_CATEGORY_POP || prev_mnemonic == ZYDIS_MNEMONIC_LEAVE))
                            {
                                result.functions.push_back(target);
                            }
                        }
                    }
                    else if (instr.raw.disp.offset != 0)
                    {
                        const auto target = ip + length + instr.raw.disp.value;

                        if (is_data_pointer(target))
                            result.refs.emplace_back(target, ip);
                        else if (is_function_pointer(target))
                            result.functions.push_back(target);
                    }
                }
            }

            prev_category = instr.meta.category;
            prev_mnemonic = instr.mnemonic;
            has_prev      = true;
            ip += length;
        }

        std::ranges::sort(result.functions);
    };

    // each chunk decodes 24 bytes early to handle boundaries landing in the middle of an instruction
    // so we would not get garbage results which can cause missing references
    // technically we can go with 16 bytes, which handles the maximum instruction lenth(15), but we use
    // 24 here to ensure the decoder has fully synchronized before the chunk for actual decoding start
    constexpr size_t warmup_bytes = 24;

    for (auto i = 0u; i < num_threads; ++i)
    {
        const auto chunk_start = exec_start + i * chunk_size;
        if (chunk_start >= exec_end)
            break;

        const auto chunk_end    = std::min(chunk_start + chunk_size, exec_end);
        const auto decode_start = std::max(exec_start, chunk_start - warmup_bytes);

        threads.emplace_back(disassemble_chunk, i, decode_start, chunk_start, chunk_end);
    }

    // wait for completion
    for (auto& t : threads)
        t.join();

    // merge results from each thread
    size_t total_funcs = seen_functions.size();
    size_t total_pads  = 0;
    size_t total_refs  = 0;

    for (const auto& r : chunk_results)
    {
        total_funcs += r.functions.size();
        total_pads += r.paddings.size();
        total_refs += r.refs.size();
    }

    std::vector<uintptr_t> padding_addrs;
    std::vector<ReferenceEntry> temp_refs;

    padding_addrs.reserve(total_pads);
    temp_refs.reserve(total_refs);
    seen_functions.reserve(total_funcs);

    for (const auto& [functions, paddings, refs] : chunk_results)
    {
        // todo: C++23 .append_range(std::views::as_rvalue(r.abc));
        padding_addrs.insert(padding_addrs.end(), std::move_iterator(paddings.begin()), std::move_iterator(paddings.end()));
        temp_refs.insert(temp_refs.end(), std::move_iterator(refs.begin()), std::move_iterator(refs.end()));
        seen_functions.insert(seen_functions.end(), std::move_iterator(functions.begin()), std::move_iterator(functions.end()));
    }

    if (seen_functions.empty()) [[unlikely]]
        return;

    // sort merged results
    std::ranges::sort(padding_addrs);
    std::ranges::sort(temp_refs, std::less{}, &ReferenceEntry::source_ip);
    std::ranges::sort(seen_functions);

    const auto [first, last] = std::ranges::unique(seen_functions);
    seen_functions.erase(first, last);

    // phase3: build function boundary
    m_function_entries.reserve(seen_functions.size());

    auto       pad_it  = padding_addrs.begin();
    const auto pad_end = padding_addrs.end();

    for (size_t idx = 0; idx < seen_functions.size(); ++idx)
    {
        const auto start           = seen_functions[idx];
        const auto next_func_start = (idx + 1 < seen_functions.size()) ? seen_functions[idx + 1] : exec_end;
        auto       end             = next_func_start;

        while (pad_it != pad_end && *pad_it <= start)
            ++pad_it;

        if (pad_it == pad_end) [[unlikely]]
            break;

        if (*pad_it < next_func_start)
            end = *pad_it;

        if (start < end) [[likely]]
            m_function_entries.emplace_back(start, end);
    }

    if (m_function_entries.empty()) [[unlikely]]
        return;

    // phase4: build reference map
    m_references.reserve(temp_refs.size());

    auto       func_it     = m_function_entries.begin();
    const auto func_end_it = m_function_entries.end();

    for (const auto& ref : temp_refs)
    {
        const auto source_ip = ref.source_ip;

        while (func_it != func_end_it && func_it->end <= source_ip)
            ++func_it;

        if (func_it == func_end_it)
            break;

        if (source_ip >= func_it->start)
            m_references.push_back(ref);
    }

    std::ranges::sort(m_references, std::less{}, &ReferenceEntry::target);
}
#endif