#include <core/core_config.hpp>
#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/schema.hpp>
#include <core/sdk/utils.hpp>
#include <schemasystem/schemasystem.h>
#include <tier0/utlstring.h>
#include <tier1/utlsymbollarge.h>
#include <string.h>

#include <algorithm>

PLUGIFY_WARN_PUSH()

PLUGIFY_LINKAGE()

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
	switch (size) {
		case sizeof(int8_t):
			return *reinterpret_cast<int8_t*>(reinterpret_cast<intptr_t>(entity) + offset);
		case sizeof(int16_t):
			return *reinterpret_cast<int16_t*>(reinterpret_cast<intptr_t>(entity) + offset);
		case sizeof(int32_t):
			return *reinterpret_cast<int32_t*>(reinterpret_cast<intptr_t>(entity) + offset);
		case sizeof(int64_t):
			return *reinterpret_cast<int64_t*>(reinterpret_cast<intptr_t>(entity) + offset);
		default:
			return 0;
	}
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	switch (size) {
		case sizeof(int8_t):
			*reinterpret_cast<int8_t*>(reinterpret_cast<intptr_t>(entity) + offset) = static_cast<int8_t>(value);
			break;
		case sizeof(int16_t):
			*reinterpret_cast<int16_t*>(reinterpret_cast<intptr_t>(entity) + offset) = static_cast<int16_t>(value);
			break;
		case sizeof(int32_t):
			*reinterpret_cast<int32_t*>(reinterpret_cast<intptr_t>(entity) + offset) = static_cast<int32_t>(value);
			break;
		case sizeof(int64_t):
			*reinterpret_cast<int64_t*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
			break;
		default:
			break;
	}
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
	switch (size) {
		case sizeof(int32_t):
			return *reinterpret_cast<float*>(reinterpret_cast<intptr_t>(entity) + offset);
		case sizeof(int64_t):
			return *reinterpret_cast<double*>(reinterpret_cast<intptr_t>(entity) + offset);
		default:
			return 0;
	}
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	switch (size) {
		case sizeof(int32_t):
			*reinterpret_cast<float*>(reinterpret_cast<intptr_t>(entity) + offset) = static_cast<float>(value);
			break;
		case sizeof(int64_t):
			*reinterpret_cast<double*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
			break;
		default:
			break;
	}
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
	return std::bit_cast<plg::vec4>(reinterpret_cast<Color*>(reinterpret_cast<intptr_t>(entity) + offset)->ToVector4D());
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	*reinterpret_cast<Color*>(reinterpret_cast<intptr_t>(entity) + offset) = NewColor(value);
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
	return reinterpret_cast<CUtlString*>(reinterpret_cast<intptr_t>(entity) + offset)->Get();
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	*reinterpret_cast<CUtlString*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
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
	const char* src = reinterpret_cast<const char*>(reinterpret_cast<intptr_t>(entity) + offset);
	const std::size_t len = strnlen(src, size);
	return plg::string(src, len);
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	switch (size) {
		case -1: {
			const CUtlSymbolLarge pooled = g_pGameEntitySystem->AllocPooledString(value.c_str());
			*reinterpret_cast<CUtlSymbolLarge*>(reinterpret_cast<intptr_t>(entity) + offset) = pooled;

			break;
		}
		default: {
			char* dst = reinterpret_cast<char*>(reinterpret_cast<intptr_t>(entity) + offset);
			const size_t n = std::min(value.size(), static_cast<size_t>(size - 1));
			value.copy(dst, n);
			dst[n] = '\0';

			break;
		}
	}
}

//

namespace {
	// Converts `count` elements of type From stored at `src` into a vector of To.
	template<typename To, typename From>
	plg::vector<To> ReadArray(const void* src, size_t count) {
		plg::vector<To> result(count);
		const From* p = static_cast<const From*>(src);
		std::transform(p, p + count, result.begin(), [](const From v) { return static_cast<To>(v); });
		return result;
	}

	// Converts `count` elements of `src` into elements of type To stored at `dst`.
	template<typename To, typename From>
	void WriteArray(void* dst, const From* src, size_t count) {
		To* p = static_cast<To*>(dst);
		std::transform(src, src + count, p, [](const From v) { return static_cast<To>(v); });
	}

	plg::vector<int64_t> ReadIntArray(const void* src, size_t count, int dataSize) {
		switch (dataSize) {
			case sizeof(int8_t):
				return ReadArray<int64_t, int8_t>(src, count);
			case sizeof(int16_t):
				return ReadArray<int64_t, int16_t>(src, count);
			case sizeof(int32_t):
				return ReadArray<int64_t, int32_t>(src, count);
			case sizeof(int64_t):
				return ReadArray<int64_t, int64_t>(src, count);
			default:
				plg::print(LS_WARNING, "Invalid integer element size: {}\n", dataSize);
				return {};
		}
	}

	void WriteIntArray(void* dst, const int64_t* src, size_t count, int dataSize) {
		switch (dataSize) {
			case sizeof(int8_t):
				WriteArray<int8_t>(dst, src, count);
				break;
			case sizeof(int16_t):
				WriteArray<int16_t>(dst, src, count);
				break;
			case sizeof(int32_t):
				WriteArray<int32_t>(dst, src, count);
				break;
			case sizeof(int64_t):
				WriteArray<int64_t>(dst, src, count);
				break;
			default:
				plg::print(LS_WARNING, "Invalid integer element size: {}\n", dataSize);
				break;
		}
	}

	plg::vector<double> ReadFloatArray(const void* src, size_t count, int dataSize) {
		switch (dataSize) {
			case sizeof(float):
				return ReadArray<double, float>(src, count);
			case sizeof(double):
				return ReadArray<double, double>(src, count);
			default:
				plg::print(LS_WARNING, "Invalid float element size: {}\n", dataSize);
				return {};
		}
	}

	void WriteFloatArray(void* dst, const double* src, size_t count, int dataSize) {
		switch (dataSize) {
			case sizeof(float):
				WriteArray<float>(dst, src, count);
				break;
			case sizeof(double):
				WriteArray<double>(dst, src, count);
				break;
			default:
				plg::print(LS_WARNING, "Invalid float element size: {}\n", dataSize);
				break;
		}
	}

	// Resizes a CUtlVector whose elements are `dataSize` bytes and returns its data pointer.
	// The layout of CUtlVector (and CNetworkUtlVectorBase) does not depend on T, only the
	// allocation size does, so the element size picks the instantiation.
	void* ResizeUtlVector(intptr_t address, int count, int dataSize) {
		switch (dataSize) {
			case sizeof(int8_t): {
				auto* vec = reinterpret_cast<CUtlVector<int8_t>*>(address);
				vec->SetCount(count);
				return vec->Base();
			}
			case sizeof(int16_t): {
				auto* vec = reinterpret_cast<CUtlVector<int16_t>*>(address);
				vec->SetCount(count);
				return vec->Base();
			}
			case sizeof(int32_t): {
				auto* vec = reinterpret_cast<CUtlVector<int32_t>*>(address);
				vec->SetCount(count);
				return vec->Base();
			}
			case sizeof(int64_t): {
				auto* vec = reinterpret_cast<CUtlVector<int64_t>*>(address);
				vec->SetCount(count);
				return vec->Base();
			}
			default:
				plg::print(LS_WARNING, "Invalid vector element size: {}\n", dataSize);
				return nullptr;
		}
	}

	// Location and length of the elements of an array-like schema field.
	struct SchemaArrayView {
		intptr_t data{};
		int count{};
	};

	SchemaArrayView GetSchemaArrayView(intptr_t base, CSchemaType* type, schema::ElementType elementType) {
		switch (elementType) {
			case schema::ElementType::Array:
				return {base, static_cast<const CSchemaType_FixedArray*>(type)->m_nElementCount};
			case schema::ElementType::Collection: {
				const auto* vec = reinterpret_cast<CUtlVector<uint8>*>(base);
				return {reinterpret_cast<intptr_t>(vec->Base()), vec->Count()};
			}
			default:
				return {};
		}
	}
} // namespace

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
	if (arraySize <= 0) {
		return {};
	}

	return ReadIntArray(reinterpret_cast<const void*>(reinterpret_cast<intptr_t>(entity) + offset), static_cast<size_t>(arraySize), dataSize);
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
	if (arraySize <= 0) {
		return;
	}

	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	const size_t n = std::min(value.size(), static_cast<size_t>(arraySize));
	WriteIntArray(reinterpret_cast<void*>(reinterpret_cast<intptr_t>(entity) + offset), value.data(), n, dataSize);
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
	if (arraySize <= 0) {
		return {};
	}

	return ReadFloatArray(reinterpret_cast<const void*>(reinterpret_cast<intptr_t>(entity) + offset), static_cast<size_t>(arraySize), dataSize);
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
	if (arraySize <= 0) {
		return;
	}

	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	const size_t n = std::min(value.size(), static_cast<size_t>(arraySize));
	WriteFloatArray(reinterpret_cast<void*>(reinterpret_cast<intptr_t>(entity) + offset), value.data(), n, dataSize);
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
	const auto* vec = reinterpret_cast<CUtlVector<uint8>*>(reinterpret_cast<intptr_t>(entity) + offset);
	const int count = vec->Count();
	if (count <= 0) {
		return {};
	}

	return ReadIntArray(vec->Base(), static_cast<size_t>(count), dataSize);
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	const int count = static_cast<int>(value.size());
	void* data = ResizeUtlVector(reinterpret_cast<intptr_t>(entity) + offset, count, dataSize);
	if (data && count > 0) {
		WriteIntArray(data, value.data(), value.size(), dataSize);
	}
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
	const auto* vec = reinterpret_cast<CUtlVector<uint8>*>(reinterpret_cast<intptr_t>(entity) + offset);
	const int count = vec->Count();
	if (count <= 0) {
		return {};
	}

	return ReadFloatArray(vec->Base(), static_cast<size_t>(count), dataSize);
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
	if (dataSize != sizeof(float) && dataSize != sizeof(double)) {
		plg::print(LS_WARNING, "Invalid float element size: {}\n", dataSize);
		return;
	}

	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	const int count = static_cast<int>(value.size());
	void* data = ResizeUtlVector(reinterpret_cast<intptr_t>(entity) + offset, count, dataSize);
	if (data && count > 0) {
		WriteFloatArray(data, value.data(), value.size(), dataSize);
	}
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
	return *reinterpret_cast<plg::vec3*>(reinterpret_cast<intptr_t>(entity) + offset);
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	*reinterpret_cast<plg::vec3*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
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
	return *reinterpret_cast<plg::vec4*>(reinterpret_cast<intptr_t>(entity) + offset);
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	*reinterpret_cast<plg::vec4*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
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
	return *reinterpret_cast<plg::vec2*>(reinterpret_cast<intptr_t>(entity) + offset);
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	*reinterpret_cast<plg::vec2*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
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
	return reinterpret_cast<CEntityHandle*>(reinterpret_cast<intptr_t>(entity) + offset)->ToInt();
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
	if (changeState) {
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	}

	*reinterpret_cast<CEntityHandle*>(reinterpret_cast<intptr_t>(entity) + offset) = CEntityHandle(value);
}

/**
 * @brief Peeks into an entity's object data and retrieves a pointer to the data at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return A pointer to the data at the given memory location.
 */
extern "C" PLUGIN_API void* GetEntDataPtr2(CEntityInstance* entity, int offset) {
	return reinterpret_cast<void*>(reinterpret_cast<intptr_t>(entity) + offset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntData2(entity, offset, size);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntData2(entity, offset, value, size, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataFloat2(entity, offset, size);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataFloat2(entity, offset, value, size, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataColor2(entity, offset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataColor2(entity, offset, value, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataString2(entity, offset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataString2(entity, offset, value, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataCString2(entity, offset, size);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataCString2(entity, offset, value, size, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataCArray2(entity, offset, arraySize, dataSize);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataCArray2(entity, offset, value, arraySize, dataSize, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataCArrayFloat2(entity, offset, arraySize, dataSize);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataCArrayFloat2(entity, offset, value, arraySize, dataSize, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataArray2(entity, offset, dataSize);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataArray2(entity, offset, value, dataSize, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataArrayFloat2(entity, offset, dataSize);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataArrayFloat2(entity, offset, value, dataSize, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataVector3D2(entity, offset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataVector3D2(entity, offset, value, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataVector4D2(entity, offset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataVector4D2(entity, offset, value, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return {};
	}

	return GetEntDataVector2D2(entity, offset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataVector2D2(entity, offset, value, changeState, chainOffset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return INVALID_EHANDLE_INDEX;
	}

	return GetEntDataEnt2(entity, offset);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	SetEntDataEnt2(entity, offset, value, changeState, chainOffset);
}

/**
 * @brief Peeks into an entity's object data and retrieves a pointer to the data at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return A pointer to the data at the given memory location.
 */
extern "C" PLUGIN_API void* GetEntDataPtr(int entityHandle, int offset) {
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return nullptr;
	}

	return GetEntDataPtr2(entity, offset);
}

/**
 * @brief Updates the networked state of a schema field for a given entity handle.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void ChangeEntityState(int entityHandle, int offset, int chainOffset) {
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot change state of '{}' with invalid entity handle: {}\n", offset, entityHandle);
		return;
	}

	return ChangeEntityState2(entity, offset, chainOffset);
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	const auto elementType = schema::GetElementType(type);
	switch (elementType) {
		case schema::ElementType::Array:
			return static_cast<const CSchemaType_FixedArray*>(type)->m_nElementCount;
		case schema::ElementType::Collection:
			return reinterpret_cast<CUtlVector<intptr_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Count();
		default:
			return 0;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return nullptr;
	}

	const intptr_t base = reinterpret_cast<intptr_t>(entity) + offset;
	int count;
	intptr_t data;
	int elementSize;

	switch (schema::GetElementType(type)) {
		case schema::ElementType::Array: {
			const auto* arrayType = static_cast<const CSchemaType_FixedArray*>(type);
			count = arrayType->m_nElementCount;
			data = base;
			elementSize = arrayType->m_nElementSize;
			break;
		}
		case schema::ElementType::Collection: {
			const auto* vec = reinterpret_cast<CUtlVector<uint8>*>(base);
			count = vec->Count();
			data = reinterpret_cast<intptr_t>(vec->Base());
			elementSize = static_cast<const CSchemaType_Atomic_CollectionOfT*>(type)->m_nElementSize;
			break;
		}
		case schema::ElementType::Single:
			count = 1;
			data = base;
			elementSize = 0;
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' has unsupported type '{}'\n", className, memberName, type->m_sTypeName.Get());
			return nullptr;
	}

	if (element < 0 || element >= count) {
		plg::print(LS_WARNING, "Element {} is out of range for '{}::{}' (size: {})\n", element, className, memberName, count);
		return nullptr;
	}

	return reinterpret_cast<void*>(data + static_cast<intptr_t>(element) * elementSize);
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	const auto [elementType, elementSize] = schema::IsIntType(type);
	switch (elementType) {
		case schema::ElementType::Array:
			switch (elementSize) {
				case sizeof(int8_t):
					return reinterpret_cast<int8_t*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
				case sizeof(int16_t):
					return reinterpret_cast<int16_t*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
				case sizeof(int32_t):
					return reinterpret_cast<int32_t*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
				case sizeof(int64_t):
					return reinterpret_cast<int64_t*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
				default:
					return 0;
			}
			break;
		case schema::ElementType::Collection:
			switch (elementSize) {
				case sizeof(int8_t):
					return reinterpret_cast<CUtlVector<int8_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
				case sizeof(int16_t):
					return reinterpret_cast<CUtlVector<int16_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
				case sizeof(int32_t):
					return reinterpret_cast<CUtlVector<int32_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
				case sizeof(int64_t):
					return reinterpret_cast<CUtlVector<int64_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
				default:
					return 0;
			}
			break;
		case schema::ElementType::Single:
			switch (size) {
				case sizeof(int8_t):
					return *reinterpret_cast<int8_t*>(reinterpret_cast<intptr_t>(entity) + offset);
				case sizeof(int16_t):
					return *reinterpret_cast<int16_t*>(reinterpret_cast<intptr_t>(entity) + offset);
				case sizeof(int32_t):
					return *reinterpret_cast<int32_t*>(reinterpret_cast<intptr_t>(entity) + offset);
				case sizeof(int64_t):
					return *reinterpret_cast<int64_t*>(reinterpret_cast<intptr_t>(entity) + offset);
				default:
					return 0;
			}
			break;
		case schema::ElementType::Class:
			return reinterpret_cast<intptr_t>(entity) + offset;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a integer, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return 0;
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	const auto [elementType, elementSize] = schema::IsIntType(type);
	switch (elementType) {
		case schema::ElementType::Array:
			switch (elementSize) {
				case sizeof(int8_t):
					reinterpret_cast<int8_t*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = static_cast<int8_t>(value);
					break;
				case sizeof(int16_t):
					reinterpret_cast<int16_t*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = static_cast<int16_t>(value);
					break;
				case sizeof(int32_t):
					reinterpret_cast<int32_t*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = static_cast<int32_t>(value);
					break;
				case sizeof(int64_t):
					reinterpret_cast<int64_t*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = value;
					break;
				default:
					break;
			}
			break;
		case schema::ElementType::Collection:
			switch (elementSize) {
				case sizeof(int8_t):
					reinterpret_cast<CUtlVector<int8_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = static_cast<int8_t>(value);
					break;
				case sizeof(int16_t):
					reinterpret_cast<CUtlVector<int16_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = static_cast<int16_t>(value);
					break;
				case sizeof(int32_t):
					reinterpret_cast<CUtlVector<int32_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = static_cast<int32_t>(value);
					break;
				case sizeof(int64_t):
					reinterpret_cast<CUtlVector<int64_t>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = value;
					break;
				default:
					break;
			}
			break;
		case schema::ElementType::Single:
			switch (size) {
				case sizeof(int8_t):
					*reinterpret_cast<int8_t*>(reinterpret_cast<intptr_t>(entity) + offset) = static_cast<int8_t>(value);
					break;
				case sizeof(int16_t):
					*reinterpret_cast<int16_t*>(reinterpret_cast<intptr_t>(entity) + offset) = static_cast<int16_t>(value);
					break;
				case sizeof(int32_t):
					*reinterpret_cast<int32_t*>(reinterpret_cast<intptr_t>(entity) + offset) = static_cast<int32_t>(value);
					break;
				case sizeof(int64_t):
					*reinterpret_cast<int64_t*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
					break;
				default:
					break;
			}
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a integer, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	const auto [elementType, elementSize] = schema::IsFloatType(type);
	switch (elementType) {
		case schema::ElementType::Array:
			switch (elementSize) {
				case sizeof(float):
					return reinterpret_cast<float*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
				case sizeof(double):
					return reinterpret_cast<double*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
				default:
					return 0;
			}
			break;
		case schema::ElementType::Collection:
			switch (elementSize) {
				case sizeof(float):
					return reinterpret_cast<CUtlVector<float>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
				case sizeof(double):
					return reinterpret_cast<CUtlVector<double>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
				default:
					return 0;
			}
			break;
		case schema::ElementType::Single:
			switch (size) {
				case sizeof(float):
					return *reinterpret_cast<float*>(reinterpret_cast<intptr_t>(entity) + offset);
				case sizeof(double):
					return *reinterpret_cast<double*>(reinterpret_cast<intptr_t>(entity) + offset);
				default:
					return 0;
			}
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a float, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return 0;
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	const auto [elementType, elementSize] = schema::IsFloatType(type);
	switch (elementType) {
		case schema::ElementType::Array:
			switch (elementSize) {
				case sizeof(float):
					reinterpret_cast<float*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = static_cast<float>(value);
					break;
				case sizeof(double):
					reinterpret_cast<double*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = value;
					break;
				default:
					break;
			}
			break;
		case schema::ElementType::Collection:
			switch (elementSize) {
				case sizeof(float):
					reinterpret_cast<CUtlVector<float>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = static_cast<float>(value);
					break;
				case sizeof(double):
					reinterpret_cast<CUtlVector<double>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = value;
					break;
				default:
					break;
			}
			break;
		case schema::ElementType::Single:
			switch (size) {
				case sizeof(float):
					*reinterpret_cast<float*>(reinterpret_cast<intptr_t>(entity) + offset) = static_cast<float>(value);
					break;
				case sizeof(double):
					*reinterpret_cast<double*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
					break;
				default:
					break;
			}
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a float, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	const auto [elementType, elementSize] = schema::IsIntType(type);
	switch (elementType) {
		case schema::ElementType::Array:
		case schema::ElementType::Collection: {
			const auto [data, count] = GetSchemaArrayView(reinterpret_cast<intptr_t>(entity) + offset, type, elementType);
			if (count <= 0) {
				return {};
			}
			return ReadIntArray(reinterpret_cast<const void*>(data), static_cast<size_t>(count), elementSize);
		}
		case schema::ElementType::Single:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not an array\n", className, memberName);
			return {};
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a integer array, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return {};
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	const auto [elementType, elementSize] = schema::IsIntType(type);
	if (elementType != schema::ElementType::Array && elementType != schema::ElementType::Collection) {
		if (elementType == schema::ElementType::Single) {
			plg::print(LS_WARNING, "Schema field '{}::{}' is not an array\n", className, memberName);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a integer array, but '{}'\n", className, memberName, type->m_sTypeName.Get());
		}
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	const intptr_t base = reinterpret_cast<intptr_t>(entity) + offset;

	if (elementType == schema::ElementType::Array) {
		const int count = static_cast<const CSchemaType_FixedArray*>(type)->m_nElementCount;
		if (value.size() > static_cast<size_t>(count)) {
			plg::print(LS_WARNING, "Schema array '{}::{}' can hold {} elements, {} were given, extra elements are ignored\n", className, memberName, count, value.size());
		}
		const size_t n = std::min(value.size(), static_cast<size_t>(count));
		WriteIntArray(reinterpret_cast<void*>(base), value.data(), n, elementSize);
	} else {
		const int count = static_cast<int>(value.size());
		void* data = ResizeUtlVector(base, count, elementSize);
		if (data && count > 0) {
			WriteIntArray(data, value.data(), value.size(), elementSize);
		}
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	const auto [elementType, elementSize] = schema::IsFloatType(type);
	switch (elementType) {
		case schema::ElementType::Array:
		case schema::ElementType::Collection: {
			const auto [data, count] = GetSchemaArrayView(reinterpret_cast<intptr_t>(entity) + offset, type, elementType);
			if (count <= 0) {
				return {};
			}
			return ReadFloatArray(reinterpret_cast<const void*>(data), static_cast<size_t>(count), elementSize);
		}
		case schema::ElementType::Single:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not an array\n", className, memberName);
			return {};
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a float array, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return {};
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	const auto [elementType, elementSize] = schema::IsFloatType(type);
	if (elementType != schema::ElementType::Array && elementType != schema::ElementType::Collection) {
		if (elementType == schema::ElementType::Single) {
			plg::print(LS_WARNING, "Schema field '{}::{}' is not an array\n", className, memberName);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a float array, but '{}'\n", className, memberName, type->m_sTypeName.Get());
		}
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	const intptr_t base = reinterpret_cast<intptr_t>(entity) + offset;

	if (elementType == schema::ElementType::Array) {
		const int count = static_cast<const CSchemaType_FixedArray*>(type)->m_nElementCount;
		if (value.size() > static_cast<size_t>(count)) {
			plg::print(LS_WARNING, "Schema array '{}::{}' can hold {} elements, {} were given, extra elements are ignored\n", className, memberName, count, value.size());
		}
		const size_t n = std::min(value.size(), static_cast<size_t>(count));
		WriteFloatArray(reinterpret_cast<void*>(base), value.data(), n, elementSize);
	} else {
		const int count = static_cast<int>(value.size());
		void* data = ResizeUtlVector(base, count, elementSize);
		if (data && count > 0) {
			WriteFloatArray(data, value.data(), value.size(), elementSize);
		}
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	const auto [elementType, elementSize] = schema::IsIntType(type);
	switch (elementType) {
		case schema::ElementType::Array:
			switch (elementSize) {
				case sizeof(int):
					return std::bit_cast<plg::vec4>(reinterpret_cast<Color*>(reinterpret_cast<intptr_t>(entity) + offset)[element].ToVector4D());
				default:
					return {};
			}
			break;
		case schema::ElementType::Collection:
			switch (elementSize) {
				case sizeof(int):
					return std::bit_cast<plg::vec4>(reinterpret_cast<CUtlVector<Color>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element).ToVector4D());
				default:
					return {};
			}
			break;
		case schema::ElementType::Single:
			switch (size) {
				case sizeof(int):
					return std::bit_cast<plg::vec4>(reinterpret_cast<Color*>(reinterpret_cast<intptr_t>(entity) + offset)->ToVector4D());
				default:
					return {};
			}
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a integer, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return {};
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	const auto [elementType, elementSize] = schema::IsIntType(type);
	switch (elementType) {
		case schema::ElementType::Array:
			switch (elementSize) {
				case sizeof(int):
					reinterpret_cast<Color*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = NewColor(value);
					break;
				default:
					break;
			}
			break;
		case schema::ElementType::Collection:
			switch (elementSize) {
				case sizeof(int):
					reinterpret_cast<CUtlVector<Color>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = NewColor(value);
					break;
				default:
					break;
			}
			break;
		case schema::ElementType::Single:
			switch (size) {
				case sizeof(int):
					*reinterpret_cast<Color*>(reinterpret_cast<intptr_t>(entity) + offset) = NewColor(value);
					break;
				default:
					break;
			}
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a integer, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	switch (schema::IsPlainType(type, sizeof(CUtlString))) {
		case schema::ElementType::Array:
			return reinterpret_cast<CUtlString*>(reinterpret_cast<intptr_t>(entity) + offset)[element].Get();
		case schema::ElementType::Collection:
			return reinterpret_cast<CUtlVector<CUtlString>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element).Get();
		case schema::ElementType::Single:
			return reinterpret_cast<CUtlString*>(reinterpret_cast<intptr_t>(entity) + offset)->Get();
		default:
			const auto [elementType, elementSize] = schema::IsIntType(type);
			switch (elementType) {
				case schema::ElementType::Array:
					if (elementSize == sizeof(char)) {
						const char* src = reinterpret_cast<const char*>(reinterpret_cast<intptr_t>(entity) + offset);
						const std::size_t len = strnlen(src, size);
						return plg::string(src, len);
					}
					break;
				case schema::ElementType::Collection:
					if (elementSize == sizeof(char)) {
						const auto& vec = *reinterpret_cast<CUtlVector<char>*>(reinterpret_cast<intptr_t>(entity) + offset);
						return plg::string(vec.Base(), static_cast<size_t>(vec.Count()));
					}
					break;
				case schema::ElementType::Single:
					if (size == sizeof(char)) {
						return plg::string(1, *reinterpret_cast<char*>(reinterpret_cast<intptr_t>(entity) + offset));
					}
					break;
				default:
					break;
			}
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a string, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return {};
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	switch (schema::IsPlainType(type, sizeof(CUtlString))) {
		case schema::ElementType::Array:
			reinterpret_cast<CUtlString*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = value;
			break;
		case schema::ElementType::Collection:
			reinterpret_cast<CUtlVector<CUtlString>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = value;
			break;
		case schema::ElementType::Single:
			*reinterpret_cast<CUtlString*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
			break;
		default:
			const auto [elementType, elementSize] = schema::IsIntType(type);
			switch (elementType) {
				case schema::ElementType::Array:
					if (elementSize == sizeof(char)) {
						char* dst = reinterpret_cast<char*>(reinterpret_cast<intptr_t>(entity) + offset);
						const size_t n = std::min(value.size(), size - 1);
						value.copy(dst, n);
						dst[n] = '\0';
						return;
					}
					break;
				case schema::ElementType::Collection:
					if (elementSize == sizeof(char)) {
						reinterpret_cast<CUtlVector<char>*>(reinterpret_cast<intptr_t>(entity) + offset)->CopyArray(value.data(), static_cast<CUtlVector<char>::IndexType_t>(value.size()));
						return;
					}
					break;
				case schema::ElementType::Single:
					if (size == sizeof(char)) {
						*reinterpret_cast<char*>(reinterpret_cast<intptr_t>(entity) + offset) = value.empty() ? 0 : *value.begin();
						return;
					}
					break;
				default:
					break;
			}
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a string, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	switch (schema::IsPlainType(type, sizeof(Vector))) {
		case schema::ElementType::Array:
			return reinterpret_cast<plg::vec3*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
		case schema::ElementType::Collection:
			return reinterpret_cast<CUtlVector<plg::vec3>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
		case schema::ElementType::Single:
			return *reinterpret_cast<plg::vec3*>(reinterpret_cast<intptr_t>(entity) + offset);
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a vector, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return {};
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	switch (schema::IsPlainType(type, sizeof(Vector))) {
		case schema::ElementType::Array:
			reinterpret_cast<plg::vec3*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = value;
			break;
		case schema::ElementType::Collection:
			reinterpret_cast<CUtlVector<plg::vec3>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = value;
			break;
		case schema::ElementType::Single:
			*reinterpret_cast<plg::vec3*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a vector, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	switch (schema::IsPlainType(type, sizeof(Vector2D))) {
		case schema::ElementType::Array:
			return reinterpret_cast<plg::vec2*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
		case schema::ElementType::Collection:
			return reinterpret_cast<CUtlVector<plg::vec2>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
		case schema::ElementType::Single:
			return *reinterpret_cast<plg::vec2*>(reinterpret_cast<intptr_t>(entity) + offset);
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a vector, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return {};
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	switch (schema::IsPlainType(type, sizeof(Vector2D))) {
		case schema::ElementType::Array:
			reinterpret_cast<plg::vec2*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = value;
			break;
		case schema::ElementType::Collection:
			reinterpret_cast<CUtlVector<plg::vec2>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = value;
			break;
		case schema::ElementType::Single:
			*reinterpret_cast<plg::vec2*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a vector, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return {};
	}

	switch (schema::IsPlainType(type, sizeof(Vector4D))) {
		case schema::ElementType::Array:
			return reinterpret_cast<plg::vec4*>(reinterpret_cast<intptr_t>(entity) + offset)[element];
		case schema::ElementType::Collection:
			return reinterpret_cast<CUtlVector<plg::vec4>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element);
		case schema::ElementType::Single:
			return *reinterpret_cast<plg::vec4*>(reinterpret_cast<intptr_t>(entity) + offset);
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a vector, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return {};
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	switch (schema::IsPlainType(type, sizeof(Vector4D))) {
		case schema::ElementType::Array:
			reinterpret_cast<plg::vec4*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = value;
			break;
		case schema::ElementType::Collection:
			reinterpret_cast<CUtlVector<plg::vec4>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = value;
			break;
		case schema::ElementType::Single:
			*reinterpret_cast<plg::vec4*>(reinterpret_cast<intptr_t>(entity) + offset) = value;
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a vector, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return INVALID_EHANDLE_INDEX;
	}

	switch (schema::IsAtomicType(type, sizeof(CEntityHandle))) {
		case schema::ElementType::Array:
			return reinterpret_cast<CEntityHandle*>(reinterpret_cast<intptr_t>(entity) + offset)[element].ToInt();
		case schema::ElementType::Collection:
			return reinterpret_cast<CUtlVector<CEntityHandle>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element).ToInt();
		case schema::ElementType::Single:
			return reinterpret_cast<CEntityHandle*>(reinterpret_cast<intptr_t>(entity) + offset)->ToInt();
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a entity handle, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			return INVALID_EHANDLE_INDEX;
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	switch (schema::IsAtomicType(type, sizeof(CEntityHandle))) {
		case schema::ElementType::Array:
			reinterpret_cast<CEntityHandle*>(reinterpret_cast<intptr_t>(entity) + offset)[element] = CEntityHandle(value);
			break;
		case schema::ElementType::Collection:
			reinterpret_cast<CUtlVector<CEntityHandle>*>(reinterpret_cast<intptr_t>(entity) + offset)->Element(element) = CEntityHandle(value);
			break;
		case schema::ElementType::Single:
			*reinterpret_cast<CEntityHandle*>(reinterpret_cast<intptr_t>(entity) + offset) = CEntityHandle(value);
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a entity handle, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot push into '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	switch (schema::IsAtomicType(type, sizeof(CEntityHandle))) {
		case schema::ElementType::Collection: {
			auto* vec = reinterpret_cast<CUtlVector<CEntityHandle>*>(reinterpret_cast<intptr_t>(entity) + offset);
			vec->AddToTail(CEntityHandle(value));
			break;
		}
		case schema::ElementType::Array:
		case schema::ElementType::Single:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a collection of entity handles and cannot be pushed into.\n", className, memberName);
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a entity handle, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	if (g_pCoreConfig->FollowCS2ServerGuidelines && schema::CS2BadList.contains(memberName)) {
		plg::print(LS_WARNING, "Cannot erase from '{}::{}' with \"FollowCS2ServerGuidelines\" option enabled.\n", className, memberName);
		return;
	}

	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (offset == -1) {
		plg::print(LS_WARNING, "Cannot find offset for '{}::{}' with entity pointer: {}\n", className, memberName, static_cast<const void*>(entity));
		return;
	}

	if (changeState) {
		if (networked) {
			const auto chainOffset = schema::FindChainOffset(className);
			SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
		} else {
			plg::print(LS_WARNING, "Schema field '{}::{}' cannot be send over network\n", className, memberName);
		}
	}

	switch (schema::IsAtomicType(type, sizeof(CEntityHandle))) {
		case schema::ElementType::Collection: {
			auto* vec = reinterpret_cast<CUtlVector<CEntityHandle>*>(reinterpret_cast<intptr_t>(entity) + offset);
			if (element < 0 || element >= vec->Count()) {
				plg::print(LS_WARNING, "Index '{}' is out of range for schema collection '{}::{}' (size = {}).\n", element, className, memberName, vec->Count());
				return;
			}
			vec->Remove(element);
			break;
		}
		case schema::ElementType::Array:
		case schema::ElementType::Single:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a collection of entity handles and cannot erase by index.\n", className, memberName);
			break;
		default:
			plg::print(LS_WARNING, "Schema field '{}::{}' is not a entity handle, but '{}'\n", className, memberName, type->m_sTypeName.Get());
			break;
	}
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
	const auto [offset, networked, size, type] = schema::GetOffset(className, memberName);
	if (networked) {
		const auto chainOffset = schema::FindChainOffset(className);
		SafeNetworkStateChanged(reinterpret_cast<intptr_t>(entity), offset, chainOffset);
	} else {
		plg::print(LS_WARNING, "Field '{}::{}' is not networked, but \"SetStateChanged\" was called on it.", className, memberName);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaArraySize2(entity, className, memberName);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return nullptr;
	}

	return GetEntSchemaPtr2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchema2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchema2(entity, className, memberName, value, changeState, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaFloat2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaFloat2(entity, className, memberName, value, changeState, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaArray2(entity, className, memberName);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaArray2(entity, className, memberName, value, changeState);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot get '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaArrayFloat2(entity, className, memberName);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaArrayFloat2(entity, className, memberName, value, changeState);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaColor2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaColor2(entity, className, memberName, value, changeState, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaString2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaString2(entity, className, memberName, value, changeState, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaVector3D2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaVector3D2(entity, className, memberName, value, changeState, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaVector2D2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaVector2D2(entity, className, memberName, value, changeState, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return {};
	}

	return GetEntSchemaVector4D2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaVector4D2(entity, className, memberName, value, changeState, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return INVALID_EHANDLE_INDEX;
	}

	return GetEntSchemaEnt2(entity, className, memberName, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot set '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	SetEntSchemaEnt2(entity, className, memberName, value, changeState, element);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot push into '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	PushEntSchemaEnt2(entity, className, memberName, value, changeState);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot erase from '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	EraseEntSchemaEnt2(entity, className, memberName, element, changeState);
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
	CEntityInstance* entity = g_pGameEntitySystem->GetEntityInstance(CEntityHandle(entityHandle));
	if (!entity) {
		plg::print(LS_WARNING, "Cannot change state of '{}::{}' with invalid entity handle: {}\n", className, memberName, entityHandle);
		return;
	}

	NetworkStateChanged2(entity, className, memberName);
}

//
//
//
//
//
//

PLUGIFY_WARN_POP()
