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

#pragma once

#include <cstdint>
#include <functional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include "interface.h"
#include "memaddr.hpp"

struct _s_RTTICompleteObjectLocator;
class CUtlString;

struct RunTimeVTableInfo
{
    CUtlString demangled_name;
    uintptr_t  address;
    uint64_t   offset;
};

class IModule
{
public:
    virtual ~IModule() = default;

    [[nodiscard]] virtual void* FindPatternEx(const char* pattern)                                               = 0;
    [[nodiscard]] virtual void* GetVirtualTableByNameEx(const char* tableName, bool is_raw_name = false)         = 0;
    [[nodiscard]] virtual void* GetFunctionByNameEx(const char* functionName) noexcept                           = 0;
    [[nodiscard]] virtual void* FindInterfaceEx(const char* interfaceName)                                       = 0;
    [[nodiscard]] virtual bool  FindPatternMultiEx(const char* pattern, CUtlLeanVector<std::uintptr_t>* results) = 0;
    [[nodiscard]] virtual void* FindStringEx(const char* str)                                                    = 0;
    [[nodiscard]] virtual void* FindDataEx(const uint8_t* needle, std::size_t needle_size, bool read_only)       = 0;
    [[nodiscard]] virtual void* FindPtrEx(const void* ptr)                                                       = 0;

    virtual void FindVtablePartial(const char* name, CUtlLeanVector<RunTimeVTableInfo>* info) = 0;
    virtual bool IsPointerDerivedFromEx(void* ptr, const char* name)                          = 0;

    [[nodiscard]] virtual bool GetReferencesEx(std::uintptr_t ptr, CUtlLeanVector<std::uintptr_t>* results)                                      = 0;
    [[nodiscard]] virtual bool FindAllFunctionsFromStringsRefsEx(CUtlLeanVector<CUtlString>* strs, CUtlLeanVector<std::uintptr_t>* results)      = 0;
    [[nodiscard]] virtual bool FindAllFunctionsFromPointersRefsEx(CUtlLeanVector<std::uintptr_t>* ptrs, CUtlLeanVector<std::uintptr_t>* results) = 0;
    virtual bool               GetFunctionRangeEx(std::uintptr_t middle, std::uintptr_t* start, std::uintptr_t* end)                             = 0;
    [[nodiscard]] virtual void* FindStringExactEx(const char* str)                                                                               = 0;
};

enum SegmentFlags : uint8_t
{
    FLAG_R = 1 << 0,
    FLAG_W = 1 << 1,
    FLAG_X = 1 << 2,
};

constexpr SegmentFlags operator|(SegmentFlags a, SegmentFlags b)
{
    return static_cast<SegmentFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

class CModule final : public IModule
{
public:
    struct VTable
    {
        std::type_info* type_info;
        uintptr_t       vtable_address;
        std::string     demangled_name;
        uint64_t        offset;

        std::vector<VTable*> children;

#ifdef PLATFORM_WINDOWS
        _s_RTTICompleteObjectLocator* object_locator;

        VTable(std::type_info* ti, uintptr_t vtable, std::string name, uint64_t off, _s_RTTICompleteObjectLocator* locator) :
            type_info(ti), vtable_address(vtable), demangled_name(std::move(name)), offset(off), object_locator(locator) {}
#else
        VTable(std::type_info* ti, uintptr_t vtable, std::string name, uint64_t off) :
            type_info(ti), vtable_address(vtable), demangled_name(std::move(name)), offset(off) {}
#endif
    };

    struct Segment
    {
        Segment()                          = default;
        Segment(Segment&&)                 = default;
        Segment(const Segment&)            = delete;
        Segment& operator=(const Segment&) = delete;
        Segment& operator=(Segment&&)      = delete;

        uint8_t              flags{};
        uintptr_t            address{};
        std::size_t          size{};
        std::vector<uint8_t> data{};
    };

    struct ReferenceEntry
    {
        uintptr_t target;
        uintptr_t source_ip;
    };

    struct FunctionEntry
    {
        uintptr_t start;
        uintptr_t end{};
    };

private:

    std::vector<Segment> _segments{};
    uintptr_t            _base_address{};
    std::size_t          _size{};
    std::string          _module_name{};
    CreateInterfaceFn    _createInterFaceFn;

    std::unordered_map<std::string, uintptr_t, plg::string_hash, std::equal_to<>>              _cached_vtables{};
    std::unordered_map<std::string, std::vector<uintptr_t>, plg::string_hash, std::equal_to<>> _vtable_functions{};

    void GetModuleInfo(std::string_view mod);;

    std::vector<FunctionEntry>  _function_entries{};
    std::vector<ReferenceEntry> _references{};

    void BuildFunctionIndexAndReferences();

    void                                 DumpVtables();
    std::vector<std::unique_ptr<VTable>> _vtables{};

#ifndef PLATFORM_WINDOWS
    std::string                                _module_path{};
    std::unordered_map<std::string, uintptr_t> _exports{};
    void                                       DumpExports(void* module_base);
#endif

    std::uintptr_t GetFunctionEntry(std::uintptr_t middle);

public:
    CModule() = default;
	//~CModule();

    CModule(const CModule&)     = delete;
    CModule(CModule&&) noexcept = delete;

    CModule& operator=(const CModule&) = delete;
    CModule& operator=(CModule&&)      = delete;

    explicit CModule(std::string_view str);
    //CModule(const std::filesystem::path& path, int flags);

    // get base address of a module
    [[nodiscard]] uintptr_t Base() const
    {
        return _base_address;
    }

    [[nodiscard]] std::string_view ModuleName() const
    {
        return _module_name;
    }

	[[nodiscard]] bool IsValid() const
	{
	    return _base_address != 0;
    }

    std::vector<std::uintptr_t> GetVFunctionsFromVTable(std::string_view vtableName);

    void LoopVFunctions(std::string_view vtable_name, const std::function<bool(CAddress)>& callback);

    [[nodiscard]] CAddress              FindPattern(std::string_view pattern) const;
    [[nodiscard]] CAddress              FindPatternStrict(std::string_view pattern) const;
    [[nodiscard]] CAddress              FindString(std::string_view str, bool read_only, bool exact = false) const;
    [[nodiscard]] CAddress              FindData(const uint8_t* needle, std::size_t needle_size, bool read_only) const;
    [[nodiscard]] CAddress              FindPtr(std::uintptr_t ptr) const;
    [[nodiscard]] std::vector<CAddress> FindPtrs(std::uintptr_t ptr) const;
    [[nodiscard]] CAddress              GetVirtualTableByName(std::string_view name, bool is_raw_name = false);
    [[nodiscard]] CAddress              FindInterface(std::string_view name) const;
    [[nodiscard]] std::vector<CAddress> FindPatternMulti(std::string_view pattern) const;
    [[nodiscard]] CAddress              GetFunctionByName(std::string_view proc_name) const;

    [[nodiscard]] CAddress GetTypeInfoFromName(std::string_view name) const;

    [[nodiscard]] bool                            IsPointerDerivedFrom(void* ptr, std::string_view vtable_name);
    std::vector<uintptr_t>                        IntersectFunctionReferences(std::vector<std::span<const ReferenceEntry>>& reference_sets);
    [[nodiscard]] std::span<const ReferenceEntry> GetReferenceRange(uintptr_t address) const;

    [[nodiscard]] CAddress FindFunctionFromStringRef(std::string_view str);
    [[nodiscard]] CAddress FindFunctionFromStringRefs(std::span<const std::string_view> strs);
    [[nodiscard]] CAddress FindFunctionFromPointerRef(std::uintptr_t ptr);
    [[nodiscard]] CAddress FindFunctionFromPointerRefs(std::span<const std::uintptr_t> ptrs);

    [[nodiscard]] std::vector<uintptr_t> FindAllFunctionsFromStringRefs(std::span<const std::string_view> strs);
    [[nodiscard]] std::vector<uintptr_t> FindAllFunctionsFromPointerRefs(std::span<const std::uintptr_t> ptrs);

    // interface
    [[nodiscard]] void* FindPatternEx(const char* pattern) override
    {
        return FindPattern(pattern);
    }

    [[nodiscard]] void* GetVirtualTableByNameEx(const char* tableName, bool bDecorated = false) override
    {
        return GetVirtualTableByName(tableName);
    }

    [[nodiscard]] void* GetFunctionByNameEx(const char* functionName) noexcept override
    {
        return GetFunctionByName(functionName);
    }

    [[nodiscard]] void* FindInterfaceEx(const char* interfaceName) override
    {
        return FindInterface(interfaceName);
    }

    [[nodiscard]] bool FindPatternMultiEx(const char* pattern, CUtlLeanVector<std::uintptr_t>* results) override
    {
        auto pattern_result = FindPatternMulti(pattern);
        if (pattern_result.empty())
            return false;

        auto size = pattern_result.size();
        results->SetSize(static_cast<int32_t>(size));

        for (auto i = 0; i < size; i++)
            results->Element(i) = pattern_result[i];

        return true;
    }

    [[nodiscard]] void* FindStringEx(const char* str) override
    {
        return FindString(str, false);
    }

    [[nodiscard]] void* FindDataEx(const uint8_t* needle, std::size_t needle_size, bool read_only) override
    {
        return FindData(needle, needle_size, read_only);
    }

    [[nodiscard]] void* FindPtrEx(const void* ptr) override
    {
        return FindPtr(reinterpret_cast<uintptr_t>(ptr));
    }

    void FindVtablePartial(const char* name, CUtlLeanVector<RunTimeVTableInfo>* info) override;
    bool IsPointerDerivedFromEx(void* ptr, const char* name) override
    {
        return IsPointerDerivedFrom(ptr, name);
    }

    [[nodiscard]] bool GetReferencesEx(std::uintptr_t ptr, CUtlLeanVector<std::uintptr_t>* results) override
    {
        auto range = GetReferenceRange(ptr);
        if (range.empty())
            return false;

        auto size = range.size();
        results->SetSize(static_cast<int32_t>(size));

        for (auto i = 0; i < size; i++)
            results->Element(i) = range[i].source_ip;

        return true;
    }

    [[nodiscard]] bool FindAllFunctionsFromStringsRefsEx(CUtlLeanVector<CUtlString>* strs, CUtlLeanVector<std::uintptr_t>* results) override
    {
        if (strs->Count() == 0) [[unlikely]]
            return false;

        std::vector<std::string_view> temp_strs{};
        temp_strs.reserve(strs->Count());

        for (int i = 0; i < strs->Count(); ++i)
            temp_strs.emplace_back(strs->Element(i).Get());

        auto temp_results = FindAllFunctionsFromStringRefs(temp_strs);
        if (temp_results.empty())
            return false;

        results->SetCount(static_cast<int32_t>(temp_results.size()));
        for (auto i = 0; i < temp_results.size(); i++)
            results->Element(i) = temp_results[i];

        return true;
    }

    [[nodiscard]] bool FindAllFunctionsFromPointersRefsEx(CUtlLeanVector<std::uintptr_t>* ptrs, CUtlLeanVector<std::uintptr_t>* results) override
    {
        if (ptrs->Count() == 0) [[unlikely]]
            return false;

        std::vector<std::uintptr_t> temp_strs{};
        temp_strs.reserve(ptrs->Count());

        for (int i = 0; i < ptrs->Count(); ++i)
        {
            temp_strs.emplace_back(ptrs->Element(i));
        }

        auto temp_results = FindAllFunctionsFromPointerRefs(temp_strs);
        if (temp_results.empty())
            return false;

        results->SetCount(static_cast<int32_t>(temp_results.size()));
        for (auto i = 0; i < temp_results.size(); i++)
            results->Element(i) = temp_results[i];

        return true;
    }

    [[nodiscard]] const FunctionEntry* GetFunctionRange(std::uintptr_t middle) const
    {
        auto it = std::ranges::upper_bound(_function_entries, middle, {}, &FunctionEntry::start);

        if (it == _function_entries.begin())
            return nullptr;

        auto candidate = std::prev(it);
        return (middle < candidate->end) ? &(*candidate) : nullptr;
    }

    bool GetFunctionRangeEx(std::uintptr_t middle, std::uintptr_t* start, std::uintptr_t* end) override
    {
        if (auto* entry = GetFunctionRange(middle))
        {
            *start = entry->start;
            *end   = entry->end;
            return true;
        }
        *start = 0;
        *end   = 0;
        return false;
    }

    [[nodiscard]] void* FindStringExactEx(const char* str) override
    {
        return FindString(str, false, true);
    }
};
