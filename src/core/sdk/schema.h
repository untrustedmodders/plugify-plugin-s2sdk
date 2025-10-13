/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2024 Source2ZE
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
	int32 offset{};
	bool networked{};
	int32 size{};
	CSchemaType* type{};
};

namespace plg {
	constexpr std::size_t hasher(std::string_view sv) noexcept {
		std::size_t hash = active_hash_traits::fnv_basis; // FNV-1a
		for (const auto& c : sv) {
			hash ^= static_cast<uint8_t>(c);
			hash *= active_hash_traits::fnv_prime;
		}
		return hash;
	}
}

namespace schema {
	static std::unordered_set<plg::string> CS2BadList = {
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

	int32_t FindChainOffset(const char* className, size_t classNameHash);
	SchemaKey GetOffset(const char* className, size_t classKey, const char* memberName, size_t memberKey);
	void NetworkStateChanged(intptr_t chainEntity, uint localOffset, int arrayIndex = 0xFFFFFFFF);

	inline int32_t FindChainOffset(std::string_view className) {
		return FindChainOffset(className.data(), plg::hasher(className));
	}
	inline SchemaKey GetOffset(std::string_view className, std::string_view memberName) {
		return GetOffset(className.data(), plg::hasher(className), memberName.data(), plg::hasher(memberName));
	}

	ElementType GetElementType(CSchemaType* type);
	std::pair<ElementType, int> IsIntType(CSchemaType* type);
	std::pair<ElementType, int> IsFloatType(CSchemaType* type);
	ElementType IsPlainType(CSchemaType* type, size_t size);
	ElementType IsAtomicType(CSchemaType* type, size_t size);

}// namespace schema

void EntityNetworkStateChanged(uintptr_t pEntity, uint32_t nOffset);
void ChainNetworkStateChanged(uintptr_t pNetworkVarChainer, uint32_t nOffset);
void NetworkVarStateChanged(uintptr_t pNetworkVar, uint32_t nOffset, uint32_t nNetworkStateChangedOffset);

void SafeNetworkStateChanged(intptr_t pEntity, int offset, int chainOffset);

class CBaseEntity;

#define SCHEMA_FIELD_OFFSET(type, varName, extra_offset)                                              \
	PLUGIFY_NO_UNIQUE_ADDRESS class varName##_prop {                                                  \
		using value_t = type;                                                                         \
		using ref_t = std::add_lvalue_reference_t<type>;                                              \
		using ptr_t = std::add_pointer_t<type>;                                                       \
                                                                                                      \
		const SchemaKey& GetKey() {                                                                   \
			static const auto key = schema::GetOffset(                                                \
			    m_className,                                                                          \
			    m_classNameHash,                                                                      \
			    #varName,                                                                             \
			    m_varNameHash                                                                         \
			);                                                                                        \
			return key;                                                                               \
		}                                                                                             \
                                                                                                      \
		int32_t GetChain() {                                                                          \
			static const auto chain = schema::FindChainOffset(m_className, m_classNameHash);          \
			return chain;                                                                             \
		}                                                                                             \
                                                                                                      \
		int32 GetOffset() {                                                                           \
			static const auto offset = GetKey().offset + extra_offset;                                \
			return offset;                                                                            \
		}                                                                                             \
                                                                                                      \
		uintptr_t GetThisPtr() const {                                                                \
			return reinterpret_cast<uintptr_t>(this) - offsetof(ThisClass, varName);                  \
		}                                                                                             \
                                                                                                      \
	public:                                                                                           \
		[[nodiscard]] ref_t Get() {                                                                   \
			return *reinterpret_cast<ptr_t>(GetThisPtr() + GetOffset());                              \
		}                                                                                             \
		void Set(ref_t val) {                                                                         \
			NetworkStateChanged();                                                                    \
			*reinterpret_cast<ptr_t>(GetThisPtr() + GetOffset()) = val;                               \
		}                                                                                             \
		void NetworkStateChanged() {                                                                  \
			if (!GetKey().networked)                                                                  \
				return;                                                                               \
			if (GetChain() != 0) {                                                                    \
				::ChainNetworkStateChanged(GetThisPtr() + GetChain(), GetOffset());                   \
			} else {                                                                                  \
				if constexpr (!m_networkStateChangedOffset)                                           \
					::EntityNetworkStateChanged(GetThisPtr(), GetOffset());                           \
				else                                                                                  \
					::NetworkVarStateChanged(GetThisPtr(), GetOffset(), m_networkStateChangedOffset); \
			}                                                                                         \
		}                                                                                             \
		[[nodiscard]] operator ref_t() {                                                              \
			return Get();                                                                             \
		}                                                                                             \
		[[nodiscard]] ref_t operator()() {                                                            \
			return Get();                                                                             \
		}                                                                                             \
		[[nodiscard]] ref_t operator->() {                                                            \
			return Get();                                                                             \
		}                                                                                             \
		void operator()(value_t val) {                                                                \
			Set(val);                                                                                 \
		}                                                                                             \
		ref_t operator=(value_t val) {                                                                \
			Set(val);                                                                                 \
			return Get();                                                                             \
		}                                                                                             \
		ref_t operator=(varName##_prop& val) {                                                        \
			Set(val());                                                                               \
			return Get();                                                                             \
		}                                                                                             \
                                                                                                      \
	private:                                                                                          \
		/*Prevent accidentally copying this wrapper class instead of the underlying field*/           \
		varName##_prop(const varName##_prop&) = delete;                                               \
		static constexpr auto m_varNameHash = plg::hasher(#varName);                                  \
	} varName;

#define SCHEMA_FIELD_POINTER_OFFSET(type, varName, extra_offset)                                      \
	PLUGIFY_NO_UNIQUE_ADDRESS class varName##_prop {                                                  \
		using value_t = type;                                                                         \
		using ref_t = std::add_lvalue_reference_t<type>;                                              \
		using ptr_t = std::add_pointer_t<type>;                                                       \
                                                                                                      \
		const SchemaKey& GetKey() {                                                                   \
			static const auto key = schema::GetOffset(                                                \
			    m_className,                                                                          \
			    m_classNameHash,                                                                      \
			    #varName,                                                                             \
			    m_varNameHash                                                                         \
			);                                                                                        \
			return key;                                                                               \
		}                                                                                             \
                                                                                                      \
		int32_t GetChain() {                                                                          \
			static const auto chain = schema::FindChainOffset(m_className, m_classNameHash);          \
			return chain;                                                                             \
		}                                                                                             \
                                                                                                      \
		int32 GetOffset() {                                                                           \
			static const auto offset = GetKey().offset + extra_offset;                                \
			return offset;                                                                            \
		}                                                                                             \
		uintptr_t GetThisPtr() const {                                                                \
			return reinterpret_cast<uintptr_t>(this) - offsetof(ThisClass, varName);                  \
		}                                                                                             \
                                                                                                      \
	public:                                                                                           \
		[[nodiscard]] ptr_t Get() {                                                                   \
			return reinterpret_cast<ptr_t>(GetThisPtr() + GetOffset());                               \
		}                                                                                             \
		void NetworkStateChanged() /*Call this after editing the field*/                              \
		{                                                                                             \
			if (!GetKey().networked)                                                                  \
				return;                                                                               \
			if (GetChain() != 0) {                                                                    \
				::ChainNetworkStateChanged(GetThisPtr() + GetChain(), GetOffset());                   \
			} else {                                                                                  \
				if constexpr (!m_networkStateChangedOffset)                                           \
					::EntityNetworkStateChanged(GetThisPtr(), GetOffset());                           \
				else                                                                                  \
					::NetworkVarStateChanged(GetThisPtr(), GetOffset(), m_networkStateChangedOffset); \
			}                                                                                         \
		}                                                                                             \
		[[nodiscard]] operator ptr_t() {                                                              \
			return Get();                                                                             \
		}                                                                                             \
		[[nodiscard]] ptr_t operator()() {                                                            \
			return Get();                                                                             \
		}                                                                                             \
		[[nodiscard]] ptr_t operator->() {                                                            \
			return Get();                                                                             \
		}                                                                                             \
                                                                                                      \
	private:                                                                                          \
		/*Prevent accidentally copying this wrapper class instead of the underlying field*/           \
		varName##_prop(const varName##_prop&) = delete;                                               \
		static constexpr auto m_varNameHash = plg::hasher(#varName);                                  \
	} varName;

// Use this when you want the member's value itself
#define SCHEMA_FIELD(type, varName) \
	SCHEMA_FIELD_OFFSET(type, varName, 0)

// Use this when you want a pointer to a member
#define SCHEMA_FIELD_POINTER(type, varName) \
	SCHEMA_FIELD_POINTER_OFFSET(type, varName, 0)

// If the class needs a specific offset for its NetworkStateChanged (like CEconItemView), use this and provide the offset
#define DECLARE_SCHEMA_CLASS_BASE(ClassName, offset)								\
	private:																		\
		typedef ClassName ThisClass;												\
		static constexpr const char* m_className = #ClassName;						\
		static constexpr size_t m_classNameHash = plg::hasher(#ClassName);          \
		static constexpr int m_networkStateChangedOffset = offset;					\
	public:

#define DECLARE_SCHEMA_CLASS(className) DECLARE_SCHEMA_CLASS_BASE(className, 0)

// Use this for non-entity classes such as CCollisionProperty or CGlowProperty
// The only difference is that their NetworkStateChanged function is index 1 on their vtable rather than being CEntityInstance::NetworkStateChanged
// Though some classes like CGameRules will instead use their CNetworkVarChainer as a link back to the parent entity
#define DECLARE_SCHEMA_CLASS_INLINE(className) DECLARE_SCHEMA_CLASS_BASE(className, 1)