#pragma once

#include <cstdint>
#include <functional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include "interface.h"
#include "memaddr.hpp"

struct RunTimeVTableInfo
{
    std::string_view demangled_name;
    uintptr_t  address;
	uint64_t   offset;

	bool operator==(const RunTimeVTableInfo& other) const noexcept = default;
	auto operator<=>(const RunTimeVTableInfo& other) const noexcept = default;
};

enum class SegFlags : uint8_t;

#ifdef PLATFORM_WINDOWS
struct _s_RTTICompleteObjectLocator;
#endif

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
        uintptr_t            address{};
        size_t               size{};
        std::vector<uint8_t> data{};
    };

    struct ReferenceEntry
    {
        uintptr_t target{};
        uintptr_t source_ip{};
    };

    struct FunctionEntry
    {
        uintptr_t start{};
        uintptr_t end{};
    };

    std::vector<Segment> _segments{};
    uintptr_t            _base_address{};
    size_t                _size{};
    std::string          _module_name{};
    CreateInterfaceFn    _createInterFaceFn;

    mutable plg::flat_hash_map<std::string, CAddress, plg::string_hash, std::equal_to<>>              _cached_vtables{};
    mutable plg::flat_hash_map<std::string, std::vector<CAddress>, plg::string_hash, std::equal_to<>> _vtable_functions{};
	mutable std::shared_mutex _mutex;

    void GetModuleInfo(std::string_view module_name);

    std::vector<FunctionEntry>  _function_entries{};
    std::vector<ReferenceEntry> _references{};

    void BuildFunctionIndexAndReferences();

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

    void DumpVtables();
    std::vector<std::unique_ptr<VTable>> _vtables{};

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

    std::flat_hash_map<std::string, uintptr_t, plg::string_hash, std::equal_to<>> _exports{};
    void DumpExports(void* module_base);
#endif

    uintptr_t GetFunctionEntry(uintptr_t middle) const;

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

    [[nodiscard]] CAddress              FindPattern(std::string_view pattern) const;
    [[nodiscard]] std::vector<CAddress> FindPatternMulti(std::string_view pattern) const;

    [[nodiscard]] CAddress              FindString(std::string_view str, bool read_only, bool exact = false) const;
    [[nodiscard]] std::vector<CAddress> FindStringMulti(std::string_view str, bool read_only, bool exact = false) const;

    [[nodiscard]] CAddress              FindData(const uint8_t* needle, size_t needle_size, bool read_only) const;
    [[nodiscard]] std::vector<CAddress> FindDataMulti(const uint8_t* needle, size_t needle_size, bool read_only) const;

    [[nodiscard]] CAddress              FindPtr(uintptr_t ptr) const;
    [[nodiscard]] std::vector<CAddress> FindPtrs(uintptr_t ptr) const;

	[[nodiscard]] CAddress              FindInterface(std::string_view name) const;

	[[nodiscard]] std::vector<CAddress> GetVFunctionsFromVTable(std::string_view vtableName) const;
	void LoopVFunctions(std::string_view vtable_name, const std::function<bool(CAddress)>& callback) const;

    [[nodiscard]] CAddress              GetVirtualTableByName(std::string_view name, bool is_raw_name = false) const;
    [[nodiscard]] CAddress              GetFunctionByName(std::string_view proc_name) const;
    [[nodiscard]] CAddress              GetTypeInfoFromName(std::string_view name) const;

	[[nodiscard]] std::vector<RunTimeVTableInfo>  FindVtablePartial(std::string_view vtable_name) const;
    [[nodiscard]] bool                            IsPointerDerivedFrom(void* ptr, std::string_view vtable_name) const;
    [[nodiscard]] std::vector<CAddress>           IntersectFunctionReferences(std::vector<std::span<const ReferenceEntry>>& reference_sets) const;
    [[nodiscard]] std::span<const ReferenceEntry> GetReferenceRange(uintptr_t address) const;

    [[nodiscard]] Result<CAddress> FindFunctionFromStringRef(std::string_view str) const;
    [[nodiscard]] Result<CAddress> FindFunctionFromStringRefs(std::span<const std::string_view> strs) const;
    [[nodiscard]] Result<CAddress> FindFunctionFromPointerRef(uintptr_t ptr) const;
    [[nodiscard]] Result<CAddress> FindFunctionFromPointerRefs(std::span<const uintptr_t> ptrs) const;
    [[nodiscard]] Result<CAddress> FindFunctionFromAddressRef(uintptr_t addr) const;
	[[nodiscard]] Result<CAddress> FindFunctionFromAddressRefs(std::span<const uintptr_t> addrs) const;
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
    [[nodiscard]] Result<std::vector<CAddress>> FindAllFunctionsFromPointerRefs(std::span<const uintptr_t> ptrs) const;
    [[nodiscard]] Result<std::vector<CAddress>> FindAllFunctionsFromAddressRefs(std::span<const uintptr_t> addrs) const;
	template<typename Range, typename Getter, typename Formatter>
	[[nodiscard]] Result<std::vector<CAddress>> FindAllFunctionsFromRefs(Range&& items, Getter&& getter, Formatter&& formatter) const
	{
		if (std::begin(items) == std::end(items)) [[unlikely]]
		{
			return MakeError("No references provided to search for.");
		}

		std::vector<std::span<const ReferenceEntry>> ref_sets;

		for (const auto& item : items)
		{
			auto addrs = getter(item);

			if (!addrs)
			{
				return MakeError(std::move(addrs.error()));
			}

			for (const auto& addr : *addrs)
			{
				if (!addr.IsValid())
				{
					return MakeError("Reference \"{}\" not found.", formatter(item));
				}

				auto range = GetReferenceRange(*addr);

				if (range.empty())
				{
					return MakeError("Reference \"{}\" (at {}) has no references.", formatter(item), addr.GetPtr());
				}

				ref_sets.push_back(range);
			}
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

constexpr SegFlags operator|(SegFlags lhs, SegFlags rhs)
{
	using underlying = std::underlying_type_t<SegFlags>;
	return static_cast<SegFlags> (static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}

constexpr bool operator&(SegFlags lhs, SegFlags rhs) noexcept
{
	using underlying = std::underlying_type_t<SegFlags>;
	return static_cast<underlying>(lhs) & static_cast<underlying>(rhs);
}

constexpr SegFlags& operator|=(SegFlags& lhs, SegFlags rhs) noexcept
{
	lhs = lhs | rhs;
	return lhs;
}
