#include <algorithm>
#include <iterator>
#include <memory>
#include <span>
#include <utility>

#include "module.hpp"
#include "scan.hpp"

#ifdef PLATFORM_WINDOWS
#    include <format>
#else
#    include <cxxabi.h>
#endif

CModule::CModule(std::string_view str)
{
    GetModuleInfo(std::format(S2SDK_LIBRARY_PREFIX "{}" S2SDK_LIBRARY_SUFFIX, str));
}

CAddress CModule::FindPattern(std::string_view pattern) const
{
    for (const auto& segment : m_segments)
    {
        if ((segment.flags & SegFlags::X) == 0)
            continue;

        const auto& data = segment.data;

        if (auto result = scan::FindPattern(data.data(), data.size(), pattern))
        {
            return segment.address + result;
        }
    }

    return {};
}

std::vector<CAddress> CModule::FindPatternMulti(std::string_view pattern) const
{
	for (const auto& segment : m_segments)
	{
		if ((segment.flags & SegFlags::X) == 0)
			continue;

		const auto& data = segment.data;

		auto result = scan::FindPatternMulti(data.data(), data.size(), pattern);
		if (result.empty())
			continue;

		std::ranges::transform(result, result.begin(), [&](CAddress address) {
			return address + segment.address;
		});

		return result;
	}

	return {};
}

CAddress CModule::FindString(std::string_view str, bool read_only, bool exact) const
{
    for (const auto& segment : m_segments)
    {
        if ((segment.flags & SegFlags::X) != 0)
            continue;

        if (read_only && (segment.flags & SegFlags::W) != 0)
            continue;

        if (auto result = scan::FindStr(segment.address, segment.size, str, true, exact))
        {
            return segment.address + result;
        }
    }

    return {};
}

std::vector<CAddress> CModule::FindStringMulti(std::string_view str, bool read_only, bool exact) const
{
	std::vector<CAddress> results{};

	for (const auto& segment : m_segments)
	{
		if ((segment.flags & SegFlags::X) != 0)
			continue;

		if (read_only && (segment.flags & SegFlags::W) != 0)
			continue;

		auto ptrs = scan::FindStrMulti(segment.address, segment.size, str, true, exact);
		if (ptrs.empty())
			continue;

		for (const auto& tmp : ptrs)
		{
			results.emplace_back(tmp + segment.address);
		}
	}

	return results;
}

CAddress CModule::FindData(const uint8_t* needle, size_t needle_size, bool read_only) const
{
    for (const auto& segment : m_segments)
    {
        if ((segment.flags & SegFlags::X) != 0)
            continue;

        if (read_only && (segment.flags & SegFlags::W) != 0)
            continue;

        if (auto result = scan::FindData(segment.address, segment.size, needle, needle_size))
        {
            return segment.address + result;
        }
    }

    return {};
}

std::vector<CAddress> CModule::FindDataMulti(const uint8_t* needle, size_t needle_size, bool read_only) const
{
    std::vector<CAddress> results{};

    for (const auto& segment : m_segments)
    {
        if ((segment.flags & SegFlags::X) != 0)
            continue;

        if (read_only && (segment.flags & SegFlags::W) != 0)
            continue;

    	auto ptrs = scan::FindDataMulti(segment.address, segment.size, needle, needle_size);
    	if (ptrs.empty())
    		continue;

    	for (const auto& tmp : ptrs)
    	{
    		results.emplace_back(tmp + segment.address);
    	}
    }

    return results;
}

CAddress CModule::FindPtr(CAddress ptr) const
{
    for (const auto& segment : m_segments)
    {
        const auto flags = segment.flags;

        if ((flags & SegFlags::X) != 0)
            continue;

    	if (auto result = scan::FindPtr(segment.address, segment.size, ptr))
    	{
    		return segment.address + result;
    	}
    }

    return {};
}

std::vector<CAddress> CModule::FindPtrs(CAddress ptr) const
{
    std::vector<CAddress> results{};

    for (const auto& segment : m_segments)
    {
        if ((segment.flags & SegFlags::X) != 0)
            continue;

        auto ptrs = scan::FindPtrs(segment.address, segment.size, ptr);
        if (ptrs.empty())
            continue;

        for (const auto& tmp : ptrs)
        {
            results.emplace_back(tmp + segment.address);
        }
    }

    return results;
}

CAddress CModule::FindInterface(std::string_view name) const
{
	if (!m_createInterface)
		return {};

    return m_createInterface(name.data(), nullptr);
}

std::vector<CAddress> CModule::GetVFunctionsFromVTable(std::string_view vtableName) const
{
	{
		std::shared_lock lock(_mutex);
		if (auto it = m_vtable_functions.find(vtableName); it != m_vtable_functions.end())
		{
			return it->second;
		}
	}

    std::vector<CAddress> funcs{};

    LoopVFunctions(vtableName, [&](CAddress addr) {
        funcs.emplace_back(addr);
        return false;
    });

	std::ranges::sort(funcs);
    
	{
		std::unique_lock lock(_mutex);
		m_vtable_functions.emplace(vtableName, funcs);
	}

    return funcs;
}

void CModule::LoopVFunctions(std::string_view vtable_name, const std::function<bool(CAddress)>& callback) const
{
    auto vtable = GetVirtualTableByName(vtable_name);
    if (!vtable.IsValid())
        return;

    uintptr_t sectionStart = 0;
    uintptr_t sectionEnd   = 0;

    for (const auto& segment : m_segments)
    {
        if ((segment.flags & SegFlags::X) != 0)
        {
            sectionStart = segment.address;
            sectionEnd   = segment.address + segment.size;
            break;
        }
    }

    for (;;)
    {
        auto address = vtable.Get<uintptr_t>();
        if (address < sectionStart || address > sectionEnd)
            return;

        if (callback(address))
            return;

       vtable.OffsetSelf(sizeof(uintptr_t));
    }
}

static constexpr std::string_view class_prefix  = "class ";
static constexpr std::string_view struct_prefix = "struct ";

CAddress CModule::GetVirtualTableByName(std::string_view name, bool is_raw_name) const
{
	{
		std::shared_lock lock(_mutex);
		if (auto it = m_cached_vtables.find(name); it != m_cached_vtables.end())
		{
			return it->second;
		}
	}

#ifdef PLATFORM_WINDOWS
    auto vtable_name = is_raw_name ? name : std::format(".?AV{}@@", name);
#else
    auto vtable_name = is_raw_name ? name : std::format("{}{}", name.length(), name);
#endif

    auto it = std::ranges::find_if(m_vtables, [&](const std::unique_ptr<VTable>& vtable) {
        // final class
        if (vtable->offset != 0)
            return false;

        std::string_view demangled_name = vtable->demangled_name;

#ifdef PLATFORM_WINDOWS
        if (vtable->type_info->raw_name() == vtable_name || demangled_name == vtable_name)
            return true;

        const std::string_view target_name = is_raw_name ? vtable_name : name;

        if (const auto idx = demangled_name.find(class_prefix); idx != std::string_view::npos)
        {
            if (demangled_name.substr(idx + class_prefix.length()) == target_name)
                return true;
        }

        if (const auto idx = demangled_name.find(struct_prefix); idx != std::string_view::npos)
        {
            if (demangled_name.substr(idx + struct_prefix.length()) == target_name)
                return true;
        }

        return false;
#else
        return vtable->type_info->name() == vtable_name || demangled_name == vtable_name;
#endif
    });

    if (it == m_vtables.end()) [[unlikely]]
        plg::print(LS_ERROR, "Failed to find vtable \"{}\"", name);

    CAddress address = it->get()->vtable_address;
	{
		std::unique_lock lock(_mutex);
		m_cached_vtables.emplace(name, address);
	}
    return address;
}

std::vector<RunTimeVTableInfo> CModule::FindVtablePartial(std::string_view vtable_name) const
{
    std::vector<RunTimeVTableInfo> result{};

    for (const auto& vtable : m_vtables)
    {
        std::string_view name = vtable->demangled_name;
        if (name.contains(vtable_name))
            result.emplace_back(vtable->demangled_name, vtable->vtable_address, vtable->offset);
    }

    std::ranges::sort(result, [](const RunTimeVTableInfo& a, const RunTimeVTableInfo& b) {
        if (a.offset == b.offset)
            return a.demangled_name < b.demangled_name;
        return a.offset < b.offset;
    });

    return result;
}

bool CModule::IsPointerDerivedFrom(void* ptr, std::string_view vtable_name) const
{
    if (ptr == nullptr) [[unlikely]]
        return false;

    CAddress vtable_address = *static_cast<uintptr_t*>(ptr);
    if (!vtable_address) [[unlikely]]
        return false;

    auto it = std::ranges::find_if(m_vtables, [vtable_address](const std::unique_ptr<VTable>& a) {
        return a->vtable_address == vtable_address;
    });

    if (it == m_vtables.end())
        return false;

    auto vtable = it->get();

    return std::ranges::any_of(vtable->children, [vtable_name](const VTable* a) {
        std::string_view name = a->demangled_name;
        return name == vtable_name || name.contains(vtable_name);
    });
}

CAddress CModule::GetTypeInfoFromName(std::string_view name) const
{
    for (const auto& vtable : m_vtables)
    {
        std::string_view demangled_name = vtable->demangled_name;

        if (demangled_name == name)
            return vtable->type_info;

#ifdef PLATFORM_WINDOWS
        if (demangled_name.starts_with(class_prefix))
        {
            if (demangled_name.substr(class_prefix.size()) == name)
                return vtable->type_info;
        }

        if (demangled_name.starts_with(struct_prefix))
        {
            if (demangled_name.substr(struct_prefix.size()) == name)
                return vtable->type_info;
        }
#endif
    }

    return {};
}

CAddress CModule::GetFunctionEntry(CAddress middle) const
{
    auto it = std::ranges::upper_bound(m_function_entries, middle, {}, &FunctionEntry::start);

    if (it == m_function_entries.begin())
    {
        return {};
    }

    auto candidate = std::prev(it);

    if (middle < candidate->end)
    {
        return candidate->start;
    }

    return {};
}

std::vector<CAddress> CModule::IntersectFunctionReferences(std::vector<std::span<const ReferenceEntry>>& reference_sets) const
{
    if (reference_sets.empty()) [[unlikely]]
        return {};

    std::ranges::sort(reference_sets, [](const auto& a, const auto& b) {
        return a.size() < b.size();
    });

    auto get_unique_funcs = [&](std::span<const ReferenceEntry> refs) {
        std::vector<CAddress> funcs;
        funcs.reserve(refs.size());

        for (const auto& [target, source_ip] : refs)
        {
            if (auto func = GetFunctionEntry(source_ip))
                funcs.emplace_back(func);
        }

        std::ranges::sort(funcs);
        auto [first, last] = std::ranges::unique(funcs);
        funcs.erase(first, last);
        return funcs;
    };

    // process the smallest set
    auto candidates = get_unique_funcs(reference_sets[0]);

    // intersect with remaining sets
    for (size_t i = 1; i < reference_sets.size(); ++i)
    {
        if (candidates.empty())
            break;

        auto next_funcs = get_unique_funcs(reference_sets[i]);
        std::vector<CAddress> intersection;
        intersection.reserve(std::min(candidates.size(), next_funcs.size()));
        std::ranges::set_intersection(candidates, next_funcs, std::back_inserter(intersection));
        std::swap(candidates, intersection);
    }

    return candidates;
}

std::span<const CModule::ReferenceEntry> CModule::GetReferenceRange(CAddress address) const
{
    auto subrange = std::ranges::equal_range(m_references, address, std::less{}, &ReferenceEntry::target);

    if (subrange.empty())
        return {};

    return {subrange.begin(), subrange.end()};
}

Result<CAddress> CModule::FindFunctionFromStringRef(std::string_view str) const
{
    return FindFunctionFromStringRefs(std::span(&str, 1));
}

Result<CAddress> CModule::FindFunctionFromStringRefs(std::span<const std::string_view> strs) const
{
    auto matches = FindAllFunctionsFromStringRefs(strs);

    if (matches->empty())
    {
        return MakeError("No function found matching provided strings:");
    }

    if (matches->size() > 1)
    {
        return MakeError("Ambiguous: {} functions match provided strings.", matches->size());
    }

    return matches->front();
}

Result<CAddress> CModule::FindFunctionFromPointerRef(CAddress ptr) const
{
    return FindFunctionFromPointerRefs(std::span(&ptr, 1));
}

Result<CAddress> CModule::FindFunctionFromPointerRefs(std::span<const CAddress> ptrs) const
{
    auto matches = FindAllFunctionsFromPointerRefs(ptrs);

	if (!matches)
	{
		return MakeError(std::move(matches.error()));
	}
    if (matches->empty())
    {
        return MakeError("No function found matching provided pointers.");
    }
    if (matches->size() > 1)
    {
        return MakeError("Ambiguous: {} functions match provided pointers.", matches->size());
    }

	return matches->front();
}

Result<CAddress> CModule::FindFunctionFromAddressRef(CAddress addr) const
{
    return FindFunctionFromAddressRefs(std::span(&addr, 1));
}

Result<CAddress> CModule::FindFunctionFromAddressRefs(std::span<const CAddress> addrs) const
{
    auto matches = FindAllFunctionsFromAddressRefs(addrs);

	if (!matches)
	{
		return MakeError(std::move(matches.error()));
	}
    if (matches->empty())
    {
        return MakeError("No function found matching provided addresses.");
    }
    if (matches->size() > 1)
    {
        return MakeError("Ambiguous: {} functions match provided addresses.", matches->size());
    }

    return matches->front();
}

Result<std::vector<CAddress>> CModule::FindAllFunctionsFromPointerRefs(std::span<const CAddress> ptrs) const
{
	if (ptrs.empty()) [[unlikely]]
	{
		return MakeError("No pointers provided to search for.");
	}

    std::vector<std::span<const ReferenceEntry>> ref_sets;
    ref_sets.reserve(ptrs.size());

    for (const auto& ptr : ptrs)
    {
        auto range = GetReferenceRange(ptr);
        if (range.empty())
        {
            return MakeError("Pointer \"{}\" has no references.", ptr.GetPtr());
        }

        ref_sets.push_back(range);
    }

    return IntersectFunctionReferences(ref_sets);
}

Result<std::vector<CAddress>> CModule::FindAllFunctionsFromAddressRefs(std::span<const CAddress> addrs) const
{
	if (addrs.empty()) [[unlikely]]
	{
		return MakeError("No addresses provided to search for.");
	}

    std::vector<std::span<const ReferenceEntry>> ref_sets;
    ref_sets.reserve(addrs.size());

    for (const auto& addr : addrs)
    {
    	auto ptr_addr = FindPtr(addr);
    	if (!ptr_addr.IsValid())
    	{
    		return MakeError("Address \"{}\" not found.", addr.GetPtr());
    	}

        auto range = GetReferenceRange(ptr_addr);
        if (range.empty())
        {
            return MakeError("Address \"{}\" (at {}) has no references.", addr.GetPtr(), ptr_addr.GetPtr());
        }

        ref_sets.push_back(range);
    }

    return IntersectFunctionReferences(ref_sets);
}

Result<std::vector<CAddress>> CModule::FindAllFunctionsFromStringRefs(std::span<const std::string_view> strs) const
{
    if (strs.empty()) [[unlikely]]
    {
        return MakeError("No strings provided to search for.");
    }

    std::vector<std::span<const ReferenceEntry>> ref_sets;
    ref_sets.reserve(strs.size());

    for (const auto& s : strs)
    {
        auto str_addr = FindString(s, false, true);
        if (!str_addr.IsValid())
        {
            return MakeError("String \"{}\" not found.", s);
        }

        auto range = GetReferenceRange(str_addr);
        if (range.empty())
        {
            return MakeError("String \"{}\" (at {}) has no references.", s, str_addr.GetPtr());
        }

        ref_sets.push_back(range);
    }

    return IntersectFunctionReferences(ref_sets);
}
