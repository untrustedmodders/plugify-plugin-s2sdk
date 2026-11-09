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

#include "schema.hpp"
#include "schema_helpers.hpp"
#include "utils.hpp"

#include <core/core_config.hpp>

#include <schemasystem/schemasystem.h>
#include <entity2/entitynetwork.h>

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
	if (chainOffset >= 0) {
		ChainNetworkStateChanged(entity + chainOffset, offset);
	} else {
		if (chainOffset == -1)
			EntityNetworkStateChanged(entity, offset);
		else
			NetworkVarStateChanged(entity, offset, 1);
	}
}

namespace {
	using SchemaValueMap = plg::flat_hash_map<plg::string, SchemaKey, plg::string_hash, std::equal_to<>>;
	using SchemaTableMap = plg::flat_hash_map<plg::string, SchemaValueMap, plg::string_hash, std::equal_to<>>;

	void CollectSchemaFields(
		CSchemaClassInfo& cls,
		SchemaValueMap& out,
		std::string_view derivedName
	) {
		for (uint8 i = 0; i < cls.m_nBaseClassCount; ++i) {
			const SchemaBaseClassInfoData_t& base = cls.m_pBaseClasses[i];
			CollectSchemaFields(*base.m_pClass, out, derivedName);
		}

		const auto& cli = g_pEntityNetworkSerializerInfo->m_ClassInfos;
		const int index = cli.Find(cls.m_pszName);

		for (uint16 i = 0; i < cls.m_nFieldCount; ++i) {
			const SchemaClassFieldData_t& field = cls.m_pFields[i];

			int size = 0;
			uint8 alignment = 0;
			field.m_pType->GetSizeAndAlignment(size, alignment);

			out.try_emplace(
				field.m_pszName,
				field.m_nSingleInheritanceOffset,
				index != cli.InvalidIndex() && cli[index]->FindField(field.m_pszName),
				static_cast<size_t>(size),
				field.m_pType
			);

			plg::print(
				LS_DETAILED,
				"{}::{} found at -> 0x{:x} - {}\n",
				derivedName,
				field.m_pszName,
				field.m_nSingleInheritanceOffset,
				static_cast<const void*>(&field)
			);
		}
	}

	SchemaValueMap InitSchemaFieldsForClass(std::string_view moduleName, std::string_view className) {
		auto* scope = g_pSchemaSystem->FindTypeScopeForModule(moduleName.data());
		if (!scope) {
			plg::print(LS_WARNING, "'{}' module was not found!\n", moduleName);
			return {};
		}

		auto cls = scope->FindDeclaredClass(className.data());
		if (!cls) {
			plg::print(LS_WARNING, "'{}' class was not found!\n", className);
			return {};
		}

		SchemaValueMap map;
		CollectSchemaFields(*cls, map, className);
		return map;
	}

	SchemaTableMap schemaTableMap;
	std::shared_mutex schemaMutex;
}// namespace

namespace schema {
	int32_t FindChainOffset(std::string_view className) {
		return FindOffset(className, chainKey).offset;
	}

	SchemaKey FindOffset(std::string_view className, std::string_view memberName) {
		{
			std::shared_lock lock(schemaMutex);
			auto it = schemaTableMap.find(className);
			if (it != schemaTableMap.end()) {
				const auto& table = it->second;
				auto it2 = table.find(memberName);
				if (it2 != table.end()) {
					return it2->second;
				}
				if (chainKey != memberName) {
					plg::print(
						LS_WARNING,
						"'{}' was not found in '{}'!\n",
						memberName,
						className
					);
				}
				return {};
			}
		}

		{
			std::unique_lock lock(schemaMutex);
			schemaTableMap.emplace(className, InitSchemaFieldsForClass(moduleName, className));
		}

		return FindOffset(className, memberName);
	}

	int32_t FindChainOffset(const char* className) {
		return FindOffset(className, chainKey).offset;
	}

	SchemaKey FindOffset(const char* className, const char* memberName) {
		return FindOffset(std::string_view(className), std::string_view(memberName));
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

	ElementSizedType IsIntType(CSchemaType* type) {
		switch (type->m_eTypeCategory) {
			case SCHEMA_TYPE_FIXED_ARRAY: {
				auto* elementType = static_cast<const CSchemaType_FixedArray*>(type)->m_pElementType;
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
						int size = static_cast<CSchemaType_DeclaredClass*>(elementType)->m_pClassInfo->m_nSize;
						if (size <= sizeof(double)) {
							return {Array, size};
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
				int size = static_cast<CSchemaType_DeclaredClass*>(type)->m_pClassInfo->m_nSize;
				if (size <= sizeof(void*)) {
					return {Single, size};
				}
				return {Class, -1};
			}
			case SCHEMA_TYPE_ATOMIC: {
				switch (type->m_eAtomicCategory) {
					case SCHEMA_ATOMIC_PLAIN:
						switch (static_cast<CSchemaType_Atomic*>(type)->m_nSize) {
							case sizeof(int8):
								return {Single, static_cast<int>(sizeof(int8))};
							case sizeof(int16):
								return {Single, static_cast<int>(sizeof(int16))};
							case sizeof(int32):
								return {Single, static_cast<int>(sizeof(int32))};
							case sizeof(int64):
								return {Single, static_cast<int>(sizeof(int64))};
							default:
								return {Invalid, -1};
						}
					case SCHEMA_ATOMIC_COLLECTION_OF_T:
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
								return {Invalid, -1};
						}
					default:
						return {Invalid, -1};
				}
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
						return {Single, static_cast<int>(sizeof(uint64))};
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

	ElementSizedType IsFloatType(CSchemaType* type) {
		switch (type->m_eTypeCategory) {
			case SCHEMA_TYPE_FIXED_ARRAY: {
				auto* elementType = static_cast<const CSchemaType_FixedArray*>(type)->m_pElementType;
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
						int size = static_cast<CSchemaType_DeclaredClass*>(elementType)->m_pClassInfo->m_nSize;
						if (size <= sizeof(double)) {
							return {Array, size};
						}
						return {Invalid, -1};
					}
					default:
						break;
				}
				return {Invalid, -1};
			}
			case SCHEMA_TYPE_DECLARED_CLASS: {
				int size = static_cast<const CSchemaType_DeclaredClass*>(type)->m_pClassInfo->m_nSize;
				if (size <= sizeof(double)) {
					return {Single, size};
				}
				return {Invalid, -1};
			}
			case SCHEMA_TYPE_ATOMIC: {
				if (type->m_eAtomicCategory == SCHEMA_ATOMIC_COLLECTION_OF_T) {
					switch (static_cast<const CSchemaType_Atomic_CollectionOfT*>(type)->m_nElementSize) {
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
				switch (static_cast<const CSchemaType_Builtin*>(type)->m_eBuiltinType) {
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
				auto* elementType = static_cast<const CSchemaType_FixedArray*>(type)->m_pElementType;
				if (elementType->m_eTypeCategory == SCHEMA_TYPE_ATOMIC && elementType->m_eAtomicCategory == SCHEMA_ATOMIC_PLAIN) {
					if (static_cast<CSchemaType_Atomic*>(elementType)->m_nSize == size)
						return Array;
				}
				return Invalid;
			}
			case SCHEMA_TYPE_ATOMIC: {
				switch (type->m_eAtomicCategory) {
					case SCHEMA_ATOMIC_PLAIN:
						if (static_cast<const CSchemaType_Atomic*>(type)->m_nSize == size)
							return Single;
						break;
					case SCHEMA_ATOMIC_COLLECTION_OF_T:
						if (static_cast<const CSchemaType_Atomic_CollectionOfT*>(type)->m_nElementSize == size)
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
				auto* elementType = static_cast<const CSchemaType_FixedArray*>(type)->m_pElementType;
				if (elementType->m_eTypeCategory == SCHEMA_TYPE_ATOMIC && elementType->m_eAtomicCategory == SCHEMA_ATOMIC_T) {
					if (static_cast<CSchemaType_Atomic_T*>(elementType)->m_nSize == size)
						return Array;
				}
				return Invalid;
			}
			case SCHEMA_TYPE_ATOMIC: {
				switch (type->m_eAtomicCategory) {
					case SCHEMA_ATOMIC_T:
						if (static_cast<const CSchemaType_Atomic_T*>(type)->m_nSize == size)
							return Single;
						break;
					case SCHEMA_ATOMIC_COLLECTION_OF_T:
						if (static_cast<const CSchemaType_Atomic_CollectionOfT*>(type)->m_nElementSize == size)
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

	//
	// Typed access helpers (schema_helpers.hpp)
	//

	void* ResizeUtlVector(intptr_t address, int count, int dataSize) {
		// The layout of CUtlVector (and CNetworkUtlVectorBase) does not depend on T, only the
		// allocation size does, so the element size picks the instantiation.
		void* data = nullptr;
		DispatchSize<int64_t>(dataSize, [&]<typename S>() {
			auto* vec = reinterpret_cast<CUtlVector<S>*>(address);
			vec->SetCount(count);
			data = vec->Base();
		});
		return data;
	}

	void NotifyDataChanged(CEntityInstance* entity, int offset, int chainOffset, bool changeState) {
		if (changeState) {
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		}
	}

	ArrayView GetArrayView(intptr_t base, CSchemaType* type, ElementType elementType) {
		switch (elementType) {
			case Array: {
				const auto* arrayType = static_cast<const CSchemaType_FixedArray*>(type);
				return {base, arrayType->m_nElementCount, arrayType->m_nElementSize};
			}
			case Collection: {
				const auto* vec = reinterpret_cast<CUtlVector<uint8>*>(base);
				const auto* collectionType = static_cast<const CSchemaType_Atomic_CollectionOfT*>(type);
				return {reinterpret_cast<intptr_t>(vec->Base()), vec->Count(), collectionType->m_nElementSize};
			}
			default:
				return {};
		}
	}

	void EntityField::WarnType(std::string_view expected) const {
		plg::print(LS_WARNING, "Schema field '{}::{}' is not a {}, but '{}'\n", className, memberName, expected, key.type->m_sTypeName.Get());
	}

	void* EntityField::Element(ElementType elementType, int element, std::string_view expected) const {
		switch (elementType) {
			case Single:
				return reinterpret_cast<void*>(Base());
			case Array:
			case Collection: {
				const auto view = GetArrayView(Base(), key.type, elementType);
				if (element < 0 || element >= view.count) {
					plg::print(LS_WARNING, "Element {} is out of range for '{}::{}' (size: {})\n", element, className, memberName, view.count);
					return nullptr;
				}
				return reinterpret_cast<void*>(view.data + static_cast<intptr_t>(element) * view.stride);
			}
			default:
				WarnType(expected);
				return nullptr;
		}
	}

	std::pair<void*, int> EntityField::SizedElement(ElementSizedType sized, int element, std::string_view expected) const {
		void* ptr = Element(sized.type, element, expected);
		const int size = sized.type == Single ? static_cast<int>(key.size) : sized.size;
		return {ptr, size};
	}

	void EntityField::NotifyChanged(bool changeState) const {
		if (!changeState) {
			return;
		}
		if (key.networked) {
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), key.offset, FindChainOffset(className));
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	EntityField FindField(CEntityInstance* entity, std::string_view className, std::string_view memberName) {
		const SchemaKey key = FindOffset(className, memberName);
		if (!key) {
			plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
			return {};
		}
		return EntityField{entity, className, memberName, key};
	}

	EntityField FindWritableField(CEntityInstance* entity, std::string_view className, std::string_view memberName, std::string_view action) {
		if (g_pCoreConfig->FollowCS2ServerGuidelines && CS2BadList.contains(memberName)) {
			plg::print(LS_WARNING, "Cannot {} '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", action, className, memberName);
			return {};
		}
		return FindField(entity, className, memberName);
	}
}// namespace schema
