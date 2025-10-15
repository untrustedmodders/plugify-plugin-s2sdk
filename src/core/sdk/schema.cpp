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

#include "schema.h"

#include <schemasystem/schemasystem.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

#include "utils.h"

extern CGlobalVars* gpGlobals;

using SchemaKeyValueMap = std::unordered_map<size_t, SchemaKey>;
using SchemaTableMap = std::unordered_map<size_t, SchemaKeyValueMap>;
static constexpr size_t g_ChainKey = plg::hasher("__m_pChainEntity");

void NetworkVarStateChanged(uintptr_t pNetworkVar, uint32_t nOffset, uint32 nNetworkStateChangedOffset) {
	NetworkStateChanged_t data(nOffset);
	CALL_VIRTUAL(void, nNetworkStateChangedOffset, (void*)pNetworkVar, &data);
}

void EntityNetworkStateChanged(uintptr_t entity, uint offset) {
	NetworkStateChanged_t data(offset);
	reinterpret_cast<CEntityInstance*>(entity)->NetworkStateChanged(data);
}

void ChainNetworkStateChanged(uintptr_t pNetworkVarChainer, uint nLocalOffset) {
	if (CEntityInstance* entity = reinterpret_cast<CNetworkVarChainer*>(pNetworkVarChainer)->GetObject()) {
		entity->NetworkStateChanged(NetworkStateChanged_t(nLocalOffset, -1, reinterpret_cast<CNetworkVarChainer*>(pNetworkVarChainer)->m_PathIndex));
	}
}

void SafeNetworkStateChanged(intptr_t entity, int offset, int chainOffset) {
	if (chainOffset > 0) {
		::ChainNetworkStateChanged(entity + chainOffset, offset);
	} else {
		if (chainOffset == 0)
			::EntityNetworkStateChanged(entity, offset);
		else
			::NetworkVarStateChanged(entity, offset, -chainOffset);
	}
}

namespace {
	bool IsFieldNetworked(const SchemaClassFieldData_t& field) {
		for (int i = 0; i < field.m_nStaticMetadataCount; ++i) {
			std::string_view fieldName(field.m_pStaticMetadata[i].m_pszName);
			if (fieldName == "MNetworkEnable")
				return true;
		}

		return false;
	}

	bool InitSchemaFieldsForClass(SchemaTableMap& tableMap, const char* className, size_t classKey) {
		CSchemaSystemTypeScope* pType = g_pSchemaSystem->FindTypeScopeForModule(S2SDK_LIBRARY_PREFIX "server" S2SDK_LIBRARY_SUFFIX);
		if (!pType)
			return false;

		SchemaMetaInfoHandle_t<CSchemaClassInfo> pClassInfo = pType->FindDeclaredClass(className);
		if (!pClassInfo) {
			tableMap.emplace(classKey, SchemaKeyValueMap());

			plg::print(LS_ERROR, "InitSchemaFieldsForClass(): '{}' was not found!\n", className);
			return false;
		}

		size_t fieldsSize = pClassInfo->m_nFieldCount;
		SchemaClassFieldData_t* fields = pClassInfo->m_pFields;

		SchemaKeyValueMap keyValueMap;
		keyValueMap.reserve(fieldsSize);

		for (size_t i = 0; i < fieldsSize; ++i) {
			const SchemaClassFieldData_t& field = fields[i];

			int size = 0;
			uint8 alignment = 0;
			field.m_pType->GetSizeAndAlignment(size, alignment);
			keyValueMap.emplace(plg::hasher(field.m_pszName), SchemaKey{field.m_nSingleInheritanceOffset, IsFieldNetworked(field), size, field.m_pType});

			//plg::print(LS_DEBUG, "{}::{} found at -> 0x{:x} - {}\reworn", className, field.m_pszName, field.m_nSingleInheritanceOffset, &field);
		}

		tableMap.emplace(classKey, std::move(keyValueMap));

		return true;
	}

}// namespace

namespace schema {
	int32_t FindChainOffset(const char* className, size_t classNameHash) {
		return GetOffset(className, classNameHash, "__m_pChainEntity", g_ChainKey).offset;
	}

	SchemaKey GetOffset(const char* className, size_t classKey, const char* memberName, size_t memberKey) {
		static SchemaTableMap schemaTableMap;

		auto tableIt = schemaTableMap.find(classKey);
		if (tableIt == schemaTableMap.end()) {
			if (InitSchemaFieldsForClass(schemaTableMap, className, classKey))
				return GetOffset(className, classKey, memberName, memberKey);
			return {};
		}

		const auto& tableMap = std::get<SchemaKeyValueMap>(*tableIt);
		auto memberIt = tableMap.find(memberKey);
		if (memberIt != tableMap.end()) {
			return std::get<SchemaKey>(*memberIt);
		} else {
			if (memberKey != g_ChainKey) {
				plg::print(LS_ERROR, "schema::GetOffset(): '{}' was not found in '{}'!\n", memberName, className);
			}
		}

		return {};
	}

	void NetworkStateChanged(intptr_t chainEntity, uint localOffset, int arrayIndex) {
		CNetworkVarChainer* chainEnt = reinterpret_cast<CNetworkVarChainer*>(chainEntity);
		CEntityInstance* entity = chainEnt->GetObject();
		if (entity && !(entity->m_pEntity->m_flags & EF_IS_CONSTRUCTION_IN_PROGRESS)) {
			NetworkStateChanged_t data(localOffset, arrayIndex, chainEnt->m_PathIndex);
			entity->NetworkStateChanged(data);
		}
	}

	ElementType GetElementType(CSchemaType* type) {
		switch (type->m_eTypeCategory) {
			case SCHEMA_TYPE_FIXED_ARRAY: {
				return Array;
			}
			case SCHEMA_TYPE_ATOMIC: {
				if (type->m_eAtomicCategory == SCHEMA_ATOMIC_COLLECTION_OF_T)
					return Collection;
				else
					return Invalid;
			}
			default:
				return Single;
		}
		PLUGIFY_UNREACHABLE();
	}

	std::pair<ElementType, int> IsIntType(CSchemaType* type) {
		switch (type->m_eTypeCategory) {
			case SCHEMA_TYPE_FIXED_ARRAY: {
				auto* elementType = static_cast<CSchemaType_FixedArray*>(type)->m_pElementType;
				switch (elementType->m_eTypeCategory) {
					case SCHEMA_TYPE_BUILTIN: {
						switch (static_cast<CSchemaType_Builtin*>(elementType)->m_eBuiltinType) {
							case SCHEMA_BUILTIN_TYPE_BOOL:
								return {Array, static_cast<int>(sizeof(bool))};
							case SCHEMA_BUILTIN_TYPE_CHAR:
								return {Array, static_cast<int>(sizeof(char))};
							case SCHEMA_BUILTIN_TYPE_INT8:
								return {Array, static_cast<int>(sizeof(int8))};
							case SCHEMA_BUILTIN_TYPE_UINT8:
								return {Array, static_cast<int>(sizeof(uint8))};
							case SCHEMA_BUILTIN_TYPE_INT16:
								return {Array, static_cast<int>(sizeof(int16))};
							case SCHEMA_BUILTIN_TYPE_UINT16:
								return {Array, static_cast<int>(sizeof(uint16))};
							case SCHEMA_BUILTIN_TYPE_INT32:
								return {Array, static_cast<int>(sizeof(int32))};
							case SCHEMA_BUILTIN_TYPE_UINT32:
								return {Array, static_cast<int>(sizeof(uint32))};
							case SCHEMA_BUILTIN_TYPE_INT64:
								return {Array, static_cast<int>(sizeof(int64))};
							case SCHEMA_BUILTIN_TYPE_UINT64:
								return {Array, static_cast<int>(sizeof(uint64_t))};
							case SCHEMA_BUILTIN_TYPE_COUNT:
								return {Array, static_cast<int>(sizeof(size_t))};
							default:
								return {Invalid, -1};
						}
					}
					case SCHEMA_TYPE_DECLARED_CLASS: {
						int nSize = static_cast<CSchemaType_DeclaredClass*>(elementType)->m_pClassInfo->m_nSize;
						if (nSize <= sizeof(double)) {
							return {Array, nSize};
						}
						return {Invalid, -1};
					}
					case SCHEMA_TYPE_DECLARED_ENUM:
						return {Array, static_cast<CSchemaType_DeclaredEnum*>(elementType)->m_pEnumInfo->m_nSize};
					case SCHEMA_TYPE_POINTER:
						return {Array, static_cast<int>(sizeof(void*))};
					default:
						break;
				}
				return {Invalid, -1};
			}
			case SCHEMA_TYPE_DECLARED_CLASS: {
				int nSize = static_cast<CSchemaType_DeclaredClass*>(type)->m_pClassInfo->m_nSize;
				if (nSize <= sizeof(void*)) {
					return {Single, nSize};
				}
				return {Class, -1};
			}
			case SCHEMA_TYPE_ATOMIC: {
				if (type->m_eAtomicCategory == SCHEMA_ATOMIC_COLLECTION_OF_T) {
					switch (static_cast<CSchemaType_Atomic_CollectionOfT*>(type)->m_nElementSize) {
						case sizeof(int8):
							return {Collection, static_cast<int>(sizeof(int8))};
						case sizeof(int16):
							return {Collection, static_cast<int>(sizeof(int16))};
						case sizeof(int32):
							return {Collection, static_cast<int>(sizeof(int32))};
						case sizeof(int64):
							return {Collection, static_cast<int>(sizeof(int64))};
						default:
							break;
					}
				}
				return {Invalid, -1};
			}
			case SCHEMA_TYPE_BUILTIN: {
				switch (static_cast<CSchemaType_Builtin*>(type)->m_eBuiltinType) {
					case SCHEMA_BUILTIN_TYPE_BOOL:
						return {Single, static_cast<int>(sizeof(bool))};
					case SCHEMA_BUILTIN_TYPE_CHAR:
						return {Single, static_cast<int>(sizeof(char))};
					case SCHEMA_BUILTIN_TYPE_INT8:
						return {Single, static_cast<int>(sizeof(int8))};
					case SCHEMA_BUILTIN_TYPE_UINT8:
						return {Single, static_cast<int>(sizeof(uint8))};
					case SCHEMA_BUILTIN_TYPE_INT16:
						return {Single, static_cast<int>(sizeof(int16))};
					case SCHEMA_BUILTIN_TYPE_UINT16:
						return {Single, static_cast<int>(sizeof(uint16))};
					case SCHEMA_BUILTIN_TYPE_INT32:
						return {Single, static_cast<int>(sizeof(int32))};
					case SCHEMA_BUILTIN_TYPE_UINT32:
						return {Single, static_cast<int>(sizeof(uint32))};
					case SCHEMA_BUILTIN_TYPE_INT64:
						return {Single, static_cast<int>(sizeof(int64))};
					case SCHEMA_BUILTIN_TYPE_UINT64:
						return {Single, static_cast<int>(sizeof(uint64_t))};
					case SCHEMA_BUILTIN_TYPE_COUNT:
						return {Single, static_cast<int>(sizeof(size_t))};
					default:
						return {Invalid, -1};
				}
			}
			case SCHEMA_TYPE_DECLARED_ENUM: {
				return {Single, static_cast<CSchemaType_DeclaredEnum*>(type)->m_pEnumInfo->m_nSize};
			}
			case SCHEMA_TYPE_POINTER: {
				return {Single, static_cast<int>(sizeof(void*))};
			}
			default: {
				return {Invalid, -1};
			}
		}
		PLUGIFY_UNREACHABLE();
	}

	std::pair<ElementType, int> IsFloatType(CSchemaType* type) {
		switch (type->m_eTypeCategory) {
			case SCHEMA_TYPE_FIXED_ARRAY: {
				auto* elementType = static_cast<CSchemaType_FixedArray*>(type)->m_pElementType;
				switch (elementType->m_eTypeCategory) {
					case SCHEMA_TYPE_BUILTIN: {
						auto* elementTypeBuiltin = static_cast<CSchemaType_Builtin*>(elementType);
						switch (elementTypeBuiltin->m_eBuiltinType) {
							case SCHEMA_BUILTIN_TYPE_FLOAT32:
								return {Array, static_cast<int>(sizeof(float32))};
							case SCHEMA_BUILTIN_TYPE_FLOAT64:
								return {Array, static_cast<int>(sizeof(float64))};
							default:
								return {Invalid, -1};
						}
					}
					case SCHEMA_TYPE_DECLARED_CLASS: {
						int nSize = static_cast<CSchemaType_DeclaredClass*>(elementType)->m_pClassInfo->m_nSize;
						if (nSize <= sizeof(double)) {
							return {Array, nSize};
						}
						return {Invalid, -1};
					}
					default:
						break;
				}
				return {Invalid, -1};
			}
			case SCHEMA_TYPE_DECLARED_CLASS: {
				int nSize = static_cast<CSchemaType_DeclaredClass*>(type)->m_pClassInfo->m_nSize;
				if (nSize <= sizeof(double)) {
					return {Single, nSize};
				}
				return {Invalid, -1};
			}
			case SCHEMA_TYPE_ATOMIC: {
				if (type->m_eAtomicCategory == SCHEMA_ATOMIC_COLLECTION_OF_T) {
					switch (static_cast<CSchemaType_Atomic_CollectionOfT*>(type)->m_nElementSize) {
						case sizeof(float32):
							return {Collection, static_cast<int>(sizeof(float32))};
						case sizeof(float64):
							return {Collection, static_cast<int>(sizeof(float64))};
						default:
							break;
					}
				}
				return {Invalid, -1};
			}
			case SCHEMA_TYPE_BUILTIN: {
				switch (static_cast<CSchemaType_Builtin*>(type)->m_eBuiltinType) {
					case SCHEMA_BUILTIN_TYPE_FLOAT32:
						return {Single, static_cast<int>(sizeof(float32))};
					case SCHEMA_BUILTIN_TYPE_FLOAT64:
						return {Single, static_cast<int>(sizeof(float64))};
					default:
						return {Invalid, -1};
				}
			}
			default:
				return {Invalid, -1};
		}
		PLUGIFY_UNREACHABLE();
	}

	ElementType IsPlainType(CSchemaType* type, size_t size) {
		switch (type->m_eTypeCategory) {
			case SCHEMA_TYPE_FIXED_ARRAY: {
				auto* elementType = static_cast<CSchemaType_FixedArray*>(type)->m_pElementType;
				if (elementType->m_eTypeCategory == SCHEMA_TYPE_ATOMIC && elementType->m_eAtomicCategory == SCHEMA_ATOMIC_PLAIN) {
					if (static_cast<CSchemaType_Atomic*>(elementType)->m_nSize == size)
						return Array;
				}
				return Invalid;
			}
			case SCHEMA_TYPE_ATOMIC: {
				switch (type->m_eAtomicCategory) {
					case SCHEMA_ATOMIC_PLAIN:
						if (static_cast<CSchemaType_Atomic*>(type)->m_nSize == size)
							return Single;
						break;
					case SCHEMA_ATOMIC_COLLECTION_OF_T:
						if (static_cast<CSchemaType_Atomic_CollectionOfT*>(type)->m_nElementSize == size)
							return Collection;
						break;
					default:
						return Invalid;
				}
				PLUGIFY_UNREACHABLE();
			}
			default:
				return Invalid;
		}
		PLUGIFY_UNREACHABLE();
	}

	ElementType IsAtomicType(CSchemaType* type, size_t size) {
		switch (type->m_eTypeCategory) {
			case SCHEMA_TYPE_FIXED_ARRAY: {
				auto* elementType = static_cast<CSchemaType_FixedArray*>(type)->m_pElementType;
				if (elementType->m_eTypeCategory == SCHEMA_TYPE_ATOMIC && elementType->m_eAtomicCategory == SCHEMA_ATOMIC_T) {
					if (static_cast<CSchemaType_Atomic_T*>(elementType)->m_nSize == size)
						return Array;
				}
				return Invalid;
			}
			case SCHEMA_TYPE_ATOMIC: {
				switch (type->m_eAtomicCategory) {
					case SCHEMA_ATOMIC_T:
						if (static_cast<CSchemaType_Atomic_T*>(type)->m_nSize == size)
							return Single;
						break;
					case SCHEMA_ATOMIC_COLLECTION_OF_T:
						if (static_cast<CSchemaType_Atomic_CollectionOfT*>(type)->m_nElementSize == size)
							return Collection;
						break;
					default:
						return Invalid;
				}
				PLUGIFY_UNREACHABLE();
			}
			default:
				return Invalid;
		}
		PLUGIFY_UNREACHABLE();
	}
}// namespace schema
