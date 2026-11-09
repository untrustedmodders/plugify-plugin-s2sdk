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
				size,
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

	// A schema type split into its container (single value, fixed array or collection) and its element.
	struct Layout {
		FieldKind kind;
		int stride;                 // size of one element in bytes
		const CSchemaType* element; // may be null for collections with an unknown template type
	};

	static Layout Unwrap(const CSchemaType* type) {
		if (type->m_eTypeCategory == SCHEMA_TYPE_FIXED_ARRAY) {
			const auto* array = static_cast<const CSchemaType_FixedArray*>(type);
			return {FieldKind::Array, array->m_nElementSize, array->m_pElementType};
		}
		if (type->m_eTypeCategory == SCHEMA_TYPE_ATOMIC && type->m_eAtomicCategory == SCHEMA_ATOMIC_COLLECTION_OF_T) {
			const auto* collection = static_cast<const CSchemaType_Atomic_CollectionOfT*>(type);
			return {FieldKind::Collection, collection->m_nElementSize, collection->m_pTemplateType};
		}
		int size = 0;
		uint8 alignment = 0;
		type->GetSizeAndAlignment(size, alignment);
		return {FieldKind::Single, size, type};
	}

	static Storage BuiltinStorage(SchemaBuiltinType_t builtin) {
		switch (builtin) {
			case SCHEMA_BUILTIN_TYPE_BOOL:
			case SCHEMA_BUILTIN_TYPE_UINT8:
				return Storage::U8;
			case SCHEMA_BUILTIN_TYPE_CHAR:
			case SCHEMA_BUILTIN_TYPE_INT8:
				return Storage::I8;
			case SCHEMA_BUILTIN_TYPE_INT16:
				return Storage::I16;
			case SCHEMA_BUILTIN_TYPE_UINT16:
				return Storage::U16;
			case SCHEMA_BUILTIN_TYPE_INT32:
				return Storage::I32;
			case SCHEMA_BUILTIN_TYPE_UINT32:
				return Storage::U32;
			case SCHEMA_BUILTIN_TYPE_INT64:
				return Storage::I64;
			case SCHEMA_BUILTIN_TYPE_UINT64:
				return Storage::U64;
			case SCHEMA_BUILTIN_TYPE_COUNT:
				return sizeof(size_t) == sizeof(uint64) ? Storage::U64 : Storage::U32;
			case SCHEMA_BUILTIN_TYPE_FLOAT32:
				return Storage::F32;
			case SCHEMA_BUILTIN_TYPE_FLOAT64:
				return Storage::F64;
			default:
				return Storage::Invalid;
		}
	}

	// Storage of one numeric value of `type`, which is `size` bytes:
	// - builtins by their own type;
	// - a class wrapping a single field (GameTime_t, CUtlStringToken, ...) by that field;
	// - other small classes, enums, pointers and atomics as signed integers of their size.
	static Storage ScalarStorage(const CSchemaType* type, int size) {
		if (type) {
			switch (type->m_eTypeCategory) {
				case SCHEMA_TYPE_BUILTIN:
					return BuiltinStorage(static_cast<const CSchemaType_Builtin*>(type)->m_eBuiltinType);
				case SCHEMA_TYPE_DECLARED_CLASS: {
					const auto* classInfo = static_cast<const CSchemaType_DeclaredClass*>(type)->m_pClassInfo;
					if (classInfo && classInfo->m_nFieldCount == 1 && classInfo->m_nBaseClassCount == 0) {
						const Storage inner = ScalarStorage(classInfo->m_pFields[0].m_pType, classInfo->m_nSize);
						if (inner != Storage::Invalid && SizeOf(inner) == size) {
							return inner;
						}
					}
					break;
				}
				case SCHEMA_TYPE_DECLARED_ENUM:
				case SCHEMA_TYPE_POINTER:
				case SCHEMA_TYPE_ATOMIC:
					break;
				default:
					return Storage::Invalid;
			}
		}
		return IntStorage(size);
	}

	static constexpr NumericField InvalidSized{FieldKind::Invalid, Storage::Invalid};

	FieldKind GetFieldKind(const CSchemaType* type) {
		return Unwrap(type).kind;
	}

	NumericField MatchInteger(const CSchemaType* type) {
		const auto [kind, stride, element] = Unwrap(type);
		if (const Storage storage = ScalarStorage(element, stride); IsInteger(storage)) {
			return {kind, storage};
		}
		// Structures larger than 8 bytes are exposed by address.
		if (kind == FieldKind::Single && type->m_eTypeCategory == SCHEMA_TYPE_DECLARED_CLASS && stride > static_cast<int>(sizeof(void*))) {
			return {FieldKind::Class, Storage::Invalid};
		}
		return InvalidSized;
	}

	NumericField MatchFloat(const CSchemaType* type) {
		const auto [kind, stride, element] = Unwrap(type);
		if (const Storage storage = ScalarStorage(element, stride); IsFloat(storage)) {
			return {kind, storage};
		}
		return InvalidSized;
	}

	// Matches atomic values (or arrays of them) of the given category and size; collections by element size.
	static FieldKind MatchAtomicCategory(const CSchemaType* type, SchemaAtomicCategory_t category, int size) {
		const auto [kind, stride, element] = Unwrap(type);
		if (stride != size) {
			return FieldKind::Invalid;
		}
		if (kind == FieldKind::Collection) {
			return FieldKind::Collection;
		}
		if (element->m_eTypeCategory == SCHEMA_TYPE_ATOMIC && element->m_eAtomicCategory == category) {
			return kind;
		}
		return FieldKind::Invalid;
	}

	FieldKind MatchPlain(const CSchemaType* type, int size) {
		return MatchAtomicCategory(type, SCHEMA_ATOMIC_PLAIN, size);
	}

	FieldKind MatchAtomic(const CSchemaType* type, int size) {
		return MatchAtomicCategory(type, SCHEMA_ATOMIC_T, size);
	}

	//
	// Typed access helpers (schema_helpers.hpp)
	//

	void* ResizeUtlVector(intptr_t address, int count, int dataSize) {
		// The layout of CUtlVector (and CNetworkUtlVectorBase) does not depend on T, only the
		// allocation size does, so the element size picks the instantiation.
		void* data = nullptr;
		Dispatch(IntStorage(dataSize), [&]<typename S>() {
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

	ArrayView GetArrayView(intptr_t base, const CSchemaType* type, FieldKind elementType) {
		switch (elementType) {
			case FieldKind::Array: {
				const auto* arrayType = static_cast<const CSchemaType_FixedArray*>(type);
				return {base, arrayType->m_nElementCount, arrayType->m_nElementSize};
			}
			case FieldKind::Collection: {
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

	void* EntityField::Element(FieldKind elementType, int element, std::string_view expected) const {
		switch (elementType) {
			case FieldKind::Single:
				if (element != 0) {
					plg::print(LS_WARNING, "Element {} is out of range for '{}::{}' (size: 1)\n", element, className, memberName);
					return nullptr;
				}
				return reinterpret_cast<void*>(Base());
			case FieldKind::Array:
			case FieldKind::Collection: {
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

	void EntityField::NotifyChanged(bool changeState) const {
		if (!changeState) {
			return;
		}
		if (key.networked) {
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), key.offset, FindChainOffset(className));
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be sent over network\n", className, memberName);
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
