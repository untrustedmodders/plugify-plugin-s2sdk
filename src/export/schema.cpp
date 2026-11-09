#include <core/core_config.hpp>
#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/schema.hpp>
#include <core/sdk/utils.hpp>
#include <schemasystem/schemasystem.h>
#include <tier0/utlstring.h>
#include <tier1/utlsymbollarge.h>
#include <string.h>

#include <algorithm>
#include <optional>
#include <source_location>
#include <string_view>
#include <type_traits>
#include <utility>

PLUGIFY_WARN_PUSH()

PLUGIFY_LINKAGE()

namespace {
	//
	// Entity handles
	//

	// Resolves an entity handle and calls `fn` with the entity, or returns `fallback` if the handle is invalid.
	template<typename R, typename F>
	R WithEntity(int entityHandle, R fallback, F&& fn, const std::source_location& loc = std::source_location::current()) {
		CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
		if (!entity) {
			plg::print(LS_WARNING, "{}: invalid entity handle: {}\n", loc.function_name(), entityHandle);
			return fallback;
		}
		return fn(entity);
	}

	// Same as above, returns a value-initialized result (or nothing) if the handle is invalid.
	template<typename F>
	auto WithEntity(int entityHandle, F&& fn, const std::source_location& loc = std::source_location::current()) -> std::invoke_result_t<F, CEntityInstance*> {
		using R = std::invoke_result_t<F, CEntityInstance*>;
		CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
		if (!entity) {
			plg::print(LS_WARNING, "{}: invalid entity handle: {}\n", loc.function_name(), entityHandle);
			if constexpr (std::is_void_v<R>) {
				return;
			} else {
				return R{};
			}
		}
		return fn(entity);
	}

	//
	// Raw values
	//

	// Calls `fn.template operator()<S>()` with the storage type S matching `dataSize`:
	// int8_t/int16_t/int32_t/int64_t for integer T, float/double for floating-point T.
	template<typename T, typename F>
	bool DispatchSize(int dataSize, F&& fn) {
		if constexpr (std::is_floating_point_v<T>) {
			switch (dataSize) {
				case sizeof(float):
					fn.template operator()<float>();
					return true;
				case sizeof(double):
					fn.template operator()<double>();
					return true;
				default:
					break;
			}
		} else {
			switch (dataSize) {
				case sizeof(int8_t):
					fn.template operator()<int8_t>();
					return true;
				case sizeof(int16_t):
					fn.template operator()<int16_t>();
					return true;
				case sizeof(int32_t):
					fn.template operator()<int32_t>();
					return true;
				case sizeof(int64_t):
					fn.template operator()<int64_t>();
					return true;
				default:
					break;
			}
		}
		plg::print(LS_WARNING, "Invalid {} element size: {}\n", std::is_floating_point_v<T> ? "float" : "integer", dataSize);
		return false;
	}

	template<typename T>
	bool IsValidElementSize(int dataSize) {
		return DispatchSize<T>(dataSize, []<typename S>() {});
	}

	template<typename T>
	T ReadValue(const void* src, int dataSize) {
		T result{};
		DispatchSize<T>(dataSize, [&]<typename S>() {
			result = static_cast<T>(*static_cast<const S*>(src));
		});
		return result;
	}

	template<typename T>
	void WriteValue(void* dst, int dataSize, T value) {
		DispatchSize<T>(dataSize, [&]<typename S>() {
			*static_cast<S*>(dst) = static_cast<S>(value);
		});
	}

	template<typename T>
	plg::vector<T> ReadValues(const void* src, size_t count, int dataSize) {
		plg::vector<T> result;
		DispatchSize<T>(dataSize, [&]<typename S>() {
			const S* p = static_cast<const S*>(src);
			result.resize(count);
			std::transform(p, p + count, result.begin(), [](const S v) { return static_cast<T>(v); });
		});
		return result;
	}

	template<typename T>
	void WriteValues(void* dst, const T* src, size_t count, int dataSize) {
		DispatchSize<T>(dataSize, [&]<typename S>() {
			std::transform(src, src + count, static_cast<S*>(dst), [](const T v) { return static_cast<S>(v); });
		});
	}

	// Resizes a CUtlVector whose elements are `dataSize` bytes and returns its data pointer.
	// The layout of CUtlVector (and CNetworkUtlVectorBase) does not depend on T, only the
	// allocation size does, so the element size picks the instantiation.
	void* ResizeUtlVector(intptr_t address, int count, int dataSize) {
		void* data = nullptr;
		DispatchSize<int64_t>(dataSize, [&]<typename S>() {
			auto* vec = reinterpret_cast<CUtlVector<S>*>(address);
			vec->SetCount(count);
			data = vec->Base();
		});
		return data;
	}

	//
	// Offset-based data
	//

	template<typename T>
	T& DataAt(CEntityInstance* entity, int offset) {
		return *reinterpret_cast<T*>(reinterpret_cast<intptr_t>(entity) + offset);
	}

	void NotifyDataChanged(CEntityInstance* entity, int offset, int chainOffset, bool changeState) {
		if (changeState) {
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		}
	}

	template<typename T>
	void SetData(CEntityInstance* entity, int offset, const T& value, bool changeState, int chainOffset) {
		NotifyDataChanged(entity, offset, chainOffset, changeState);
		DataAt<T>(entity, offset) = value;
	}

	template<typename T>
	plg::vector<T> GetDataCArray(CEntityInstance* entity, int offset, int arraySize, int dataSize) {
		if (arraySize <= 0) {
			return {};
		}
		return ReadValues<T>(&DataAt<uint8>(entity, offset), static_cast<size_t>(arraySize), dataSize);
	}

	template<typename T>
	void SetDataCArray(CEntityInstance* entity, int offset, const plg::vector<T>& value, int arraySize, int dataSize, bool changeState, int chainOffset) {
		if (arraySize <= 0 || !IsValidElementSize<T>(dataSize)) {
			return;
		}
		NotifyDataChanged(entity, offset, chainOffset, changeState);
		const size_t n = std::min(value.size(), static_cast<size_t>(arraySize));
		WriteValues<T>(&DataAt<uint8>(entity, offset), value.data(), n, dataSize);
	}

	template<typename T>
	plg::vector<T> GetDataVector(CEntityInstance* entity, int offset, int dataSize) {
		const auto& vec = DataAt<CUtlVector<uint8>>(entity, offset);
		if (vec.Count() <= 0) {
			return {};
		}
		return ReadValues<T>(vec.Base(), static_cast<size_t>(vec.Count()), dataSize);
	}

	template<typename T>
	void SetDataVector(CEntityInstance* entity, int offset, const plg::vector<T>& value, int dataSize, bool changeState, int chainOffset) {
		if (!IsValidElementSize<T>(dataSize)) {
			return;
		}
		NotifyDataChanged(entity, offset, chainOffset, changeState);
		const int count = static_cast<int>(value.size());
		void* data = ResizeUtlVector(reinterpret_cast<intptr_t>(entity) + offset, count, dataSize);
		if (data && count > 0) {
			WriteValues<T>(data, value.data(), value.size(), dataSize);
		}
	}

	//
	// Schema fields
	//

	// Location, length and stride of the elements of an array-like schema field.
	struct SchemaArrayView {
		intptr_t data{};
		int count{};
		int stride{};
	};

	SchemaArrayView GetSchemaArrayView(intptr_t base, CSchemaType* type, schema::ElementType elementType) {
		switch (elementType) {
			case schema::ElementType::Array: {
				const auto* arrayType = static_cast<const CSchemaType_FixedArray*>(type);
				return {base, arrayType->m_nElementCount, arrayType->m_nElementSize};
			}
			case schema::ElementType::Collection: {
				const auto* vec = reinterpret_cast<CUtlVector<uint8>*>(base);
				const auto* collectionType = static_cast<const CSchemaType_Atomic_CollectionOfT*>(type);
				return {reinterpret_cast<intptr_t>(vec->Base()), vec->Count(), collectionType->m_nElementSize};
			}
			default:
				return {};
		}
	}

	// A resolved schema member of a concrete entity.
	struct SchemaField {
		CEntityInstance* entity;
		const plg::string& className;
		const plg::string& memberName;
		SchemaKey key;

		intptr_t Base() const {
			return reinterpret_cast<intptr_t>(entity) + key.offset;
		}

		void WarnType(std::string_view expected) const {
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a {}, but '{}'\n", className, memberName, expected, key.type->m_sTypeName.Get());
		}

		// Address of `element` for arrays and collections, or of the field itself for single values.
		// Warns and returns nullptr on a type mismatch or when the element is out of range.
		void* Element(schema::ElementType elementType, int element, std::string_view expected) const {
			switch (elementType) {
				case schema::ElementType::Single:
					return reinterpret_cast<void*>(Base());
				case schema::ElementType::Array:
				case schema::ElementType::Collection: {
					const auto [data, count, stride] = GetSchemaArrayView(Base(), key.type, elementType);
					if (element < 0 || element >= count) {
						plg::print(LS_WARNING, "Element {} is out of range for '{}::{}' (size: {})\n", element, className, memberName, count);
						return nullptr;
					}
					return reinterpret_cast<void*>(data + static_cast<intptr_t>(element) * stride);
				}
				default:
					WarnType(expected);
					return nullptr;
			}
		}

		// Element address plus its size in bytes, for fields classified by IsIntType/IsFloatType.
		std::pair<void*, int> SizedElement(schema::ElementSizedType sized, int element, std::string_view expected) const {
			void* ptr = Element(sized.type, element, expected);
			const int size = sized.type == schema::ElementType::Single ? static_cast<int>(key.size) : sized.size;
			return {ptr, size};
		}

		void NotifyChanged(bool changeState) const {
			if (!changeState) {
				return;
			}
			if (key.networked) {
				SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), key.offset, schema::FindChainOffset(className));
			} else {
				plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
			}
		}
	};

	std::optional<SchemaField> FindSchemaField(CEntityInstance* entity, const plg::string& className, const plg::string& memberName) {
		const SchemaKey key = schema::GetOffset(className, memberName);
		if (key.offset == -1) {
			plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
			return std::nullopt;
		}
		return SchemaField{entity, className, memberName, key};
	}

	// Same as FindSchemaField, but also refuses members blocked by the "FollowCS2ServerGuidelines" option.
	std::optional<SchemaField> FindWritableSchemaField(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, std::string_view action = "set") {
		if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
			plg::print(LS_WARNING, "Cannot {} '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", action, className, memberName);
			return std::nullopt;
		}
		return FindSchemaField(entity, className, memberName);
	}

	// Integer (int64_t) or floating-point (double) schema values.
	template<typename T>
	schema::ElementSizedType ClassifyNumeric(CSchemaType* type) {
		if constexpr (std::is_floating_point_v<T>) {
			return schema::IsFloatType(type);
		} else {
			return schema::IsIntType(type);
		}
	}

	template<typename T>
	constexpr std::string_view numericName = std::is_floating_point_v<T> ? "float" : "integer";

	template<typename T>
	T GetSchemaNumeric(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
		const auto field = FindSchemaField(entity, className, memberName);
		if (!field) {
			return {};
		}

		const auto sized = ClassifyNumeric<T>(field->key.type);
		if constexpr (std::is_integral_v<T>) {
			// Structures larger than 8 bytes are returned by address.
			if (sized.type == schema::ElementType::Class) {
				return static_cast<T>(field->Base());
			}
		}

		const auto [ptr, size] = field->SizedElement(sized, element, numericName<T>);
		return ptr ? ReadValue<T>(ptr, size) : T{};
	}

	template<typename T>
	void SetSchemaNumeric(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, T value, bool changeState, int element) {
		const auto field = FindWritableSchemaField(entity, className, memberName);
		if (!field) {
			return;
		}

		const auto [ptr, size] = field->SizedElement(ClassifyNumeric<T>(field->key.type), element, numericName<T>);
		if (!ptr || !IsValidElementSize<T>(size)) {
			return;
		}

		field->NotifyChanged(changeState);
		WriteValue<T>(ptr, size, value);
	}

	// Values stored as-is (vectors), matched by their size.
	template<typename T, size_t Size = sizeof(T)>
	T GetSchemaPlain(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element, std::string_view expected) {
		const auto field = FindSchemaField(entity, className, memberName);
		if (!field) {
			return {};
		}

		const auto* ptr = static_cast<const T*>(field->Element(schema::IsPlainType(field->key.type, Size), element, expected));
		return ptr ? *ptr : T{};
	}

	template<typename T, size_t Size = sizeof(T)>
	void SetSchemaPlain(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const T& value, bool changeState, int element, std::string_view expected) {
		const auto field = FindWritableSchemaField(entity, className, memberName);
		if (!field) {
			return;
		}

		auto* ptr = static_cast<T*>(field->Element(schema::IsPlainType(field->key.type, Size), element, expected));
		if (!ptr) {
			return;
		}

		field->NotifyChanged(changeState);
		*ptr = value;
	}

	template<typename T>
	plg::vector<T> GetSchemaArray(CEntityInstance* entity, const plg::string& className, const plg::string& memberName) {
		const auto field = FindSchemaField(entity, className, memberName);
		if (!field) {
			return {};
		}

		const auto [elementType, elementSize] = ClassifyNumeric<T>(field->key.type);
		switch (elementType) {
			case schema::ElementType::Array:
			case schema::ElementType::Collection: {
				const auto [data, count, stride] = GetSchemaArrayView(field->Base(), field->key.type, elementType);
				if (count <= 0) {
					return {};
				}
				return ReadValues<T>(reinterpret_cast<const void*>(data), static_cast<size_t>(count), elementSize);
			}
			case schema::ElementType::Single:
				plg::print(LS_WARNING, "Schema field '{}::{}' is not an array\n", className, memberName);
				return {};
			default:
				field->WarnType(std::is_floating_point_v<T> ? "float array" : "integer array");
				return {};
		}
	}

	template<typename T>
	void SetSchemaArray(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const plg::vector<T>& value, bool changeState) {
		const auto field = FindWritableSchemaField(entity, className, memberName);
		if (!field) {
			return;
		}

		const auto [elementType, elementSize] = ClassifyNumeric<T>(field->key.type);
		switch (elementType) {
			case schema::ElementType::Array: {
				if (!IsValidElementSize<T>(elementSize)) {
					return;
				}
				const int count = static_cast<const CSchemaType_FixedArray*>(field->key.type)->m_nElementCount;
				if (value.size() > static_cast<size_t>(count)) {
					plg::print(LS_WARNING, "Schema array '{}::{}' can hold {} elements, {} were given, extra elements are ignored\n", className, memberName, count, value.size());
				}
				field->NotifyChanged(changeState);
				const size_t n = std::min(value.size(), static_cast<size_t>(count));
				WriteValues<T>(reinterpret_cast<void*>(field->Base()), value.data(), n, elementSize);
				break;
			}
			case schema::ElementType::Collection: {
				if (!IsValidElementSize<T>(elementSize)) {
					return;
				}
				field->NotifyChanged(changeState);
				const int count = static_cast<int>(value.size());
				void* data = ResizeUtlVector(field->Base(), count, elementSize);
				if (data && count > 0) {
					WriteValues<T>(data, value.data(), value.size(), elementSize);
				}
				break;
			}
			case schema::ElementType::Single:
				plg::print(LS_WARNING, "Schema field '{}::{}' is not an array\n", className, memberName);
				break;
			default:
				field->WarnType(std::is_floating_point_v<T> ? "float array" : "integer array");
				break;
		}
	}
} // namespace

/**
 * @brief Get the offset of a member in a given schema class.
 *
 * This function retrieves the offset of a member in the specified class.
 *
 * @param className The name of the class.
 * @param memberName The name of the member whose offset is to be retrieved.
 * @return The offset of the member in the class, or -1 if the offset is not found.
 */
extern "C" PLUGIN_API int32_t GetSchemaOffset(const plg::string& className, const plg::string& memberName) {
	return schema::GetOffset(className, memberName).offset;
}

/**
 * @brief Get the offset of a chain in a given schema class.
 *
 * This function retrieves the offset of a chain in the specified class.
 *
 * @param className The name of the class.
 * @return The offset of the chain entity in the class, or -1 if the offset is not found.
 */
extern "C" PLUGIN_API int32_t GetSchemaChainOffset(const plg::string& className) {
	return schema::FindChainOffset(className);
}

/**
 * @brief Check if a schema field is networked.
 *
 * This function checks whether the specified member of the given class is networked.
 *
 * @param className The name of the class.
 * @param memberName The name of the member to check.
 * @return True if the member is networked, false otherwise.
 */
extern "C" PLUGIN_API bool IsSchemaFieldNetworked(const plg::string& className, const plg::string& memberName) {
	return schema::GetOffset(className, memberName).networked;
}

/**
 * @brief Get the size of a schema class.
 *
 * This function retrieves the size of the specified class in bytes.
 *
 * @param className The name of the class.
 * @return The size of the class in bytes, or -1 if the class is not found.
 */
extern "C" PLUGIN_API int32_t GetSchemaClassSize(const plg::string& className) {
	auto* scope = g_pSchemaSystem->FindTypeScopeForModule(schema::moduleName.data());
	if (!scope) {
		plg::print(LS_WARNING, "'{}' module was not found!\n", schema::moduleName);
		return {};
	}
	auto cls = scope->FindDeclaredClass(className.c_str());
	if (!cls) {
		plg::print(LS_WARNING, "'{}' class was not found!\n", className);
		return -1;
	}
	return cls->m_nSize;
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the integer value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param size Number of bytes to read (valid values are 1, 2, 4 or 8).
 * @return The integer value at the given memory location.
 */
extern "C" PLUGIN_API int64_t GetEntData2(CEntityInstance* entity, int offset, int size) {
	return ReadValue<int64_t>(&DataAt<uint8>(entity, offset), size);
}

/**
 * @brief Peeks into an entity's object data and sets the integer value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The integer value to set.
 * @param size Number of bytes to write (valid values are 1, 2, 4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntData2(CEntityInstance* entity, int offset, int64_t value, int size, bool changeState, int chainOffset) {
	NotifyDataChanged(entity, offset, chainOffset, changeState);
	WriteValue<int64_t>(&DataAt<uint8>(entity, offset), size, value);
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the float value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param size Number of bytes to read (valid values are 1, 2, 4 or 8).
 * @return The float value at the given memory location.
 */
extern "C" PLUGIN_API double GetEntDataFloat2(CEntityInstance* entity, int offset, int size) {
	return ReadValue<double>(&DataAt<uint8>(entity, offset), size);
}

/**
 * @brief Peeks into an entity's object data and sets the float value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The float value to set.
 * @param size Number of bytes to write (valid values are 1, 2, 4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataFloat2(CEntityInstance* entity, int offset, double value, int size, bool changeState, int chainOffset) {
	NotifyDataChanged(entity, offset, chainOffset, changeState);
	WriteValue<double>(&DataAt<uint8>(entity, offset), size, value);
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the color value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @return The color value at the given memory location.
 */
extern "C" PLUGIN_API plg::vec4 GetEntDataColor2(CEntityInstance* entity, int offset) {
	return std::bit_cast<plg::vec4>(DataAt<Color>(entity, offset).ToVector4D());
}

/**
 * @brief Peeks into an entity's object data and sets the color value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The color value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataColor2(CEntityInstance* entity, int offset, const plg::vec4& value, bool changeState, int chainOffset) {
	SetData(entity, offset, NewColor(value), changeState, chainOffset);
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the string value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @return The string value at the given memory location.
 */
extern "C" PLUGIN_API plg::string GetEntDataString2(CEntityInstance* entity, int offset) {
	return DataAt<CUtlString>(entity, offset).Get();
}

/**
 * @brief Peeks into an entity's object data and sets the string at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The string value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataString2(CEntityInstance* entity, int offset, const plg::string& value, bool changeState, int chainOffset) {
	NotifyDataChanged(entity, offset, chainOffset, changeState);
	DataAt<CUtlString>(entity, offset) = value;
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the string value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param size Number of bytes to read.
 * @return The string value at the given memory location.
 */
extern "C" PLUGIN_API plg::string GetEntDataCString2(CEntityInstance* entity, int offset, int size) {
	const char* src = &DataAt<char>(entity, offset);
	return plg::string(src, strnlen(src, size));
}

/**
 * @brief Peeks into an entity's object data and sets the string at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The string value to set.
 * @param size Number of bytes to write. If size -1 it's CUtlSymbolLarge.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataCString2(CEntityInstance* entity, int offset, const plg::string& value, int size, bool changeState, int chainOffset) {
	NotifyDataChanged(entity, offset, chainOffset, changeState);

	if (size == -1) {
		DataAt<CUtlSymbolLarge>(entity, offset) = g_pGameEntitySystem->AllocPooledString(value.c_str());
		return;
	}

	char* dst = &DataAt<char>(entity, offset);
	const size_t n = std::min(value.size(), static_cast<size_t>(size - 1));
	value.copy(dst, n);
	dst[n] = '\0';
}

//

/**
 * @brief Peeks into an entity's object data and retrieves the fixed-size integer array at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param arraySize Number of elements in the array.
 * @param dataSize Size of each element in bytes (1, 2, 4 or 8).
 * @return The array values at the given memory location.
 */
extern "C" PLUGIN_API plg::vector<int64_t> GetEntDataCArray2(CEntityInstance* entity, int offset, int arraySize, int dataSize) {
	return GetDataCArray<int64_t>(entity, offset, arraySize, dataSize);
}

/**
 * @brief Peeks into an entity's object data and sets the fixed-size integer array at the given offset.
 *
 * Writes min(value.size(), arraySize) elements, the rest of the array is left untouched.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The array values to set.
 * @param arraySize Number of elements in the array.
 * @param dataSize Size of each element in bytes (1, 2, 4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataCArray2(CEntityInstance* entity, int offset, const plg::vector<int64_t>& value, int arraySize, int dataSize, bool changeState, int chainOffset) {
	SetDataCArray(entity, offset, value, arraySize, dataSize, changeState, chainOffset);
}

//

/**
 * @brief Peeks into an entity's object data and retrieves the fixed-size float array at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param arraySize Number of elements in the array.
 * @param dataSize Size of each element in bytes (4 or 8).
 * @return The array values at the given memory location.
 */
extern "C" PLUGIN_API plg::vector<double> GetEntDataCArrayFloat2(CEntityInstance* entity, int offset, int arraySize, int dataSize) {
	return GetDataCArray<double>(entity, offset, arraySize, dataSize);
}

/**
 * @brief Peeks into an entity's object data and sets the fixed-size float array at the given offset.
 *
 * Writes min(value.size(), arraySize) elements, the rest of the array is left untouched.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The array values to set.
 * @param arraySize Number of elements in the array.
 * @param dataSize Size of each element in bytes (4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataCArrayFloat2(CEntityInstance* entity, int offset, const plg::vector<double>& value, int arraySize, int dataSize, bool changeState, int chainOffset) {
	SetDataCArray(entity, offset, value, arraySize, dataSize, changeState, chainOffset);
}

//

/**
 * @brief Peeks into an entity's object data and retrieves the integer vector (CUtlVector) at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param dataSize Size of each element in bytes (1, 2, 4 or 8).
 * @return The vector elements at the given memory location.
 */
extern "C" PLUGIN_API plg::vector<int64_t> GetEntDataArray2(CEntityInstance* entity, int offset, int dataSize) {
	return GetDataVector<int64_t>(entity, offset, dataSize);
}

/**
 * @brief Peeks into an entity's object data and replaces the integer vector (CUtlVector) at the given offset.
 *
 * The vector is resized to value.size() and filled with the given values.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The vector elements to set.
 * @param dataSize Size of each element in bytes (1, 2, 4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataArray2(CEntityInstance* entity, int offset, const plg::vector<int64_t>& value, int dataSize, bool changeState, int chainOffset) {
	SetDataVector(entity, offset, value, dataSize, changeState, chainOffset);
}

//

/**
 * @brief Peeks into an entity's object data and retrieves the float vector (CUtlVector) at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param dataSize Size of each element in bytes (4 or 8).
 * @return The vector elements at the given memory location.
 */
extern "C" PLUGIN_API plg::vector<double> GetEntDataArrayFloat2(CEntityInstance* entity, int offset, int dataSize) {
	return GetDataVector<double>(entity, offset, dataSize);
}

/**
 * @brief Peeks into an entity's object data and replaces the float vector (CUtlVector) at the given offset.
 *
 * The vector is resized to value.size() and filled with the given values.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The vector elements to set.
 * @param dataSize Size of each element in bytes (4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataArrayFloat2(CEntityInstance* entity, int offset, const plg::vector<double>& value, int dataSize, bool changeState, int chainOffset) {
	SetDataVector(entity, offset, value, dataSize, changeState, chainOffset);
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the vector value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @return The vector value at the given memory location.
 */
extern "C" PLUGIN_API plg::vec3 GetEntDataVector3D2(CEntityInstance* entity, int offset) {
	return DataAt<plg::vec3>(entity, offset);
}

/**
 * @brief Peeks into an entity's object data and sets the vector at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataVector3D2(CEntityInstance* entity, int offset, const plg::vec3& value, bool changeState, int chainOffset) {
	SetData(entity, offset, value, changeState, chainOffset);
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the vector value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @return The vector value at the given memory location.
 */
extern "C" PLUGIN_API plg::vec4 GetEntDataVector4D2(CEntityInstance* entity, int offset) {
	return DataAt<plg::vec4>(entity, offset);
}

/**
 * @brief Peeks into an entity's object data and sets the vector at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataVector4D2(CEntityInstance* entity, int offset, const plg::vec4& value, bool changeState, int chainOffset) {
	SetData(entity, offset, value, changeState, chainOffset);
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the vector value at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @return The vector value at the given memory location.
 */
extern "C" PLUGIN_API plg::vec2 GetEntDataVector2D2(CEntityInstance* entity, int offset) {
	return DataAt<plg::vec2>(entity, offset);
}

/**
 * @brief Peeks into an entity's object data and sets the vector at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataVector2D2(CEntityInstance* entity, int offset, const plg::vec2& value, bool changeState, int chainOffset) {
	SetData(entity, offset, value, changeState, chainOffset);
}

//

/**
 * @brief Peeks into an entity's object data and retrieves the entity handle at the given offset.
 *
 * @note This will only work on offsets that are stored as "entity handles" (which usually looks like m_h* in properties)
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @return The entity handle at the given memory location.
 */
extern "C" PLUGIN_API int GetEntDataEnt2(CEntityInstance* entity, int offset) {
	return DataAt<CEntityHandle>(entity, offset).ToInt();
}

/**
 * @brief Peeks into an entity's object data and sets the entity handle at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The entity handle to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataEnt2(CEntityInstance* entity, int offset, int value, bool changeState, int chainOffset) {
	SetData(entity, offset, CEntityHandle(value), changeState, chainOffset);
}

/**
 * @brief Peeks into an entity's object data and retrieves a pointer to the data at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return A pointer to the data at the given memory location.
 */
extern "C" PLUGIN_API void* GetEntDataPtr2(CEntityInstance* entity, int offset) {
	return &DataAt<uint8>(entity, offset);
}

/**
 * @brief Updates the networked state of a schema field for a given entity pointer.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param offset The offset of the schema to use.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void ChangeEntityState2(CEntityInstance* entity, int offset, int chainOffset) {
	SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
}

//


/**
 * @brief Peeks into an entity's object schema and retrieves the integer value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param size Number of bytes to read (valid values are 1, 2, 4 or 8).
 * @return The integer value at the given memory location.
 */
extern "C" PLUGIN_API int64_t GetEntData(int entityHandle, int offset, int size) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntData2(entity, offset, size); });
}

/**
 * @brief Peeks into an entity's object data and sets the integer value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The integer value to set.
 * @param size Number of bytes to write (valid values are 1, 2, 4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntData(int entityHandle, int offset, int64_t value, int size, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntData2(entity, offset, value, size, changeState, chainOffset); });
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the float value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param size Number of bytes to read (valid values are 1, 2, 4 or 8).
 * @return The float value at the given memory location.
 */
extern "C" PLUGIN_API double GetEntDataFloat(int entityHandle, int offset, int size) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataFloat2(entity, offset, size); });
}

/**
 * @brief Peeks into an entity's object data and sets the float value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The float value to set.
 * @param size Number of bytes to write (valid values are 1, 2, 4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataFloat(int entityHandle, int offset, double value, int size, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataFloat2(entity, offset, value, size, changeState, chainOffset); });
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the color value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return The color value at the given memory location.
 */
extern "C" PLUGIN_API plg::vec4 GetEntDataColor(int entityHandle, int offset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataColor2(entity, offset); });
}

/**
 * @brief Peeks into an entity's object data and sets the color at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The color value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataColor(int entityHandle, int offset, const plg::vec4& value, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataColor2(entity, offset, value, changeState, chainOffset); });
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the string value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return The string value at the given memory location.
 */
extern "C" PLUGIN_API plg::string GetEntDataString(int entityHandle, int offset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataString2(entity, offset); });
}

/**
 * @brief Peeks into an entity's object data and sets the string at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The string value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataString(int entityHandle, int offset, const plg::string& value, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataString2(entity, offset, value, changeState, chainOffset); });
}

/**
 * @brief Peeks into an entity's object schema and retrieves the string value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param size Number of bytes to read.
 * @return The string value at the given memory location.
 */
extern "C" PLUGIN_API plg::string GetEntDataCString(int entityHandle, int offset, int size) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataCString2(entity, offset, size); });
}

/**
 * @brief Peeks into an entity's object data and sets the string at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The string value to set.
 * @param size Number of bytes to write.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataCString(int entityHandle, int offset, const plg::string& value, int size, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataCString2(entity, offset, value, size, changeState, chainOffset); });
}

//

/**
 * @brief Peeks into an entity's object data and retrieves the fixed-size integer array at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param arraySize Number of elements in the array.
 * @param dataSize Size of each element in bytes (1, 2, 4 or 8).
 * @return The array values at the given memory location.
 */
extern "C" PLUGIN_API plg::vector<int64_t> GetEntDataCArray(int entityHandle, int offset, int arraySize, int dataSize) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataCArray2(entity, offset, arraySize, dataSize); });
}

/**
 * @brief Peeks into an entity's object data and sets the fixed-size integer array at the given offset.
 *
 * Writes min(value.size(), arraySize) elements, the rest of the array is left untouched.
 *
 * @param entityHandle The handle of the entity from which the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The array values to set.
 * @param arraySize Number of elements in the array.
 * @param dataSize Size of each element in bytes (1, 2, 4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataCArray(int entityHandle, int offset, const plg::vector<int64_t>& value, int arraySize, int dataSize, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataCArray2(entity, offset, value, arraySize, dataSize, changeState, chainOffset); });
}

/**
 * @brief Peeks into an entity's object data and retrieves the fixed-size float array at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param arraySize Number of elements in the array.
 * @param dataSize Size of each element in bytes (4 or 8).
 * @return The array values at the given memory location.
 */
extern "C" PLUGIN_API plg::vector<double> GetEntDataCArrayFloat(int entityHandle, int offset, int arraySize, int dataSize) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataCArrayFloat2(entity, offset, arraySize, dataSize); });
}

/**
 * @brief Peeks into an entity's object data and sets the fixed-size float array at the given offset.
 *
 * Writes min(value.size(), arraySize) elements, the rest of the array is left untouched.
 *
 * @param entityHandle The handle of the entity from which the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The array values to set.
 * @param arraySize Number of elements in the array.
 * @param dataSize Size of each element in bytes (4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataCArrayFloat(int entityHandle, int offset, const plg::vector<double>& value, int arraySize, int dataSize, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataCArrayFloat2(entity, offset, value, arraySize, dataSize, changeState, chainOffset); });
}

/**
 * @brief Peeks into an entity's object data and retrieves the integer vector (CUtlVector) at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param dataSize Size of each element in bytes (1, 2, 4 or 8).
 * @return The vector elements at the given memory location.
 */
extern "C" PLUGIN_API plg::vector<int64_t> GetEntDataArray(int entityHandle, int offset, int dataSize) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataArray2(entity, offset, dataSize); });
}

/**
 * @brief Peeks into an entity's object data and replaces the integer vector (CUtlVector) at the given offset.
 *
 * The vector is resized to value.size() and filled with the given values.
 *
 * @param entityHandle The handle of the entity from which the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The vector elements to set.
 * @param dataSize Size of each element in bytes (1, 2, 4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataArray(int entityHandle, int offset, const plg::vector<int64_t>& value, int dataSize, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataArray2(entity, offset, value, dataSize, changeState, chainOffset); });
}

/**
 * @brief Peeks into an entity's object data and retrieves the float vector (CUtlVector) at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param dataSize Size of each element in bytes (4 or 8).
 * @return The vector elements at the given memory location.
 */
extern "C" PLUGIN_API plg::vector<double> GetEntDataArrayFloat(int entityHandle, int offset, int dataSize) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataArrayFloat2(entity, offset, dataSize); });
}

/**
 * @brief Peeks into an entity's object data and replaces the float vector (CUtlVector) at the given offset.
 *
 * The vector is resized to value.size() and filled with the given values.
 *
 * @param entityHandle The handle of the entity from which the value is to be set.
 * @param offset The offset of the schema to use.
 * @param value The vector elements to set.
 * @param dataSize Size of each element in bytes (4 or 8).
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataArrayFloat(int entityHandle, int offset, const plg::vector<double>& value, int dataSize, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataArrayFloat2(entity, offset, value, dataSize, changeState, chainOffset); });
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the vector value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return The vector value at the given memory location.
 */
extern "C" PLUGIN_API plg::vec3 GetEntDataVector3D(int entityHandle, int offset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataVector3D2(entity, offset); });
}

/**
 * @brief Peeks into an entity's object data and sets the vector at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataVector3D(int entityHandle, int offset, const plg::vec3& value, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataVector3D2(entity, offset, value, changeState, chainOffset); });
}

//

/**
 * @brief Peeks into an entity's object schema and retrieves the vector value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return The vector value at the given memory location.
 */
extern "C" PLUGIN_API plg::vec4 GetEntDataVector4D(int entityHandle, int offset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataVector4D2(entity, offset); });
}

/**
 * @brief Peeks into an entity's object data and sets the vector at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataVector4D(int entityHandle, int offset, const plg::vec4& value, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataVector4D2(entity, offset, value, changeState, chainOffset); });
}

//

//

/**
 * @brief Peeks into an entity's object schema and retrieves the vector value at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return The vector value at the given memory location.
 */
extern "C" PLUGIN_API plg::vec2 GetEntDataVector2D(int entityHandle, int offset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataVector2D2(entity, offset); });
}

/**
 * @brief Peeks into an entity's object data and sets the vector at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataVector2D(int entityHandle, int offset, const plg::vec2& value, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataVector2D2(entity, offset, value, changeState, chainOffset); });
}

//

/**
 * @brief Peeks into an entity's object data and retrieves the entity handle at the given offset.
 *
 * @note This will only work on offsets that are stored as "entity handles" (which usually looks like m_h* in properties)
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return The entity handle at the given memory location.
 */
extern "C" PLUGIN_API int GetEntDataEnt(int entityHandle, int offset) {
	return WithEntity(entityHandle, static_cast<int>(INVALID_EHANDLE_INDEX), [&](CEntityInstance* entity) { return GetEntDataEnt2(entity, offset); });
}

/**
 * @brief Peeks into an entity's object data and sets the entity handle at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param value The entity handle to set.
 * @param changeState If true, change will be sent over the network.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void SetEntDataEnt(int entityHandle, int offset, int value, bool changeState, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataEnt2(entity, offset, value, changeState, chainOffset); });
}

/**
 * @brief Peeks into an entity's object data and retrieves a pointer to the data at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return A pointer to the data at the given memory location.
 */
extern "C" PLUGIN_API void* GetEntDataPtr(int entityHandle, int offset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataPtr2(entity, offset); });
}

/**
 * @brief Updates the networked state of a schema field for a given entity handle.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void ChangeEntityState(int entityHandle, int offset, int chainOffset) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return ChangeEntityState2(entity, offset, chainOffset); });
}

//

/**
 * @brief Retrieves the count of values that an entity schema's array can store.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @return Size of array (in elements) or 0 if schema is not an array.
 */
extern "C" PLUGIN_API int GetEntSchemaArraySize2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName) {
	const auto field = FindSchemaField(entity, className, memberName);
	if (!field) {
		return {};
	}

	return GetSchemaArrayView(field->Base(), field->key.type, schema::GetElementType(field->key.type)).count;
}

/**
 * @brief Retrieves a pointer to a structure or an array element in an entity's schema.
 *
 * Works for fixed arrays and collections (CUtlVector, CNetworkUtlVectorBase, etc.) of any element type,
 * including structures. For non-array fields returns a pointer to the field itself (element must be 0).
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A pointer to the field or element, or nullptr if the field is not found or element is out of range.
 */
extern "C" PLUGIN_API void* GetEntSchemaPtr2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	const auto field = FindSchemaField(entity, className, memberName);
	if (!field) {
		return nullptr;
	}

	const auto elementType = schema::GetElementType(field->key.type);
	if (elementType == schema::ElementType::Single && element != 0) {
		plg::print(LS_WARNING, "Element {} is out of range for '{}::{}' (size: 1)\n", element, className, memberName);
		return nullptr;
	}

	return field->Element(elementType, element, "supported type");
}

//

/**
 * @brief Retrieves an integer value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return An integer value at the given schema offset.
 */
extern "C" PLUGIN_API int64_t GetEntSchema2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	return GetSchemaNumeric<int64_t>(entity, className, memberName, element);
}

/**
 * @brief Sets an integer value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The integer value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return An integer value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchema2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int64_t value, bool changeState, int element) {
	SetSchemaNumeric<int64_t>(entity, className, memberName, value, changeState, element);
}

//

/**
 * @brief Retrieves a float value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A float value at the given schema offset.
 */
extern "C" PLUGIN_API double GetEntSchemaFloat2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	return GetSchemaNumeric<double>(entity, className, memberName, element);
}

/**
 * @brief Sets a float value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The float value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A float value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaFloat2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, double value, bool changeState, int element) {
	SetSchemaNumeric<double>(entity, className, memberName, value, changeState, element);
}

//

/**
 * @brief Retrieves all elements of an integer array from an entity's schema.
 *
 * Works for fixed arrays and collections (CUtlVector, CNetworkUtlVectorBase, etc.) of integer-like elements.
 * This function is considered safer and more robust over GetEntDataArray, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @return The array elements, or an empty array if the field is not found or is not an integer array.
 */
extern "C" PLUGIN_API plg::vector<int64_t> GetEntSchemaArray2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName) {
	return GetSchemaArray<int64_t>(entity, className, memberName);
}

/**
 * @brief Sets all elements of an integer array in an entity's schema.
 *
 * For fixed arrays writes min(value.size(), array size) elements and leaves the rest untouched.
 * For collections (CUtlVector, CNetworkUtlVectorBase, etc.) resizes the collection to value.size().
 * This function is considered safer and more robust over SetEntDataArray, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The array elements to set.
 * @param changeState If true, change will be sent over the network.
 */
extern "C" PLUGIN_API void SetEntSchemaArray2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const plg::vector<int64_t>& value, bool changeState) {
	SetSchemaArray(entity, className, memberName, value, changeState);
}

//

//

/**
 * @brief Retrieves all elements of a float array from an entity's schema.
 *
 * Works for fixed arrays and collections (CUtlVector, CNetworkUtlVectorBase, etc.) of float elements.
 * This function is considered safer and more robust over GetEntDataArrayFloat, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @return The array elements, or an empty array if the field is not found or is not a float array.
 */
extern "C" PLUGIN_API plg::vector<double> GetEntSchemaArrayFloat2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName) {
	return GetSchemaArray<double>(entity, className, memberName);
}

/**
 * @brief Sets all elements of a float array in an entity's schema.
 *
 * For fixed arrays writes min(value.size(), array size) elements and leaves the rest untouched.
 * For collections (CUtlVector, CNetworkUtlVectorBase, etc.) resizes the collection to value.size().
 * This function is considered safer and more robust over SetEntDataArrayFloat, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The array elements to set.
 * @param changeState If true, change will be sent over the network.
 */
extern "C" PLUGIN_API void SetEntSchemaArrayFloat2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const plg::vector<double>& value, bool changeState) {
	SetSchemaArray(entity, className, memberName, value, changeState);
}

//

/**
 * @brief Retrieves a color value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A float value at the given schema offset.
 */
extern "C" PLUGIN_API plg::vec4 GetEntSchemaColor2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	const auto field = FindSchemaField(entity, className, memberName);
	if (!field) {
		return {};
	}

	const auto [ptr, size] = field->SizedElement(schema::IsIntType(field->key.type), element, "color");
	if (!ptr || size != static_cast<int>(sizeof(Color))) {
		return {};
	}

	return std::bit_cast<plg::vec4>(static_cast<const Color*>(ptr)->ToVector4D());
}

/**
 * @brief Sets a float value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The float value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A color value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaColor2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const plg::vec4& value, bool changeState, int element) {
	const auto field = FindWritableSchemaField(entity, className, memberName);
	if (!field) {
		return;
	}

	const auto [ptr, size] = field->SizedElement(schema::IsIntType(field->key.type), element, "color");
	if (!ptr || size != static_cast<int>(sizeof(Color))) {
		return;
	}

	field->NotifyChanged(changeState);
	*static_cast<Color*>(ptr) = NewColor(value);
}

//

/**
 * @brief Retrieves a string value from an entity's schema.
*
* This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A string value at the given schema offset.
 */
extern "C" PLUGIN_API plg::string GetEntSchemaString2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	const auto field = FindSchemaField(entity, className, memberName);
	if (!field) {
		return {};
	}

	// CUtlString fields
	if (const auto stringType = schema::IsPlainType(field->key.type, sizeof(CUtlString)); stringType != schema::ElementType::Invalid) {
		const auto* ptr = static_cast<const CUtlString*>(field->Element(stringType, element, "string"));
		return ptr ? plg::string(ptr->Get()) : plg::string();
	}

	// char[N], CUtlVector<char> and char fields
	const auto [elementType, elementSize] = schema::IsIntType(field->key.type);
	switch (elementType) {
		case schema::ElementType::Array:
			if (elementSize == sizeof(char)) {
				const char* src = reinterpret_cast<const char*>(field->Base());
				return plg::string(src, strnlen(src, field->key.size));
			}
			break;
		case schema::ElementType::Collection:
			if (elementSize == sizeof(char)) {
				const auto& vec = *reinterpret_cast<const CUtlVector<char>*>(field->Base());
				return plg::string(vec.Base(), static_cast<size_t>(vec.Count()));
			}
			break;
		case schema::ElementType::Single:
			if (field->key.size == sizeof(char)) {
				return plg::string(1, *reinterpret_cast<const char*>(field->Base()));
			}
			break;
		default:
			break;
	}

	field->WarnType("string");
	return {};
}

/**
 * @brief Sets a string value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The string value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return An string value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaString2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const plg::string& value, bool changeState, int element) {
	const auto field = FindWritableSchemaField(entity, className, memberName);
	if (!field) {
		return;
	}

	// CUtlString fields
	if (const auto stringType = schema::IsPlainType(field->key.type, sizeof(CUtlString)); stringType != schema::ElementType::Invalid) {
		auto* ptr = static_cast<CUtlString*>(field->Element(stringType, element, "string"));
		if (ptr) {
			field->NotifyChanged(changeState);
			*ptr = value;
		}
		return;
	}

	// char[N], CUtlVector<char> and char fields
	const auto [elementType, elementSize] = schema::IsIntType(field->key.type);
	switch (elementType) {
		case schema::ElementType::Array:
			if (elementSize == sizeof(char)) {
				field->NotifyChanged(changeState);
				char* dst = reinterpret_cast<char*>(field->Base());
				const size_t n = std::min(value.size(), field->key.size - 1);
				value.copy(dst, n);
				dst[n] = '\0';
				return;
			}
			break;
		case schema::ElementType::Collection:
			if (elementSize == sizeof(char)) {
				field->NotifyChanged(changeState);
				reinterpret_cast<CUtlVector<char>*>(field->Base())->CopyArray(value.data(), static_cast<CUtlVector<char>::IndexType_t>(value.size()));
				return;
			}
			break;
		case schema::ElementType::Single:
			if (field->key.size == sizeof(char)) {
				field->NotifyChanged(changeState);
				*reinterpret_cast<char*>(field->Base()) = value.empty() ? 0 : value.front();
				return;
			}
			break;
		default:
			break;
	}

	field->WarnType("string");
}

//

/**
 * @brief Retrieves a vector value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API plg::vec3 GetEntSchemaVector3D2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	return GetSchemaPlain<plg::vec3, sizeof(Vector)>(entity, className, memberName, element, "vector");
}

/**
 * @brief Sets a vector value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaVector3D2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const plg::vec3& value, bool changeState, int element) {
	SetSchemaPlain<plg::vec3, sizeof(Vector)>(entity, className, memberName, value, changeState, element, "vector");
}

//

/**
 * @brief Retrieves a vector value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API plg::vec2 GetEntSchemaVector2D2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	return GetSchemaPlain<plg::vec2, sizeof(Vector2D)>(entity, className, memberName, element, "vector");
}

//

/**
 * @brief Sets a vector value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaVector2D2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const plg::vec2& value, bool changeState, int element) {
	SetSchemaPlain<plg::vec2, sizeof(Vector2D)>(entity, className, memberName, value, changeState, element, "vector");
}

//

/**
 * @brief Retrieves a vector value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API plg::vec4 GetEntSchemaVector4D2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	return GetSchemaPlain<plg::vec4, sizeof(Vector4D)>(entity, className, memberName, element, "vector");
}

/**
 * @brief Sets a vector value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaVector4D2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, const plg::vec4& value, bool changeState, int element) {
	SetSchemaPlain<plg::vec4, sizeof(Vector4D)>(entity, className, memberName, value, changeState, element, "vector");
}

//

/**
 * @brief Retrieves an entity handle from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @note This will only work on offsets that are stored as "entity handles" (which usually looks like m_h* in properties)
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A string value at the given schema offset.
 */
extern "C" PLUGIN_API int GetEntSchemaEnt2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element) {
	const auto field = FindSchemaField(entity, className, memberName);
	if (!field) {
		return INVALID_EHANDLE_INDEX;
	}

	const auto* ptr = static_cast<const CEntityHandle*>(field->Element(schema::IsAtomicType(field->key.type, sizeof(CEntityHandle)), element, "entity handle"));
	return ptr ? ptr->ToInt() : INVALID_EHANDLE_INDEX;
}

/**
 * @brief Sets an entity handle in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @note This will only work on offsets that are stored as "entity handles" (which usually looks like m_h* in properties)
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The entity handle to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return An entity handle at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaEnt2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int value, bool changeState, int element) {
	const auto field = FindWritableSchemaField(entity, className, memberName);
	if (!field) {
		return;
	}

	auto* ptr = static_cast<CEntityHandle*>(field->Element(schema::IsAtomicType(field->key.type, sizeof(CEntityHandle)), element, "entity handle"));
	if (!ptr) {
		return;
	}

	field->NotifyChanged(changeState);
	*ptr = CEntityHandle(value);
}

/***
 * @brief Pushes an entity handle into an entity's schema collection.
 *
 * This function appends a new entity handle to a collection-typed schema field
 * (for example, a `CUtlVector<CEntityHandle>`). It performs strict offset and
 * type checking and optionally notifies the network system about the change.
 *
 * @note This will only work on offsets that are stored as "entity handles" in collections
 *       (which usually looks like m_h* in properties and use `CUtlVector<CEntityHandle>`).
 *
 * @param entity Pointer to the instance of the class where the value is to be pushed.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The entity handle to push.
 * @param changeState If true, change will be sent over the network.
 */
extern "C" PLUGIN_API void PushEntSchemaEnt2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int value, bool changeState) {
	const auto field = FindWritableSchemaField(entity, className, memberName, "push into");
	if (!field) {
		return;
	}

	if (schema::IsAtomicType(field->key.type, sizeof(CEntityHandle)) != schema::ElementType::Collection) {
		field->WarnType("collection of entity handles");
		return;
	}

	field->NotifyChanged(changeState);
	reinterpret_cast<CUtlVector<CEntityHandle>*>(field->Base())->AddToTail(CEntityHandle(value));
}

/**
 * @brief Erases an entity handle from an entity's schema collection by index.
 *
 * This function removes an entity handle at the given index from a collection-typed
 * schema field (for example, a `CUtlVector<CEntityHandle>`). It performs strict offset
 * and type checking and optionally notifies the network system about the change.
 *
 * @note This will only work on offsets that are stored as "entity handles" in collections
 *       (which usually looks like m_h* in properties and use `CUtlVector<CEntityHandle>`).
 *
 * @param entity Pointer to the instance of the class where the value is to be erased.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element index to erase (starting from 0).
 * @param changeState If true, change will be sent over the network.
 */
extern "C" PLUGIN_API void EraseEntSchemaEnt2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName, int element, bool changeState) {
	const auto field = FindWritableSchemaField(entity, className, memberName, "erase from");
	if (!field) {
		return;
	}

	if (schema::IsAtomicType(field->key.type, sizeof(CEntityHandle)) != schema::ElementType::Collection) {
		field->WarnType("collection of entity handles");
		return;
	}

	auto* vec = reinterpret_cast<CUtlVector<CEntityHandle>*>(field->Base());
	if (element < 0 || element >= vec->Count()) {
		plg::print(LS_WARNING, "Element {} is out of range for '{}::{}' (size: {})\n", element, className, memberName, vec->Count());
		return;
	}

	field->NotifyChanged(changeState);
	vec->Remove(element);
}

//

/**
 * @brief Updates the networked state of a schema field for a given entity pointer.
 *
 * This function ensures that changes to a schema field are appropriately networked,
 * while adhering to specific guidelines if configured. It validates whether the field
 * is networked, computes necessary offsets, and invokes the required methods to mark
 * the state as changed.
 *
 * @param entity Pointer to the instance of the class where the value is to be set.
 * @param className The name of the class that contains the member.
 * @param memberName The name of the member to be set.
 */
extern "C" PLUGIN_API void NetworkStateChanged2(CEntityInstance* entity, const plg::string& className, const plg::string& memberName) {
	if (const auto field = FindSchemaField(entity, className, memberName)) {
		field->NotifyChanged(true);
	}
}

//
//
//
//
//
//

/**
 * @brief Retrieves the count of values that an entity schema's array can store.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @return Size of array (in elements) or 0 if schema is not an array.
 */
extern "C" PLUGIN_API int GetEntSchemaArraySize(int entityHandle, const plg::string& className, const plg::string& memberName) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaArraySize2(entity, className, memberName); });
}

/**
 * @brief Retrieves a pointer to a structure or an array element in an entity's schema.
 *
 * Works for fixed arrays and collections (CUtlVector, CNetworkUtlVectorBase, etc.) of any element type,
 * including structures. For non-array fields returns a pointer to the field itself (element must be 0).
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A pointer to the field or element, or nullptr if the field is not found or element is out of range.
 */
extern "C" PLUGIN_API void* GetEntSchemaPtr(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaPtr2(entity, className, memberName, element); });
}

//

/**
 * @brief Retrieves an integer value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return An integer value at the given schema offset.
 */
extern "C" PLUGIN_API int64_t GetEntSchema(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchema2(entity, className, memberName, element); });
}

/**
 * @brief Sets an integer value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The integer value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return An integer value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchema(int entityHandle, const plg::string& className, const plg::string& memberName, int64_t value, bool changeState, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchema2(entity, className, memberName, value, changeState, element); });
}

//

/**
 * @brief Retrieves a float value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A float value at the given schema offset.
 */
extern "C" PLUGIN_API double GetEntSchemaFloat(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaFloat2(entity, className, memberName, element); });
}

/**
 * @brief Sets a float value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The float value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A float value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaFloat(int entityHandle, const plg::string& className, const plg::string& memberName, double value, bool changeState, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaFloat2(entity, className, memberName, value, changeState, element); });
}

//

/**
 * @brief Retrieves all elements of an integer array from an entity's schema.
 *
 * Works for fixed arrays and collections (CUtlVector, CNetworkUtlVectorBase, etc.) of integer-like elements.
 * This function is considered safer and more robust over GetEntDataArray, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @return The array elements, or an empty array if the field is not found or is not an integer array.
 */
extern "C" PLUGIN_API plg::vector<int64_t> GetEntSchemaArray(int entityHandle, const plg::string& className, const plg::string& memberName) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaArray2(entity, className, memberName); });
}

/**
 * @brief Sets all elements of an integer array in an entity's schema.
 *
 * For fixed arrays writes min(value.size(), array size) elements and leaves the rest untouched.
 * For collections (CUtlVector, CNetworkUtlVectorBase, etc.) resizes the collection to value.size().
 * This function is considered safer and more robust over SetEntDataArray, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The array elements to set.
 * @param changeState If true, change will be sent over the network.
 */
extern "C" PLUGIN_API void SetEntSchemaArray(int entityHandle, const plg::string& className, const plg::string& memberName, const plg::vector<int64_t>& value, bool changeState) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaArray2(entity, className, memberName, value, changeState); });
}

//

/**
 * @brief Retrieves all elements of a float array from an entity's schema.
 *
 * Works for fixed arrays and collections (CUtlVector, CNetworkUtlVectorBase, etc.) of float elements.
 * This function is considered safer and more robust over GetEntDataArrayFloat, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @return The array elements, or an empty array if the field is not found or is not a float array.
 */
extern "C" PLUGIN_API plg::vector<double> GetEntSchemaArrayFloat(int entityHandle, const plg::string& className, const plg::string& memberName) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaArrayFloat2(entity, className, memberName); });
}

/**
 * @brief Sets all elements of a float array in an entity's schema.
 *
 * For fixed arrays writes min(value.size(), array size) elements and leaves the rest untouched.
 * For collections (CUtlVector, CNetworkUtlVectorBase, etc.) resizes the collection to value.size().
 * This function is considered safer and more robust over SetEntDataArrayFloat, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be set.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The array elements to set.
 * @param changeState If true, change will be sent over the network.
 */
extern "C" PLUGIN_API void SetEntSchemaArrayFloat(int entityHandle, const plg::string& className, const plg::string& memberName, const plg::vector<double>& value, bool changeState) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaArrayFloat2(entity, className, memberName, value, changeState); });
}

//

/**
 * @brief Retrieves a color value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A color value at the given schema offset.
 */
extern "C" PLUGIN_API plg::vec4 GetEntSchemaColor(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaColor2(entity, className, memberName, element); });
}

/**
 * @brief Sets a color value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The color value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A color value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaColor(int entityHandle, const plg::string& className, const plg::string& memberName, const plg::vec4& value, bool changeState, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaColor2(entity, className, memberName, value, changeState, element); });
}

//

/**
 * @brief Retrieves a string value from an entity's schema.
*
* This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A string value at the given schema offset.
 */
extern "C" PLUGIN_API plg::string GetEntSchemaString(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaString2(entity, className, memberName, element); });
}

/**
 * @brief Sets a string value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The string value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return An string value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaString(int entityHandle, const plg::string& className, const plg::string& memberName, const plg::string& value, bool changeState, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaString2(entity, className, memberName, value, changeState, element); });
}

//

/**
 * @brief Retrieves a vector value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A string value at the given schema offset.
 */
extern "C" PLUGIN_API plg::vec3 GetEntSchemaVector3D(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaVector3D2(entity, className, memberName, element); });
}

/**
 * @brief Sets a vector value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaVector3D(int entityHandle, const plg::string& className, const plg::string& memberName, const plg::vec3& value, bool changeState, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaVector3D2(entity, className, memberName, value, changeState, element); });
}

//

/**
 * @brief Retrieves a vector value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A string value at the given schema offset.
 */
extern "C" PLUGIN_API plg::vec2 GetEntSchemaVector2D(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaVector2D2(entity, className, memberName, element); });
}

/**
 * @brief Sets a vector value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaVector2D(int entityHandle, const plg::string& className, const plg::string& memberName, const plg::vec2& value, bool changeState, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaVector2D2(entity, className, memberName, value, changeState, element); });
}

//

/**
 * @brief Retrieves a vector value from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A string value at the given schema offset.
 */
extern "C" PLUGIN_API plg::vec4 GetEntSchemaVector4D(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaVector4D2(entity, className, memberName, element); });
}

/**
 * @brief Sets a vector value in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The vector value to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A vector value at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaVector4D(int entityHandle, const plg::string& className, const plg::string& memberName, const plg::vec4& value, bool changeState, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaVector4D2(entity, className, memberName, value, changeState, element); });
}

//

/**
 * @brief Retrieves an entity handle from an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @note This will only work on offsets that are stored as "entity handles" (which usually looks like m_h* in properties)
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element # (starting from 0) if schema is an array.
 * @return A string value at the given schema offset.
 */
extern "C" PLUGIN_API int GetEntSchemaEnt(int entityHandle, const plg::string& className, const plg::string& memberName, int element) {
	return WithEntity(entityHandle, static_cast<int>(INVALID_EHANDLE_INDEX), [&](CEntityInstance* entity) { return GetEntSchemaEnt2(entity, className, memberName, element); });
}

/**
 * @brief Sets an entity handle in an entity's schema.
 *
 * This function is considered safer and more robust over GetEntData, because it performs strict offset checking and typing rules.
 *
 * @note This will only work on offsets that are stored as "entity handles" (which usually looks like m_h* in properties)
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The entity handle to set.
 * @param changeState If true, change will be sent over the network.
 * @param element Element # (starting from 0) if schema is an array.
 * @return An entity handle at the given schema offset.
 */
extern "C" PLUGIN_API void SetEntSchemaEnt(int entityHandle, const plg::string& className, const plg::string& memberName, int value, bool changeState, int element) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaEnt2(entity, className, memberName, value, changeState, element); });
}

/**
 * @brief Pushes an entity handle into an entity's schema collection.
 *
 * This is a convenience wrapper over PushEntSchemaEnt2 that resolves the entity
 * from its handle and appends a new entity handle value to the target schema
 * collection field.
 *
 * @param entityHandle The handle of the entity whose schema collection is modified.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param value The entity handle to push.
 * @param changeState If true, change will be sent over the network.
 */
extern "C" PLUGIN_API void PushEntSchemaEnt(int entityHandle, const plg::string& className, const plg::string& memberName, int value, bool changeState) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return PushEntSchemaEnt2(entity, className, memberName, value, changeState); });
}

/**
 * @brief Erases an entity handle from an entity's schema collection by index.
 *
 * This is a convenience wrapper over EraseEntSchemaEnt2 that resolves the entity
 * from its handle and removes an entity handle value at the given index from the
 * target schema collection field.
 *
 * @param entityHandle The handle of the entity whose schema collection is modified.
 * @param className The name of the class.
 * @param memberName The name of the schema member.
 * @param element Element index to erase (starting from 0).
 * @param changeState If true, change will be sent over the network.
 */
extern "C" PLUGIN_API void EraseEntSchemaEnt(int entityHandle, const plg::string& className, const plg::string& memberName, int element, bool changeState) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return EraseEntSchemaEnt2(entity, className, memberName, element, changeState); });
}

//

/**
 * @brief Updates the networked state of a schema field for a given entity handle.
 *
 * This function ensures that changes to a schema field are appropriately networked,
 * while adhering to specific guidelines if configured. It validates whether the field
 * is networked, computes necessary offsets, and invokes the required methods to mark
 * the state as changed.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param className The name of the class that contains the member.
 * @param memberName The name of the member to be set.
 */
extern "C" PLUGIN_API void NetworkStateChanged(int entityHandle, const plg::string& className, const plg::string& memberName) {
	return WithEntity(entityHandle, [&](CEntityInstance* entity) { return NetworkStateChanged2(entity, className, memberName); });
}

//
//
//
//
//
//

PLUGIFY_WARN_POP()
