#pragma once

#include <cstdint>
#include <functional>
#include <span>
#include <string>
#include <vector>

#include "address.hpp"
#include "interface.h"

struct RunTimeVTableInfo
{
    std::string_view demangled_name;
    CAddress address;
	uint64_t offset;

	bool operator==(const RunTimeVTableInfo& other) const noexcept = default;
	auto operator<=>(const RunTimeVTableInfo& other) const noexcept = default;
};

enum class SegFlags : uint8_t;

#ifdef PLATFORM_WINDOWS
struct _s_RTTICompleteObjectLocator;
#endif

// from https://github.com/Kxnrl/modsharp-public/blob/master/Engine/src/module.cpp
class CModule final
{
    struct Segment
    {
        Segment()                          = default;
        Segment(Segment&&)                 = default;
        Segment(const Segment&)            = delete;
        Segment& operator=(const Segment&) = delete;
        Segment& operator=(Segment&&)      = delete;

        SegFlags             flags{};
        CAddress             address{};
        size_t               size{};
        std::vector<uint8_t> data{};
    };

    struct ReferenceEntry
    {
        CAddress target{};
        CAddress source_ip{};
    };

    struct FunctionEntry
    {
        CAddress start{};
        CAddress end{};
    };

    std::vector<Segment> m_segments{};
    CAddress             m_base_address{};
    size_t               m_size{};
    std::string          m_module_name{};
    CreateInterfaceFn    m_createInterface;

    mutable plg::flat_hash_map<std::string, CAddress, plg::string_hash, std::equal_to<>>              m_cached_vtables{};
    mutable plg::flat_hash_map<std::string, std::vector<CAddress>, plg::string_hash, std::equal_to<>> m_vtable_functions{};
	mutable std::shared_mutex _mutex;

    void GetModuleInfo(std::string_view module_name);

    std::vector<FunctionEntry>  m_function_entries{};
    std::vector<ReferenceEntry> m_references{};

    void BuildFunctionIndexAndReferences();

	struct VTable
	{
		std::type_info* type_info;
		CAddress        vtable_address;
		std::string     demangled_name;
		uint64_t        offset;

		std::vector<VTable*> children;

#ifdef PLATFORM_WINDOWS
		_s_RTTICompleteObjectLocator* object_locator;

		VTable(std::type_info* ti, CAddress vtable, std::string_view name, uint64_t off, _s_RTTICompleteObjectLocator* locator) :
			type_info(ti), vtable_address(vtable), demangled_name(name), offset(off), object_locator(locator) {}
#else
		VTable(std::type_info* ti, CAddress vtable, std::string_view name, uint64_t off) :
			type_info(ti), vtable_address(vtable), demangled_name(name), offset(off) {}
#endif
	};

    void DumpVtables();
    std::vector<std::unique_ptr<VTable>> m_vtables{};

#ifndef PLATFORM_WINDOWS
	struct RunTimeTypeInfo
	{
		CAddress class_typeinfo;
		CAddress si_class_typeinfo;
		CAddress vmi_class_typeinfo;

		bool operator==(const RunTimeTypeInfo& other) const noexcept = default;
		auto operator<=>(const RunTimeTypeInfo& other) const noexcept = default;
	};

	struct TypeInfo
	{
		std::type_info* ti;
		[[nodiscard]] uintptr_t address() const { return reinterpret_cast<uintptr_t>(ti); }

		bool operator==(const TypeInfo& other) const noexcept = default;
		auto operator<=>(const TypeInfo& other) const noexcept = default;
	};

	std::vector<RunTimeTypeInfo> GetRuntimeTypeInfos() const;
	std::vector<TypeInfo> GetTypeInfos(std::span<const RunTimeTypeInfo> runtime_typeinfos) const;

    plg::flat_hash_map<std::string, CAddress, plg::string_hash, std::equal_to<>> _exports{};
    void DumpExports(void* module_base);
#endif

    CAddress GetFunctionEntry(CAddress middle) const;

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
    [[nodiscard]] CAddress Base() const
    {
        return m_base_address;
    }

    [[nodiscard]] std::string_view ModuleName() const
    {
        return m_module_name;
    }

	[[nodiscard]] bool IsValid() const
	{
	    return m_base_address;
    }

    [[nodiscard]] CAddress              FindPattern(std::string_view pattern) const;
    [[nodiscard]] std::vector<CAddress> FindPatternMulti(std::string_view pattern) const;

    [[nodiscard]] CAddress              FindString(std::string_view str, bool read_only, bool exact = false) const;
    [[nodiscard]] std::vector<CAddress> FindStringMulti(std::string_view str, bool read_only, bool exact = false) const;

    [[nodiscard]] CAddress              FindData(const uint8_t* needle, size_t needle_size, bool read_only) const;
    [[nodiscard]] std::vector<CAddress> FindDataMulti(const uint8_t* needle, size_t needle_size, bool read_only) const;

    [[nodiscard]] CAddress              FindPtr(CAddress ptr) const;
    [[nodiscard]] std::vector<CAddress> FindPtrs(CAddress ptr) const;

	[[nodiscard]] CAddress              FindInterface(std::string_view name) const;

	[[nodiscard]] std::vector<CAddress> GetVFunctionsFromVTable(std::string_view vtableName) const;
	void LoopVFunctions(std::string_view vtable_name, const std::function<bool(CAddress)>& callback) const;

    [[nodiscard]] CAddress              GetVirtualTableByName(std::string_view name, bool is_raw_name = false) const;
    [[nodiscard]] CAddress              GetFunctionByName(std::string_view proc_name) const;
    [[nodiscard]] CAddress              GetTypeInfoFromName(std::string_view name) const;

	[[nodiscard]] std::vector<RunTimeVTableInfo>  FindVtablePartial(std::string_view vtable_name) const;
    [[nodiscard]] bool                            IsPointerDerivedFrom(void* ptr, std::string_view vtable_name) const;
    [[nodiscard]] std::vector<CAddress>           IntersectFunctionReferences(std::vector<std::span<const ReferenceEntry>>& reference_sets) const;
    [[nodiscard]] std::span<const ReferenceEntry> GetReferenceRange(CAddress address) const;

    [[nodiscard]] Result<CAddress> FindFunctionFromStringRef(std::string_view str) const;
    [[nodiscard]] Result<CAddress> FindFunctionFromStringRefs(std::span<const std::string_view> strs) const;
    [[nodiscard]] Result<CAddress> FindFunctionFromPointerRef(CAddress ptr) const;
    [[nodiscard]] Result<CAddress> FindFunctionFromPointerRefs(std::span<const CAddress> ptrs) const;
    [[nodiscard]] Result<CAddress> FindFunctionFromAddressRef(CAddress addr) const;
	[[nodiscard]] Result<CAddress> FindFunctionFromAddressRefs(std::span<const CAddress> addrs) const;
	template<typename Range, typename Getter, typename Formatter>
	[[nodiscard]] Result<CAddress> FindFunctionFromRefs(Range&& items, Getter&& getter, Formatter&& formatter) const
	{
		auto matches = FindAllFunctionsFromRefs(items, getter, formatter);

		if (!matches)
		{
			return MakeError(std::move(matches.error()));
		}
		if (matches->empty())
		{
			return MakeError("No function found matching provided references.");
		}
		if (matches->size() > 1)
		{
			return MakeError("Ambiguous: {} functions match provided references.", matches->size());
		}

		return matches->front();
	}

    [[nodiscard]] Result<std::vector<CAddress>> FindAllFunctionsFromStringRefs(std::span<const std::string_view> strs) const;
    [[nodiscard]] Result<std::vector<CAddress>> FindAllFunctionsFromPointerRefs(std::span<const CAddress> ptrs) const;
    [[nodiscard]] Result<std::vector<CAddress>> FindAllFunctionsFromAddressRefs(std::span<const CAddress> addrs) const;
	template<typename Range, typename Getter, typename Formatter>
	[[nodiscard]] Result<std::vector<CAddress>> FindAllFunctionsFromRefs(Range&& items, Getter&& getter, Formatter&& formatter) const
	{
		if (std::ranges::begin(items) == std::ranges::end(items)) [[unlikely]]
		{
			return MakeError("No references provided to search for.");
		}

		std::vector<std::span<const ReferenceEntry>> ref_sets;
		ref_sets.reserve(std::ranges::size(items));

		for (const auto& item : items)
		{
			auto addr = getter(item);

			if (!addr)
			{
				return MakeError(std::move(addr.error()));
			}

			if (!addr->IsValid())
			{
				return MakeError("Reference \"{}\" not found.", formatter(item));
			}

			auto range = GetReferenceRange(*addr);

			if (range.empty())
			{
				return MakeError("Reference \"{}\" (at {}) has no references.", formatter(item), *addr);
			}

			ref_sets.push_back(range);
		}

		return IntersectFunctionReferences(ref_sets);
	}
};

enum class SegFlags : uint8_t
{
	UNSET = 0,
	R = 1 << 0,
	W = 1 << 1,
	X = 1 << 2,
};

consteval void enable_bitmask_operators(SegFlags);