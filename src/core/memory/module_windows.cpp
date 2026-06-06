#ifdef PLATFORM_WINDOWS

#    include "os.h"
#    include "module.hpp"

#    include <Zydis/Zydis.h>

#    include <algorithm>
#    include <cinttypes>
#    include <format>
#    include <ranges>
#    include <thread>

#    ifdef __clang__
// clang-cl compat: MSVC pre-defines _ThrowInfo as a compiler built-in type.
// Newer MSVC CRT headers (ehdata_forceinclude.h) define ThrowInfo at line 178
// but reference _ThrowInfo at line 187. clang-cl lacks this built-in, so we
// use a macro to map _ThrowInfo -> ThrowInfo before the header is parsed.
#        define _ThrowInfo ThrowInfo
#    endif
#    include <rttidata.h>
#    ifdef __clang__
#        undef _ThrowInfo
#    endif
#    include <vcruntime.h>

void CModule::GetModuleInfo(std::string_view module_name)
{
    HMODULE handle = GetModuleHandleA(module_name.data());
    if (!handle)
        return;

    m_module_name.resize(MAX_PATH);
    auto actual_size = GetModuleFileNameA(handle, m_module_name.data(), MAX_PATH);
    m_module_name.resize(actual_size);

    m_module_name = m_module_name.substr(m_module_name.find_last_of('\\') + 1);

    m_base_address = handle;

    const auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(handle);

    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        return;

    const auto bytes = reinterpret_cast<uint8_t*>(handle);

    const auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(bytes + dosHeader->e_lfanew);

    if (ntHeader->Signature != IMAGE_NT_SIGNATURE)
        return;

    m_size = ntHeader->OptionalHeader.SizeOfImage;

    auto section = IMAGE_FIRST_SECTION(ntHeader);

    for (auto i = 0u; i < ntHeader->FileHeader.NumberOfSections; i++, section++)
    {
        const auto isExecutable = (section->Characteristics & IMAGE_SCN_MEM_EXECUTE) != 0;
        const auto isReadable   = (section->Characteristics & IMAGE_SCN_MEM_READ) != 0;
        const auto isWritable   = (section->Characteristics & IMAGE_SCN_MEM_WRITE) != 0;

        const auto start = m_base_address + section->VirtualAddress;
        const auto size  = section->Misc.VirtualSize;

        auto& segment   = m_segments.emplace_back();
        segment.address = start;
        segment.size    = size;
        if (isExecutable)
            segment.flags |= SegFlags::X;
        if (isReadable)
            segment.flags |= SegFlags::R;
        if (isWritable)
            segment.flags |= SegFlags::W;

        const auto data = static_cast<uint8_t*>(start);
        segment.data    = std::vector(data, data + size);
		//segment.name    = reinterpret_cast<const char*>(section->Name);
    }

    m_createInterface = GetFunctionByName("CreateInterface").As<CreateInterfaceFn>();

    {
        [[maybe_unused]] plg::Scope scope(m_module_name + "::DumpVTables");
        DumpVtables();
    }
    {
        [[maybe_unused]] plg::Scope scope(m_module_name + "::BuildFunctionIndexAndReferences");
        BuildFunctionIndexAndReferences();
    }
}

void CModule::BuildFunctionIndexAndReferences()
{
    // from praydog https://github.com/cursey/kananlib/blob/7a99a94cea3dbcbd46b54885bd3d04f1d242e21a/src/Scan.cpp#L1329-L1344
    const auto dos_header = m_base_address.As<PIMAGE_DOS_HEADER>();
    const auto nt_header  = (m_base_address + dos_header->e_lfanew).As<PIMAGE_NT_HEADERS>();

    const auto directory = &nt_header->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];

    const auto rva  = directory->VirtualAddress;
    const auto size = directory->Size;

    const auto directory_ptr = (m_base_address + rva).As<PIMAGE_RUNTIME_FUNCTION_ENTRY>();

    const auto entries = size / sizeof(IMAGE_RUNTIME_FUNCTION_ENTRY);

    if (entries <= 0)
    {
        plg::print(LS_ERROR, "No exception directory entries was found for {}\n", m_module_name);
        return;
    }

    m_function_entries.reserve(entries);

    // https://learn.microsoft.com/en-us/cpp/build/exception-handling-x64?view=msvc-170
    struct UNWIND_INFO
    {
        uint8_t Version : 3;
        uint8_t Flags : 5;
        uint8_t SizeOfProlog;
        uint8_t CountOfCodes;
        uint8_t FrameRegister : 4;
        uint8_t FrameOffset : 4;
    };

    for (size_t i = 0; i < entries;)
    {
        auto start_exception = &directory_ptr[i];
        auto start_address   = start_exception->BeginAddress;
        auto end_address     = start_exception->EndAddress;

        size_t next_i = i + 1;
        while (next_i < entries && directory_ptr[next_i].BeginAddress == end_address)
        {
            // checking UNW_FLAG_CHAININFO flag in next entry's unwind data
            // if that flag is set, meaning the next entry belongs to current entry, we should merge it
            // otherwise we treat it as a new function
            auto next_unwind_rva = directory_ptr[next_i].UnwindData;
            auto next_unwind_ptr = (m_base_address + next_unwind_rva).As<UNWIND_INFO*>();

            if ((next_unwind_ptr->Flags & UNW_FLAG_CHAININFO) == 0) break;

            // flag is set, merge
            end_address = directory_ptr[next_i].EndAddress;
            next_i++;
        }

        m_function_entries.emplace_back(
        	m_base_address + start_address,
        	m_base_address + end_address
        );

        i = next_i;
    }

    std::ranges::sort(m_function_entries,
                      [](const FunctionEntry& a, const FunctionEntry& b) {
                          return a.start < b.start;
                      });

    auto is_in_data_segment = [this](CAddress address) noexcept {
        for (const auto& segment : m_segments)
        {
            if (segment.flags & SegFlags::X) continue;
            if (segment.address <= address && address < segment.address + segment.size) return true;
        }
        return false;
    };

    auto is_in_text_segment = [this](CAddress address) noexcept {
        for (const auto& segment : m_segments)
        {
            if ((segment.flags & SegFlags::X) == 0) continue;
            if (segment.address <= address && address < segment.address + segment.size) return true;
        }
        return false;
    };

    const auto num_threads = std::max(1u, std::thread::hardware_concurrency());
    const auto chunk_size  = (m_function_entries.size() + num_threads - 1) / num_threads;

    std::vector<std::vector<ReferenceEntry>> chunk_results(num_threads);
    std::vector<std::thread>                 threads;
    threads.reserve(num_threads);

    // multithreaded solution inspired by the code snippet @angelfor3v3r gave me a long time ago.
    // to be honest i could have used yaxpeax-x86, which is the fastest decoder i have found yet (it takes about 100ms to decode the entire .text segment
    // in libserver.so while zydis takes ~450ms), but i dont think it is worth the effort to replace zydis with it,
    // not to mention safetyhook also uses zydis and i use the encoder feature from zydis too.
    // hopefully no one copies or recodes this function in another language and claims they coded it without giving credit 😭🙏

    auto disassemble_chunk = [&](std::uint32_t idx, size_t start_idx, size_t end_idx) {
        ZydisDecoder decoder{};
        if (ZYAN_FAILED(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_STACK_WIDTH_64))) return;

        ZydisDecodedInstruction instr{};
        ZydisDecodedOperand     operands[ZYDIS_MAX_OPERAND_COUNT]{};

        auto& local_refs = chunk_results[idx];
        local_refs.reserve((end_idx - start_idx) * 10);

        for (size_t i = start_idx; i < end_idx; ++i)
        {
            const auto& entry = m_function_entries[i];
            auto        start = entry.start;
            auto        end   = entry.end;

            uintptr_t jumptable_start_address{};

            for (auto ip = start; ip < end;)
            {
                if (jumptable_start_address != 0 && ip >= jumptable_start_address) break;

                if (!ZYAN_SUCCESS(ZydisDecoderDecodeFull(&decoder, ip, ZYDIS_MAX_INSTRUCTION_LENGTH, &instr, operands)))
                {
                    ip += instr.length;
                    continue;
                }

                if (instr.attributes & ZYDIS_ATTRIB_IS_RELATIVE)
                {
                    if (instr.raw.disp.offset != 0)
                    {
                        const auto target = ip + instr.length + instr.raw.disp.value;

                        if (is_in_data_segment(target)) local_refs.emplace_back(target, ip);
                    }
                }
                else if (jumptable_start_address == 0 && (instr.mnemonic == ZYDIS_MNEMONIC_MOV || instr.mnemonic == ZYDIS_MNEMONIC_MOVSXD))
                {
                    const auto& src = operands[1];
                    if (src.type == ZYDIS_OPERAND_TYPE_MEMORY && src.mem.index != ZYDIS_REGISTER_NONE && (src.mem.segment == ZYDIS_REGISTER_DS || src.mem.segment == ZYDIS_REGISTER_SS) && src.mem.scale != 0 && src.mem.disp.value > 0)
                    {
                        auto target = src.mem.disp.value + m_base_address;

                        if (is_in_text_segment(target)) jumptable_start_address = target;
                    }
                }

                ip += instr.length;
            }
        }
    };

    for (auto i = 0u; i < num_threads; ++i)
    {
        const auto start_idx = i * chunk_size;
        const auto end_idx   = std::min(start_idx + chunk_size, m_function_entries.size());

        if (start_idx >= m_function_entries.size()) break;

        threads.emplace_back(disassemble_chunk, i, start_idx, end_idx);
    }

    // wait for completion
    for (auto& t : threads) t.join();

    // merge results from each thread
    size_t total_refs = 0;
    for (const auto& r : chunk_results) total_refs += r.size();

    m_references.reserve(total_refs);

    for (auto& r : chunk_results) m_references.insert(m_references.end(), std::make_move_iterator(r.begin()), std::make_move_iterator(r.end()));

    std::ranges::sort(m_references, std::less{}, &ReferenceEntry::target);
}

void CModule::DumpVtables()
{
    // originally inspired by praydog & cursey's kananlib https://github.com/cursey/kananlib/blob/main/src/RTTI.cpp
    // but made some improvements based on our usage.
    // hopefully no one copies or recodes this function in another language and claims they coded it without giving credit 😭🙏

    constexpr auto type_info_type_descriptor_name = ".?AVtype_info@@";

    auto type_descriptor_address = FindString(type_info_type_descriptor_name, false);
    if (!type_descriptor_address.IsValid())
    {
        plg::print(LS_ERROR, "Failed to find type descriptor address for \"{}\" in module {}\n", type_info_type_descriptor_name, m_module_name);
        return;
    }

    auto type_info = type_descriptor_address.Deref(-0x10);

    const auto type_info_xrefs = FindPtrs(type_info);
    m_vtables.reserve(type_info_xrefs.size());

    std::vector<uint32_t> valid_type_rvas;
    valid_type_rvas.reserve(type_info_xrefs.size());

    for (const auto& xref : type_info_xrefs)
    	valid_type_rvas.push_back(static_cast<uint32_t>(xref - m_base_address));

    // sort for binary search
    std::ranges::sort(valid_type_rvas);

    plg::flat_hash_map<std::type_info*, VTable*> vtable_map;

    for (const auto& segment : m_segments)
    {
        if (segment.flags & (SegFlags::X | SegFlags::W))
			continue;

        auto start_addr = segment.address;
        auto end_addr   = start_addr + segment.size;

        auto is_in_current_section = [&](CAddress ptr) {
            return start_addr <= ptr && ptr < end_addr;
        };

        for (auto ptr = start_addr; ptr < end_addr - sizeof(void*); ptr += sizeof(void*))
        {
            CAddress potential_col_ptr = ptr.Get<uintptr_t>();

            // check for alignment, struct _s_RTTICompleteObjectLocator aligns to 4 bytes
            if ((potential_col_ptr & 3) != 0) continue;

            if (!is_in_current_section(potential_col_ptr)) continue;

            auto col = potential_col_ptr.As<_s_RTTICompleteObjectLocator*>();

            // 0 --> RTTI Class Hierarchy Descriptor
            // 1 --> RTTI Complete Object Locator
            if (col->signature != 1) continue;

            if (std::ranges::binary_search(valid_type_rvas, col->pTypeDescriptor))
            {
                auto vtable_start = ptr + sizeof(void*);
                auto ti = (m_base_address + col->pTypeDescriptor).As<std::type_info*>();

                auto node = std::make_unique<VTable>(ti, vtable_start, ti->name(), col->offset, col);

                if (col->offset == 0) vtable_map[ti] = node.get();

                m_vtables.push_back(std::move(node));
            }
        }
    }

    for (const auto& vtable : m_vtables)
    {
        auto locator = vtable->object_locator;

        auto hierarchy_descriptor = (m_base_address + locator->pClassDescriptor).As<_s_RTTIClassHierarchyDescriptor*>();
        auto base_class_array     = (m_base_address + hierarchy_descriptor->pBaseClassArray).As<int32_t*>();

        // starts at 1 to skip the class itself
        for (uint32_t i = 1; i < hierarchy_descriptor->numBaseClasses; i++)
        {
            auto base_class_descriptor = (m_base_address + base_class_array[i]).As<_s_RTTIBaseClassDescriptor*>();
            auto base_class_ti         = (m_base_address + base_class_descriptor->pTypeDescriptor).As<std::type_info*>();

            auto it = vtable_map.find(base_class_ti);
            if (it != vtable_map.end()) vtable->children.push_back(it->second);
        }
    }
}

CAddress CModule::GetFunctionByName(std::string_view proc_name) const
{
    return GetProcAddress(m_base_address.As<HMODULE>(), proc_name.data());
}

#endif