/*
 * ModSharp
 * Copyright (C) 2023-2026 Kxnrl. All Rights Reserved.
 *
 * This file is part of ModSharp.
 * ModSharp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * ModSharp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with ModSharp. If not, see <https://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <iterator>
#include <memory>
#include <span>
#include <utility>

#include "module.hpp"
#include "scan.hpp"

#ifdef PLATFORM_WINDOWS
#	include <format>
#else
#	include <cxxabi.h>
#endif

CModule::CModule(std::string_view str)
{
	GetModuleInfo(std::format(S2SDK_LIBRARY_PREFIX "{}" S2SDK_LIBRARY_SUFFIX, str));
}

CAddress CModule::FindPattern(std::string_view pattern) const
{
	for (const auto& section : _sections)
	{
        if ((section.flags & FLAG_X) == 0)
            continue;

        const auto& data = section.data;

        if (auto result = scan::FindPattern(data.data(), data.size(), pattern))
        {
            if (result > 0)
                return section.address + result;
        }
	}

	return {};
}

CAddress CModule::FindPatternStrict(std::string_view pattern) const
{
	const auto& result = FindPatternMulti(pattern);
	if (result.empty() || result.size() > 1)
        return {};
	return result[0];
}

CAddress CModule::FindString(std::string_view str, bool read_only, bool exact) const
{
	for (const auto& section : _sections)
	{
        if ((section.flags & FLAG_X) != 0)
            continue;

        if (read_only && (section.flags & FLAG_W) != 0)
            continue;

        if (auto result = scan::FindStr(reinterpret_cast<uint8_t*>(section.address), section.size, str, true, exact))
        {
            if (result > 0)
                return section.address + result;
        }
	}

	return {};
}

CAddress CModule::FindData(const uint8_t* needle, std::size_t needle_size, bool read_only) const
{
	for (const auto& section : _sections)
	{
        if ((section.flags & FLAG_X) != 0)
            continue;

        if (read_only && (section.flags & FLAG_W) != 0)
            continue;

        if (auto result = scan::FindData(reinterpret_cast<uint8_t*>(section.address), section.size, needle, needle_size))
        {
            if (result > 0)
                return section.address + result;
        }
	}

	return {};
}

CAddress CModule::FindPtr(uintptr_t ptr) const
{
	for (const auto& section : _sections)
	{
        const auto flags = section.flags;

        if ((flags & FLAG_X) != 0)
            continue;

        auto res = scan::FindPtr(section.address, section.size, ptr);
        if (res > 0)
            return res + section.address;
	}

	return {};
}

std::vector<CAddress> CModule::FindPtrs(uintptr_t ptr) const
{
	std::vector<CAddress> results{};

	for (const auto& section : _sections)
	{
        if ((section.flags & FLAG_X) != 0)
            continue;

        auto ptrs = scan::FindPtrs(section.address, section.size, ptr);
        if (ptrs.empty())
            continue;

        for (auto temp : ptrs)
        {
            results.emplace_back(temp + section.address);
        }
	}

	return results;
}

CAddress CModule::FindInterface(std::string_view name) const
{
	if (!_createInterFaceFn)
		return {};

	return _createInterFaceFn(name.data(), nullptr);
}

std::vector<CAddress> CModule::FindPatternMulti(std::string_view pattern) const
{
	for (const auto& section : _sections)
	{
        if ((section.flags & FLAG_X) == 0)
            continue;

        const auto& data = section.data;

        auto result = scan::FindPatternMulti(const_cast<uint8_t*>(data.data()), data.size(), pattern);
        if (!result.empty())
        {
            std::ranges::transform(result, result.begin(), [&](CAddress address) {
                return address + section.address;
            });

            return result;
        }
	}

	return {};
}

std::vector<uintptr_t> CModule::GetVFunctionsFromVTable(std::string_view vtableName)
{
	if (auto it = _vtable_functions.find(vtableName); it != _vtable_functions.end())
	{
        return it->second;
	}

	std::vector<uintptr_t> funcs{};

	LoopVFunctions(vtableName, [&](CAddress addr) {
        funcs.emplace_back(addr);
        return false;
	});

	_vtable_functions.emplace(vtableName, funcs);

	return funcs;
}

void CModule::LoopVFunctions(std::string_view vtable_name, const std::function<bool(CAddress)>& callback)
{
	auto vtable = GetVirtualTableByName(vtable_name);
	if (!vtable.IsValid())
        return;

	uintptr_t segmentStart = 0;
	uintptr_t segmentEnd	= 0;

	for (const auto& section : _sections)
	{
        if ((section.flags & FLAG_X) != 0)
        {
            segmentStart = section.address;
            segmentEnd	= section.address + section.size;
            break;
        }
	}

	for (;;)
	{
        auto address = vtable.Get<uintptr_t>();
        if (address < segmentStart || address > segmentEnd)
            return;

        if (callback(address))
            return;

        vtable = vtable.Offset(sizeof(uintptr_t));
	}
}

static constexpr std::string_view class_prefix  = "class ";
static constexpr std::string_view struct_prefix = "struct ";

CAddress CModule::GetVirtualTableByName(std::string_view name, bool is_raw_name)
{
	if (const auto it = _cached_vtables.find(name); it != _cached_vtables.end())
	{
        return it->second;
	}

#ifdef PLATFORM_WINDOWS
	auto vtable_name = is_raw_name ? name : std::format(".?AV{}@@", name);
#else
	auto vtable_name = is_raw_name ? name : std::format("{}{}", name.length(), name);
#endif

	auto it = std::ranges::find_if(_vtables, [&](const std::unique_ptr<VTable>& vtable) {
        // 只需要final class
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

	if (it == _vtables.end()) [[unlikely]]
        plg::print(LS_ERROR, "Failed to find vtable \"{}\"", name);

	auto address = it->get()->vtable_address;
	_cached_vtables.emplace(name, address);
	return address;
}

void CModule::FindVtablePartial(const char* name, CUtlLeanVector<RunTimeVTableInfo>* info)
{
	std::vector<VTable> result{};

	for (const auto& vtable : _vtables)
	{
        std::string_view vtable_name = vtable->demangled_name;

        if (vtable_name.find(name) != std::string_view::npos)
            result.emplace_back(*vtable);
	}

	std::ranges::sort(result, [](const VTable& a, const VTable& b) {
        if (a.offset == b.offset)
            return a.demangled_name < b.demangled_name;
        return a.offset < b.offset;
	});

	for (const auto& value : result)
	{
        auto ptr = info->AddToTailGetPtr();

        ptr->address        = value.vtable_address;
        ptr->demangled_name = value.demangled_name.c_str();
        ptr->offset         = value.offset;
	}
}

bool CModule::IsPointerDerivedFrom(void* ptr, std::string_view vtable_name)
{
	if (ptr == nullptr) [[unlikely]]
        return false;

	auto vtable_address = *static_cast<uintptr_t*>(ptr);
	if (vtable_address == 0) [[unlikely]]
        return false;

	auto it = std::ranges::find_if(_vtables, [vtable_address](const std::unique_ptr<VTable>& a) {
        return a->vtable_address == vtable_address;
	});

	if (it == _vtables.end())
        return false;

	auto vtable = it->get();

	return std::ranges::any_of(vtable->children, [vtable_name](const VTable* a) {
        std::string_view name = a->demangled_name;
        return name == vtable_name || name.find(vtable_name) != std::string_view::npos;
	});
}

CAddress CModule::GetTypeInfoFromName(std::string_view name) const
{
	for (const auto& vtable : _vtables)
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

uintptr_t CModule::GetFunctionEntry(uintptr_t middle)
{
	auto it = std::ranges::upper_bound(_function_entries, middle, {}, &FunctionEntry::start);

	if (it == _function_entries.begin())
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

std::vector<uintptr_t> CModule::IntersectFunctionReferences(std::vector<std::span<const ReferenceEntry>>& reference_sets)
{
	if (reference_sets.empty()) [[unlikely]]
        return {};

	std::ranges::sort(reference_sets, [](const auto& a, const auto& b) {
        return a.size() < b.size();
	});

	auto get_unique_funcs = [&](std::span<const ReferenceEntry> refs) {
        std::vector<uintptr_t> funcs;
        funcs.reserve(refs.size());

        for (const auto& entry : refs)
        {
            if (auto f = GetFunctionEntry(entry.source_ip); f != 0)
                funcs.emplace_back(f);
        }

        std::ranges::sort(funcs);
        auto [first, last] = std::ranges::unique(funcs);
        funcs.erase(first, funcs.end());
        return funcs;
	};

	// process the smallest set
	auto candidates = get_unique_funcs(reference_sets[0]);

	// intersect with remaining sets
	for (size_t i = 1; i < reference_sets.size(); ++i)
	{
        if (candidates.empty())
            break;

        auto                        next_funcs = get_unique_funcs(reference_sets[i]);
        std::vector<uintptr_t> intersection;
        intersection.reserve(std::min(candidates.size(), next_funcs.size()));

        std::ranges::set_intersection(candidates, next_funcs, std::back_inserter(intersection));
        std::swap(candidates, intersection);
	}

	return candidates;
}

std::span<const CModule::ReferenceEntry> CModule::GetReferenceRange(uintptr_t address) const
{
	auto subrange = std::ranges::equal_range(_references, address, std::less{}, &ReferenceEntry::target);

	if (subrange.empty())
        return {};

	return {subrange.begin(), subrange.end()};
}

CAddress CModule::FindFunctionFromStringRef(std::string_view str)
{
	return FindFunctionFromStringRefs(std::span(&str, 1));
}

CAddress CModule::FindFunctionFromStringRefs(std::span<const std::string_view> strs)
{
	auto matches = FindAllFunctionsFromStringRefs(strs);

	if (matches.empty())
	{
        plg::print(LS_WARNING, "No function found matching provided strings:\n{}", plg::join(strs, "\n"));
        return {};
	}

	if (matches.size() > 1)
	{
        plg::print(LS_WARNING, "Ambiguous: {} functions match provided strings:\n{}", matches.size(), plg::join(strs, "\n"));

        for (std::size_t i = 0; i < matches.size(); i++)
        {
            plg::print(LS_WARNING, "#{} {}+0x{:x}\n", i, _module_name, matches[i] - _base_address);
        }
        return {};
	}

	return matches[0];
}

CAddress CModule::FindFunctionFromPointerRef(uintptr_t ptr)
{
	return FindFunctionFromPointerRefs(std::span(&ptr, 1));
}

CAddress CModule::FindFunctionFromPointerRefs(std::span<const uintptr_t> ptrs)
{
	auto matches = FindAllFunctionsFromPointerRefs(ptrs);

	if (matches.empty())
	{
        plg::print(LS_WARNING, "No function found matching provided pointers.\n");
        return {};
	}
	if (matches.size() > 1)
	{
        plg::print(LS_WARNING, "Ambiguous: {} functions match provided pointers.\n", matches.size());
        return {};
	}

	return matches[0];
}

std::vector<uintptr_t> CModule::FindAllFunctionsFromPointerRefs(std::span<const uintptr_t> ptrs)
{
	if (ptrs.empty()) [[unlikely]]
        return {};

	std::vector<std::span<const ReferenceEntry>> ref_sets;
	ref_sets.reserve(ptrs.size());

	for (auto ptr : ptrs)
	{
        auto range = GetReferenceRange(ptr);

        if (range.empty())
        {
            plg::print(LS_WARNING, "Pointer \"{}\" has no references.\n", ptr);
            return {};
        }

        ref_sets.push_back(range);
	}

	return IntersectFunctionReferences(ref_sets);
}

std::vector<uintptr_t> CModule::FindAllFunctionsFromStringRefs(std::span<const std::string_view> strs)
{
	if (strs.empty())
	{
        plg::print(LS_WARNING, "No strings provided to search for.\n");
        return {};
	}

	std::vector<std::span<const ReferenceEntry>> ref_sets;
	ref_sets.reserve(strs.size());

	for (const auto& s : strs)
	{
        auto str_addr = FindString(s, false, true);
        if (!str_addr.IsValid())
        {
            plg::print(LS_WARNING, "String \"{}\" not found.\n", s);
            return {};
        }

        auto range = GetReferenceRange(str_addr);

        if (range.empty())
        {
            plg::print(LS_WARNING, "String \"{}\" (at {}) has no references.\n", s, str_addr.GetPtr());
            return {};
        }

        ref_sets.push_back(range);
	}

	return IntersectFunctionReferences(ref_sets);
}