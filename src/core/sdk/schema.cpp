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

#include "schema.h"
#include "utils.h"

#include <schemasystem/schemasystem.h>

// memdbgon must be the last include file in a .cpp file!!!
#include <tier0/memdbgon.h>

void NetworkVarStateChanged(uintptr_t networkVar, uint32_t offset, uint32_t networkStateChangedOffset) {
	NetworkStateChanged_t data(offset);
	CALL_VIRTUAL(void, networkStateChangedOffset, reinterpret_cast<void*>(networkVar), &data);
}

void EntityNetworkStateChanged(uintptr_t entity, uint32_t offset) {
	NetworkStateChanged_t data(offset);
	reinterpret_cast<CEntityInstance*>(entity)->NetworkStateChanged(data);
}

void ChainNetworkStateChanged(uintptr_t networkVarChainer, uint32_t localOffset) {
	if (CEntityInstance* entity = reinterpret_cast<CNetworkVarChainer*>(networkVarChainer)->GetObject()) {
		entity->NetworkStateChanged(NetworkStateChanged_t(localOffset, -1, reinterpret_cast<CNetworkVarChainer*>(networkVarChainer)->m_PathIndex));
	}
}

void SafeNetworkStateChanged(intptr_t entity, int offset, int chainOffset) {
	if (chainOffset > 0) {
		ChainNetworkStateChanged(entity + chainOffset, offset);
	} else {
		if (chainOffset == 0)
			EntityNetworkStateChanged(entity, offset);
		else
			NetworkVarStateChanged(entity, offset, -chainOffset);
	}
}

namespace {
	using SchemaValueMap = plg::flat_hash_map<plg::string, SchemaKey, plg::string_hash, std::equal_to<>>;
	using SchemaTableMap = plg::flat_hash_map<plg::string, SchemaValueMap, plg::string_hash, std::equal_to<>>;

	bool IsFieldNetworked(const SchemaClassFieldData_t& field) {
		for (int i = 0; i < field.m_nStaticMetadataCount; ++i) {
			std::string_view fieldName(field.m_pStaticMetadata[i].m_pszName);
			if (fieldName == "MNetworkEnable")
				return true;
		}

		return false;
	}

	SchemaValueMap InitSchemaFieldsForClass(std::string_view className) {
		CSchemaSystemTypeScope* pType = g_pSchemaSystem->FindTypeScopeForModule(S2SDK_LIBRARY_PREFIX "server" S2SDK_LIBRARY_SUFFIX);
		if (!pType)
			return {};

		SchemaMetaInfoHandle_t<CSchemaClassInfo> pClassInfo = pType->FindDeclaredClass(className.data());
		if (!pClassInfo) {
			plg::print(LS_ERROR, "InitSchemaFieldsForClass(): '{}' was not found!\n", className);
			return {};
		}

		size_t fieldsSize = pClassInfo->m_nFieldCount;
		SchemaClassFieldData_t* fields = pClassInfo->m_pFields;

		SchemaValueMap valueMap;
		valueMap.reserve(fieldsSize);

		for (size_t i = 0; i < fieldsSize; ++i) {
			const SchemaClassFieldData_t& field = fields[i];

			int size = 0;
			uint8 alignment = 0;
			field.m_pType->GetSizeAndAlignment(size, alignment);
			valueMap.emplace(field.m_pszName, SchemaKey{field.m_nSingleInheritanceOffset, IsFieldNetworked(field), static_cast<size_t>(size), field.m_pType});

			plg::print(LS_DETAILED, "{}::{} found at -> 0x{:x} - {}\n", className, field.m_pszName, field.m_nSingleInheritanceOffset, static_cast<const void*>(&field));
		}

		return valueMap;
	}

	SchemaTableMap schemaTableMap;
	std::shared_mutex schemaMutex;
}// namespace

namespace schema {
	static constexpr std::string_view g_ChainKey = "__m_pChainEntity";

	int32_t FindChainOffset(std::string_view className) {
		return GetOffset(className, g_ChainKey).offset;
	}

	SchemaKey GetOffset(std::string_view className, std::string_view memberName) {
		{
			std::shared_lock lock(schemaMutex);
			auto it = schemaTableMap.find(className);
			if (it != schemaTableMap.end()) {
				auto& table = it->second;
				auto it2 = table.find(memberName);
				if (it2 != table.end()) {
					return it2->second;
				}
				if (g_ChainKey != memberName) {
					plg::print(
						LS_ERROR,
						"schema::GetOffset(): '{}' was not found in '{}'!\n",
						memberName,
						className
					);
				}
				return {};
			}
		}

		{
			std::unique_lock lock(schemaMutex);
			schemaTableMap.emplace(className, InitSchemaFieldsForClass(className));
		}

		return GetOffset(className, memberName);
	}

	int32_t FindChainOffset(const char* className) {
		return GetOffset(className, g_ChainKey).offset;
	}

	SchemaKey GetOffset(const char* className, const char* memberName) {
		return GetOffset(std::string_view(className), std::string_view(memberName));
	}

	void NetworkStateChanged(intptr_t chainEntity, int32_t localOffset, int32_t arrayIndex) {
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
				int size = static_cast<CSchemaType_DeclaredClass*>(type)->m_pClassInfo->m_nSize;
				if (size <= sizeof(double)) {
					return {Single, size};
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
