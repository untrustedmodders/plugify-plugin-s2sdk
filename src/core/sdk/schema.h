/**
 * =============================================================================
 * s2sdk
 * Copyright (C) 2023-2025 untrustedmodders
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstdint>

#include "virtual.h"
#include <entityinstance.h>
#include <smartptr.h>
#include <tier0/dbg.h>
#include <utldelegate.h>
#undef schema

struct SchemaKey {
    int32 offset;
    int32 networked : 1;
    int32 size : 31;
    CSchemaType* type;
};

namespace schema {
	static plg::flat_hash_set<plg::string> CS2BadList = {
			"m_bIsValveDS",
			"m_bIsQuestEligible",
			"m_iItemDefinitionIndex",// in unmanaged this cannot be set.
			"m_iEntityLevel",
			"m_iItemIDHigh",
			"m_iItemIDLow",
			"m_iAccountID",
			"m_iEntityQuality",

			"m_bInitialized",
			"m_szCustomName",
			"m_iAttributeDefinitionIndex",
			"m_iRawValue32",
			"m_iRawInitialValue32",
			"m_flValue", // MNetworkAlias "m_iRawValue32"
			"m_flInitialValue", // MNetworkAlias "m_iRawInitialValue32"
			"m_bSetBonus",
			"m_nRefundableCurrency",

			"m_OriginalOwnerXuidLow",
			"m_OriginalOwnerXuidHigh",

			"m_nFallbackPaintKit",
			"m_nFallbackSeed",
			"m_flFallbackWear",
			"m_nFallbackStatTrak",

			"m_iCompetitiveWins",
			"m_iCompetitiveRanking",
			"m_iCompetitiveRankType",
			"m_iCompetitiveRankingPredicted_Win",
			"m_iCompetitiveRankingPredicted_Loss",
			"m_iCompetitiveRankingPredicted_Tie",

			"m_nActiveCoinRank",
			"m_nMusicID",
	};

	enum ElementType : int {
		Invalid,
		Single,
		Array,
		Collection,
		Class
	};

    int32_t FindChainOffset(std::string_view className);
    SchemaKey GetOffset(std::string_view className, std::string_view memberName);

    int32_t FindChainOffset(const char* className);
    SchemaKey GetOffset(const char* className, const char* memberName);

    void NetworkStateChanged(intptr_t chainEntity, uint32_t localOffset, int32_t arrayIndex = 0xFFFFFFFF);

	ElementType GetElementType(CSchemaType* type);
	std::pair<ElementType, int> IsIntType(CSchemaType* type);
	std::pair<ElementType, int> IsFloatType(CSchemaType* type);
	ElementType IsPlainType(CSchemaType* type, size_t size);
	ElementType IsAtomicType(CSchemaType* type, size_t size);

}// namespace schema

void EntityNetworkStateChanged(uintptr_t entity, uint32_t offset);
void ChainNetworkStateChanged(uintptr_t networkVarChainer, uint32_t offset);
void NetworkVarStateChanged(uintptr_t networkVar, uint32_t offset, uint32_t networkStateChangedOffset);

void SafeNetworkStateChanged(intptr_t entity, int offset, int chainOffset);

template<
    typename T,
    typename ThisClass,
    char const* MemberName,
    size_t(*MemberOffset)(),
    std::size_t ExtraOffset = 0
>
class SchemaField {
    using ptr_t = std::conditional_t<
        std::is_const_v<std::remove_reference_t<T>>,
        const T*,
        T*
    >;
    using ref_t = std::conditional_t<
        std::is_const_v<std::remove_reference_t<T>>,
        const T&,
        T&
    >;

public:
    [[nodiscard]] auto Key() const {
        static const auto key = schema::GetOffset(ThisClass::m_className, MemberName);
        return key;
    }

    [[nodiscard]] auto Chain() const {
        static const auto chain = schema::FindChainOffset(ThisClass::m_className);
        return chain;
    }

    [[nodiscard]] auto Offset() const { return Key().offset + ExtraOffset; }

    [[nodiscard]] auto ThisPtr() const { return reinterpret_cast<uintptr_t>(this) - MemberOffset(); }

    [[nodiscard]] ref_t Get() const {
        return *reinterpret_cast<ptr_t>(ThisPtr() + Offset());
    }

    void Set(const T& val) {
        NotifyNetworkChange();
        *reinterpret_cast<ptr_t>(ThisPtr() + Offset()) = val;
    }

    [[nodiscard]] operator ref_t() const { return Get(); }

    [[nodiscard]] auto operator->() const requires (std::is_pointer_v<T>) { return Get(); }
    [[nodiscard]] auto operator->() const requires (!std::is_pointer_v<T>) { return std::addressof(Get()); }

    [[nodiscard]] auto operator*() const requires (std::is_pointer_v<T>) { return *Get(); }
    [[nodiscard]] auto operator*() const requires (!std::is_pointer_v<T>) { return Get(); }

    [[nodiscard]] auto operator[](auto idx) const { return Get()[idx]; }
    [[nodiscard]] auto operator+(auto v) const { return Get() + v; }
    [[nodiscard]] auto operator-(auto v) const { return Get() - v; }
    [[nodiscard]] bool operator==(const auto& v) const { return Get() == v; }
    [[nodiscard]] auto operator<=>(const auto& v) const { return Get() <=> v; }

    auto& operator=(const T& val) {
        Set(val);
        return *this;
    }

    [[nodiscard]] bool operator==(std::nullptr_t) const requires (std::is_pointer_v<T>) {
        return Get() == nullptr;
    }

    // Increment / decrement
    auto& operator+=(const auto& v) requires std::is_arithmetic_v<T> {
        Set(Get() + v); return *this;
    }

    auto& operator-=(const auto& v) requires std::is_arithmetic_v<T> {
        Set(Get() - v); return *this;
    }

    auto& operator++() requires std::is_arithmetic_v<T> {
        Set(Get() + 1); return *this;
    }

    auto& operator--() requires std::is_arithmetic_v<T> {
        Set(Get() - 1); return *this;
    }

    [[nodiscard]] auto operator++(int) requires std::is_arithmetic_v<T> {
        auto old = Get(); Set(old + 1); return old;
    }

    [[nodiscard]] auto operator--(int) requires std::is_arithmetic_v<T> {
        auto old = Get(); Set(old - 1); return old;
    }

    // Arithmetic
    auto& operator*=(const auto& v) requires std::is_arithmetic_v<T> { Set(Get() * v); return *this; }
    auto& operator/=(const auto& v) requires std::is_arithmetic_v<T> { Set(Get() / v); return *this; }
    [[nodiscard]] auto operator*(const auto& v) const requires std::is_arithmetic_v<T> { return Get() * v; }
    [[nodiscard]] auto operator/(const auto& v) const requires std::is_arithmetic_v<T> { return Get() / v; }

    // Bitwise
    auto& operator|=(const auto& v) requires std::is_integral_v<T> { Set(Get() | v); return *this; }
    auto& operator&=(const auto& v) requires std::is_integral_v<T> { Set(Get() & v); return *this; }
    auto& operator^=(const auto& v) requires std::is_integral_v<T> { Set(Get() ^ v); return *this; }

    [[nodiscard]] auto operator|(const auto& v) const requires std::is_integral_v<T> { return Get() | v; }
    [[nodiscard]] auto operator&(const auto& v) const requires std::is_integral_v<T> { return Get() & v; }
    [[nodiscard]] auto operator^(const auto& v) const requires std::is_integral_v<T> { return Get() ^ v; }

    void NotifyNetworkChange() const {
        if (!Key().networked) return;

        const auto thisPtr = ThisPtr();
        const auto offset = Offset();

        if (const auto chain = Chain(); chain != 0) {
            ::ChainNetworkStateChanged(thisPtr + chain, offset);
        } else if constexpr (ThisClass::m_networkStateChangedOffset) {
            ::NetworkVarStateChanged(thisPtr, offset, ThisClass::m_networkStateChangedOffset);
        } else {
            ::EntityNetworkStateChanged(thisPtr, offset);
        }
    }
};


template<
    typename T,
    typename ThisClass,
    char const* MemberName,
    size_t(*MemberOffset)(),
    std::size_t ExtraOffset = 0
>
class SchemaPointerField {
    using ptr_t = std::conditional_t<
        std::is_const_v<std::remove_reference_t<T>>,
        const T*,
        T*
    >;

public:
    [[nodiscard]] auto Key() const {
        static const auto key = schema::GetOffset(ThisClass::m_className, MemberName);
        return key;
    }

    [[nodiscard]] auto Chain() const {
        static const auto chain = schema::FindChainOffset(ThisClass::m_className);
        return chain;
    }

    [[nodiscard]] auto Offset() const { return Key().offset + ExtraOffset; }

    [[nodiscard]] auto ThisPtr() const { return reinterpret_cast<uintptr_t>(this) - MemberOffset(); }

    [[nodiscard]] auto Get() const {
        return reinterpret_cast<ptr_t>(ThisPtr() + Offset());
    }

    [[nodiscard]] operator ptr_t() const { return Get(); }
    [[nodiscard]] auto operator->() const { return Get(); }
    [[nodiscard]] auto operator*() const { return *Get(); }
    [[nodiscard]] decltype(auto) operator[](auto idx) const { return Get()[idx]; }

    [[nodiscard]] bool operator==(auto ptr) const { return Get() == ptr; }

    void NetworkStateChanged() const {
        if (!Key().networked) return;

        const auto thisPtr = ThisPtr();
        const auto offset = Offset();

        if (const auto chain = Chain(); chain != 0) {
            ::ChainNetworkStateChanged(thisPtr + chain, offset);
        } else if constexpr (ThisClass::m_networkStateChangedOffset) {
            ::NetworkVarStateChanged(thisPtr, offset, ThisClass::m_networkStateChangedOffset);
        } else {
            ::EntityNetworkStateChanged(thisPtr, offset);
        }
    }
};

#if 0
template<
    typename T,
    typename ThisClass,
    char const* MemberName,
    size_t(*MemberOffset)(),
    std::size_t ExtraOffset = 0
>
class SchemaField {
	using ptr_t = std::conditional_t<
			std::is_const_v<std::remove_reference_t<T>>,
			const T*,
			T*
		>;
	using ref_t = std::conditional_t<
			std::is_const_v<std::remove_reference_t<T>>,
			const T&,
			T&
	    >;

	// Compact internal helpers
	[[nodiscard]] auto Key(this auto&& self) {
		static const auto key = schema::GetOffset(ThisClass::m_className, MemberName);
		return key;
	}
	[[nodiscard]] auto Chain(this auto&& self) {
		static const auto chain = schema::FindChainOffset(ThisClass::m_className);
		return chain;
	}
	[[nodiscard]] auto Offset(this auto&& self) { return self.Key().offset + ExtraOffset; }
	[[nodiscard]] auto ThisPtr(this auto&& self) { return reinterpret_cast<uintptr_t>(&self) - MemberOffset(); }

public:
	[[nodiscard]] ref_t Get(this auto&& self) {
		return *reinterpret_cast<ptr_t>(self.ThisPtr() + self.Offset());
	}

	void Set(this auto&& self, const T& val) {
		self.NotifyNetworkChange();
		*reinterpret_cast<ptr_t>(self.ThisPtr() + self.Offset()) = val;
	}

	[[nodiscard]] operator ref_t(this auto&& self) { return self.Get(); }
	[[nodiscard]] auto operator->(this auto&& self) requires (std::is_pointer_v<T>) { return self.Get(); }
	[[nodiscard]] auto operator->(this auto&& self) requires (!std::is_pointer_v<T>) { return std::addressof(self.Get()); }
    [[nodiscard]] auto operator*(this auto&& self) requires (std::is_pointer_v<T>) { return *self.Get(); }
    [[nodiscard]] auto operator*(this auto&& self) requires (!std::is_pointer_v<T>) { return self.Get(); }
    [[nodiscard]] auto operator[](this auto&& self, auto idx) { return self.Get()[idx]; }
    [[nodiscard]] auto operator+(this auto&& self, auto v) { return self.Get() + v; }
    [[nodiscard]] auto operator-(this auto&& self, auto v) { return self.Get() - v; }
    [[nodiscard]] bool operator==(this auto&& self, const auto& v) { return self.Get() == v; }
    [[nodiscard]] auto operator<=>(this auto&& self, const auto& v) { return self.Get() <=> v; }

    auto& operator=(this auto&& self, const T& val) {
	    self.Set(val); return self;
    }
	[[nodiscard]] bool operator==(this auto&& self, std::nullptr_t) requires (std::is_pointer_v<T>) {
		return self.Get() == nullptr;
	}

	// Increment/decrement
    auto& operator+=(this auto&& self, const auto& v) requires std::is_arithmetic_v<T> {
        self.Set(self.Get() + v); return self;
    }
    auto& operator-=(this auto&& self, const auto& v) requires std::is_arithmetic_v<T> {
        self.Set(self.Get() - v); return self;
    }
	auto& operator++(this auto&& self) requires std::is_arithmetic_v<T> {
		self.Set(self.Get() + 1); return self;
	}
	auto& operator--(this auto&& self) requires std::is_arithmetic_v<T> {
		self.Set(self.Get() - 1); return self;
	}
	[[nodiscard]] auto operator++(this auto&& self, int) requires std::is_arithmetic_v<T> {
		auto old = self.Get(); self.Set(old + 1); return old;
	}
	[[nodiscard]] auto operator--(this auto&& self, int) requires std::is_arithmetic_v<T> {
		auto old = self.Get(); self.Set(old - 1); return old;
	}

	// Arithmetic (for numeric fields)
	auto& operator*=(this auto&& self, const auto& v) requires std::is_arithmetic_v<T> {
		self.Set(self.Get() * v); return self;
	}
	auto& operator/=(this auto&& self, const auto& v) requires std::is_arithmetic_v<T> {
		self.Set(self.Get() / v); return self;
	}
	[[nodiscard]] auto operator*(this auto&& self, const auto& v) requires std::is_arithmetic_v<T> {
		return self.Get() * v;
	}
	[[nodiscard]] auto operator/(this auto&& self, const auto& v) requires std::is_arithmetic_v<T> {
		return self.Get() / v;
	}

	// Bitwise (for integral fields)
	auto& operator|=(this auto&& self, const auto& v) requires std::is_integral_v<T> {
		self.Set(self.Get() | v); return self;
	}
	auto& operator&=(this auto&& self, const auto& v) requires std::is_integral_v<T> {
		self.Set(self.Get() & v); return self;
	}
	auto& operator^=(this auto&& self, const auto& v) requires std::is_integral_v<T> {
		self.Set(self.Get() ^ v); return self;
	}
	[[nodiscard]] auto operator|(this auto&& self, const auto& v) requires std::is_integral_v<T> {
		return self.Get() | v;
	}
	[[nodiscard]] auto operator&(this auto&& self, const auto& v) requires std::is_integral_v<T> {
		return self.Get() & v;
	}
	[[nodiscard]] auto operator^(this auto&& self, const auto& v) requires std::is_integral_v<T> {
		return self.Get() ^ v;
	}

	void NotifyNetworkChange(this auto&& self) {
		if (!self.Key().networked)
			return;

		const auto thisPtr = self.ThisPtr();
		const auto offset = self.Offset();

		if (const auto chain = self.Chain(); chain != 0) {
			::ChainNetworkStateChanged(thisPtr + chain, offset);
		} else if constexpr (ThisClass::m_networkStateChangedOffset) {
			::NetworkVarStateChanged(thisPtr, offset, ThisClass::m_networkStateChangedOffset);
		} else {
			::EntityNetworkStateChanged(thisPtr, offset);
		}
	}
};

template<
    typename T,
    typename ThisClass,
    char const* MemberName,
    size_t(*MemberOffset)(),
    std::size_t ExtraOffset = 0
>
class SchemaPointerField {
	using ptr_t = std::conditional_t<
			std::is_const_v<std::remove_reference_t<T>>,
			const T*,
			T*
		>;
	/*using ref_t = std::conditional_t<
			std::is_const_v<std::remove_reference_t<T>>,
			const T&,
			T&
	    >;*/

    // Compact internal helpers
    [[nodiscard]] auto Key(this auto&& self) {
        static const auto key = schema::GetOffset(ThisClass::m_className, MemberName);
        return key;
    }
    [[nodiscard]] auto Chain(this auto&& self) {
        static const auto chain = schema::FindChainOffset(ThisClass::m_className);
        return chain;
    }
    [[nodiscard]] auto Offset(this auto&& self) { return self.Key().offset + ExtraOffset; }
    [[nodiscard]] auto ThisPtr(this auto&& self) { return reinterpret_cast<uintptr_t>(&self) - MemberOffset(); }

public:
    // Single-line methods
    [[nodiscard]] auto Get(this auto&& self) {
        return reinterpret_cast<ptr_t>(self.ThisPtr() + self.Offset());
    }

	[[nodiscard]] operator ptr_t(this auto&& self) { return self.Get(); }
	//[[nodiscard]] explicit operator bool(this auto&& self) { return self.Get() != nullptr; }
    [[nodiscard]] auto operator->(this auto&& self) { return self.Get(); }
    [[nodiscard]] auto operator*(this auto&& self) { return *self.Get(); }
    [[nodiscard]] decltype(auto) operator[](this auto&& self, auto idx) { return self.Get()[idx]; }
    //[[nodiscard]] auto operator+(this auto&& self, auto offset) { return self.Get() + offset; }
    //[[nodiscard]] auto operator-(this auto&& self, auto offset) { return self.Get() - offset; }
    [[nodiscard]] bool operator==(this auto&& self, auto ptr) { return self.Get() == ptr; }
    //[[nodiscard]] auto operator<=>(this auto&& self, auto ptr) { return self.Get() <=> ptr; }

    void NetworkStateChanged(this auto&& self) {
        if (!self.Key().networked) return;
        const auto thisPtr = self.ThisPtr();
        const auto offset = self.Offset();
        if (const auto chain = self.Chain(); chain != 0) {
            ::ChainNetworkStateChanged(thisPtr + chain, offset);
        } else if constexpr (ThisClass::m_networkStateChangedOffset) {
            ::NetworkVarStateChanged(thisPtr, offset, ThisClass::m_networkStateChangedOffset);
        } else {
            ::EntityNetworkStateChanged(thisPtr, offset);
        }
    }
};
#endif

// macro helpers: trivial, localize offsetof usage to callsites only
#define SCHEMA_FIELD_OFFSET(type, name, extra) \
private: \
	static constexpr const char name##_str[] = #name; \
	static constexpr size_t name##_offset() { return offsetof(ThisClass, name); }; \
public: \
    SchemaField<type, ThisClass, name##_str, name##_offset, (extra)> name;

#define SCHEMA_FIELD(type, name) \
    SCHEMA_FIELD_OFFSET(type, name, 0)

#define SCHEMA_FIELD_POINTER_OFFSET(type, name, extra) \
private: \
	static constexpr const char name##_str[] = #name; \
	static constexpr size_t name##_offset() { return offsetof(ThisClass, name); }; \
public: \
	SchemaPointerField<type, ThisClass, name##_str, name##_offset, (extra)> name;

#define SCHEMA_FIELD_POINTER(type, name) \
    SCHEMA_FIELD_POINTER_OFFSET(type, name, 0)

#if 0
// Helper macros for common parts
#define SCHEMA_FIELD_COMMON_HELPERS(type, varName, extra_offset) \
    using ptr_t = std::conditional_t< \
        std::is_const_v<std::remove_reference_t<type>>, \
        const type*, \
        type* \
    >; \
    using ref_t = std::conditional_t< \
        std::is_const_v<std::remove_reference_t<type>>, \
        const type&, \
        type& \
    >; \
    \
    [[nodiscard]] auto Key(this auto&& self) { \
        static const auto key = schema::GetOffset(ThisClass::m_className, #varName); \
        return key; \
    } \
    [[nodiscard]] auto Chain(this auto&& self) { \
        static const auto chain = schema::FindChainOffset(ThisClass::m_className); \
        return chain; \
    } \
    [[nodiscard]] auto Offset(this auto&& self) { return self.Key().offset + (extra_offset); } \
    [[nodiscard]] auto ThisPtr(this auto&& self) { return reinterpret_cast<uintptr_t>(&self) - offsetof(ThisClass, varName); }

// Main value field macro
#define SCHEMA_FIELD_OFFSET(type, varName, extra_offset) \
    class varName##_prop { \
        SCHEMA_FIELD_COMMON_HELPERS(type, varName, extra_offset) \
        \
    public: \
        [[nodiscard]] ref_t Get(this auto&& self) { \
            return *reinterpret_cast<ptr_t>(self.ThisPtr() + self.Offset()); \
        } \
        \
        void Set(this auto&& self, const type& val) { \
            self.NotifyNetworkChange(); \
            *reinterpret_cast<ptr_t>(self.ThisPtr() + self.Offset()) = val; \
        } \
        \
        [[nodiscard]] operator ref_t(this auto&& self) { return self.Get(); } \
        [[nodiscard]] auto operator->(this auto&& self) requires (std::is_pointer_v<type>) { return self.Get(); } \
        [[nodiscard]] auto operator->(this auto&& self) requires (!std::is_pointer_v<type>) { return std::addressof(self.Get()); } \
        [[nodiscard]] auto operator*(this auto&& self) requires (std::is_pointer_v<type>) { return *self.Get(); } \
        [[nodiscard]] auto operator*(this auto&& self) requires (!std::is_pointer_v<type>) { return self.Get(); } \
        [[nodiscard]] auto operator[](this auto&& self, auto idx) { return self.Get()[idx]; } \
        [[nodiscard]] auto operator+(this auto&& self, auto v) { return self.Get() + v; } \
        [[nodiscard]] auto operator-(this auto&& self, auto v) { return self.Get() - v; } \
        [[nodiscard]] bool operator==(this auto&& self, const auto& v) { return self.Get() == v; } \
        [[nodiscard]] auto operator<=>(this auto&& self, const auto& v) { return self.Get() <=> v; } \
        \
        auto& operator=(this auto&& self, const type& val) { \
            self.Set(val); return self; \
        } \
        [[nodiscard]] bool operator==(this auto&& self, std::nullptr_t) requires (std::is_pointer_v<type>) { \
            return self.Get() == nullptr; \
        } \
        \
        auto& operator+=(this auto&& self, const auto& v) requires std::is_arithmetic_v<type> { \
            self.Set(self.Get() + v); return self; \
        } \
        auto& operator-=(this auto&& self, const auto& v) requires std::is_arithmetic_v<type> { \
            self.Set(self.Get() - v); return self; \
        } \
        auto& operator++(this auto&& self) requires std::is_arithmetic_v<type> { \
            self.Set(self.Get() + 1); return self; \
        } \
        auto& operator--(this auto&& self) requires std::is_arithmetic_v<type> { \
            self.Set(self.Get() - 1); return self; \
        } \
        [[nodiscard]] auto operator++(this auto&& self, int) requires std::is_arithmetic_v<type> { \
            auto old = self.Get(); self.Set(old + 1); return old; \
        } \
        [[nodiscard]] auto operator--(this auto&& self, int) requires std::is_arithmetic_v<type> { \
            auto old = self.Get(); self.Set(old - 1); return old; \
        } \
        \
        auto& operator*=(this auto&& self, const auto& v) requires std::is_arithmetic_v<type> { \
            self.Set(self.Get() * v); return self; \
        } \
        auto& operator/=(this auto&& self, const auto& v) requires std::is_arithmetic_v<type> { \
            self.Set(self.Get() / v); return self; \
        } \
        [[nodiscard]] auto operator*(this auto&& self, const auto& v) requires std::is_arithmetic_v<type> { \
            return self.Get() * v; \
        } \
        [[nodiscard]] auto operator/(this auto&& self, const auto& v) requires std::is_arithmetic_v<type> { \
            return self.Get() / v; \
        } \
        \
        auto& operator|=(this auto&& self, const auto& v) requires std::is_integral_v<type> { \
            self.Set(self.Get() | v); return self; \
        } \
        auto& operator&=(this auto&& self, const auto& v) requires std::is_integral_v<type> { \
            self.Set(self.Get() & v); return self; \
        } \
        auto& operator^=(this auto&& self, const auto& v) requires std::is_integral_v<type> { \
            self.Set(self.Get() ^ v); return self; \
        } \
        [[nodiscard]] auto operator|(this auto&& self, const auto& v) requires std::is_integral_v<type> { \
            return self.Get() | v; \
        } \
        [[nodiscard]] auto operator&(this auto&& self, const auto& v) requires std::is_integral_v<type> { \
            return self.Get() & v; \
        } \
        [[nodiscard]] auto operator^(this auto&& self, const auto& v) requires std::is_integral_v<type> { \
            return self.Get() ^ v; \
        } \
        \
        void NotifyNetworkChange(this auto&& self) { \
            if (!self.Key().networked) \
                return; \
            const auto thisPtr = self.ThisPtr(); \
            const auto offset = self.Offset(); \
            if (const auto chain = self.Chain(); chain != 0) { \
                ::ChainNetworkStateChanged(thisPtr + chain, offset); \
            } else if constexpr (ThisClass::m_networkStateChangedOffset) { \
                ::NetworkVarStateChanged(thisPtr, offset, ThisClass::m_networkStateChangedOffset); \
            } else { \
                ::EntityNetworkStateChanged(thisPtr, offset); \
            } \
        } \
        \
    private: \
        varName##_prop(const varName##_prop&) = delete; \
    } varName;

#define SCHEMA_FIELD(type, varName) \
    SCHEMA_FIELD_OFFSET(type, varName, 0)

// Pointer field macro
#define SCHEMA_FIELD_POINTER_OFFSET(type, varName, extra_offset) \
    class varName##_prop { \
        using ptr_t = std::conditional_t< \
            std::is_const_v<std::remove_reference_t<type>>, \
            const type*, \
            type* \
        >; \
        \
        [[nodiscard]] auto Key(this auto&& self) { \
            static const auto key = schema::GetOffset(ThisClass::m_className, #varName); \
            return key; \
        } \
        [[nodiscard]] auto Chain(this auto&& self) { \
            static const auto chain = schema::FindChainOffset(ThisClass::m_className); \
            return chain; \
        } \
        [[nodiscard]] auto Offset(this auto&& self) { return self.Key().offset + (extra_offset); } \
        [[nodiscard]] auto ThisPtr(this auto&& self) { return reinterpret_cast<uintptr_t>(&self) - offsetof(ThisClass, varName); } \
        \
    public: \
        [[nodiscard]] auto Get(this auto&& self) { \
            return reinterpret_cast<ptr_t>(self.ThisPtr() + self.Offset()); \
        } \
        \
        [[nodiscard]] operator ptr_t(this auto&& self) { return self.Get(); } \
        [[nodiscard]] auto operator->(this auto&& self) { return self.Get(); } \
        [[nodiscard]] auto operator*(this auto&& self) { return *self.Get(); } \
        [[nodiscard]] decltype(auto) operator[](this auto&& self, auto idx) { return self.Get()[idx]; } \
        [[nodiscard]] bool operator==(this auto&& self, auto ptr) { return self.Get() == ptr; } \
        \
        void NetworkStateChanged(this auto&& self) { \
            if (!self.Key().networked) return; \
            const auto thisPtr = self.ThisPtr(); \
            const auto offset = self.Offset(); \
            if (const auto chain = self.Chain(); chain != 0) { \
                ::ChainNetworkStateChanged(thisPtr + chain, offset); \
            } else if constexpr (ThisClass::m_networkStateChangedOffset) { \
                ::NetworkVarStateChanged(thisPtr, offset, ThisClass::m_networkStateChangedOffset); \
            } else { \
                ::EntityNetworkStateChanged(thisPtr, offset); \
            } \
        } \
        \
    private: \
        varName##_prop(const varName##_prop&) = delete; \
    } varName;

#define SCHEMA_FIELD_POINTER(type, name) \
	SCHEMA_FIELD_POINTER_OFFSET(type, name, 0)
#endif

// If the class needs a specific offset for its NetworkStateChanged (like CEconItemView), use this and provide the offset
#define DECLARE_SCHEMA_CLASS_BASE(ClassName, offset)								\
	public:																		    \
		using ThisClass = ClassName;												\
		static constexpr const char m_className[] = #ClassName;						\
		static constexpr int m_networkStateChangedOffset = offset;                  \

#define DECLARE_SCHEMA_CLASS(className) DECLARE_SCHEMA_CLASS_BASE(className, 0)

// Use this for non-entity classes such as CCollisionProperty or CGlowProperty
// The only difference is that their NetworkStateChanged function is index 1 on their vtable rather than being CEntityInstance::NetworkStateChanged
// Though some classes like CGameRules will instead use their CNetworkVarChainer as a link back to the parent entity
#define DECLARE_SCHEMA_CLASS_INLINE(className) DECLARE_SCHEMA_CLASS_BASE(className, 1)