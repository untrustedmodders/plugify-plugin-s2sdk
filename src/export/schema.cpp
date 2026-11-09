#include <core/core_config.hpp>
#include <core/sdk/entity/cbaseentity.h>
#include <core/sdk/helpers.hpp>
#include <core/sdk/schema.hpp>
#include <core/sdk/schema_helpers.hpp>
#include <core/sdk/utils.hpp>
#include <schemasystem/schemasystem.h>
#include <tier0/utlstring.h>
#include <tier1/utlsymbollarge.h>
#include <string.h>

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
	return schema::ReadValue<int64_t>(&schema::DataAt<uint8>(entity, offset), size);
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
	schema::NotifyDataChanged(entity, offset, chainOffset, changeState);
	schema::WriteValue<int64_t>(&schema::DataAt<uint8>(entity, offset), size, value);
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
	return schema::ReadValue<double>(&schema::DataAt<uint8>(entity, offset), size);
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
	schema::NotifyDataChanged(entity, offset, chainOffset, changeState);
	schema::WriteValue<double>(&schema::DataAt<uint8>(entity, offset), size, value);
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
	return std::bit_cast<plg::vec4>(schema::DataAt<Color>(entity, offset).ToVector4D());
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
	schema::SetData(entity, offset, NewColor(value), changeState, chainOffset);
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
	return schema::DataAt<CUtlString>(entity, offset).Get();
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
	schema::NotifyDataChanged(entity, offset, chainOffset, changeState);
	schema::DataAt<CUtlString>(entity, offset) = value;
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
	const char* src = &schema::DataAt<char>(entity, offset);
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
	schema::NotifyDataChanged(entity, offset, chainOffset, changeState);

	if (size == -1) {
		schema::DataAt<CUtlSymbolLarge>(entity, offset) = g_pGameEntitySystem->AllocPooledString(value.c_str());
		return;
	}

	char* dst = &schema::DataAt<char>(entity, offset);
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
	return schema::GetDataCArray<int64_t>(entity, offset, arraySize, dataSize);
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
	schema::SetDataCArray(entity, offset, value, arraySize, dataSize, changeState, chainOffset);
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
	return schema::GetDataCArray<double>(entity, offset, arraySize, dataSize);
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
	schema::SetDataCArray(entity, offset, value, arraySize, dataSize, changeState, chainOffset);
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
	return schema::GetDataVector<int64_t>(entity, offset, dataSize);
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
	schema::SetDataVector(entity, offset, value, dataSize, changeState, chainOffset);
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
	return schema::GetDataVector<double>(entity, offset, dataSize);
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
	schema::SetDataVector(entity, offset, value, dataSize, changeState, chainOffset);
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
	return schema::DataAt<plg::vec3>(entity, offset);
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
	schema::SetData(entity, offset, value, changeState, chainOffset);
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
	return schema::DataAt<plg::vec4>(entity, offset);
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
	schema::SetData(entity, offset, value, changeState, chainOffset);
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
	return schema::DataAt<plg::vec2>(entity, offset);
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
	schema::SetData(entity, offset, value, changeState, chainOffset);
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
	return schema::DataAt<CEntityHandle>(entity, offset).ToInt();
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
	schema::SetData(entity, offset, CEntityHandle(value), changeState, chainOffset);
}

/**
 * @brief Peeks into an entity's object data and retrieves a pointer to the data at the given offset.
 *
 * @param entity Pointer to the instance of the class where the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return A pointer to the data at the given memory location.
 */
extern "C" PLUGIN_API void* GetEntDataPtr2(CEntityInstance* entity, int offset) {
	return &schema::DataAt<uint8>(entity, offset);
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntData2(entity, offset, size); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntData2(entity, offset, value, size, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataFloat2(entity, offset, size); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataFloat2(entity, offset, value, size, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataColor2(entity, offset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataColor2(entity, offset, value, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataString2(entity, offset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataString2(entity, offset, value, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataCString2(entity, offset, size); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataCString2(entity, offset, value, size, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataCArray2(entity, offset, arraySize, dataSize); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataCArray2(entity, offset, value, arraySize, dataSize, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataCArrayFloat2(entity, offset, arraySize, dataSize); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataCArrayFloat2(entity, offset, value, arraySize, dataSize, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataArray2(entity, offset, dataSize); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataArray2(entity, offset, value, dataSize, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataArrayFloat2(entity, offset, dataSize); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataArrayFloat2(entity, offset, value, dataSize, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataVector3D2(entity, offset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataVector3D2(entity, offset, value, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataVector4D2(entity, offset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataVector4D2(entity, offset, value, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataVector2D2(entity, offset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataVector2D2(entity, offset, value, changeState, chainOffset); });
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
	return helpers::WithEntity(entityHandle, static_cast<int>(INVALID_EHANDLE_INDEX), [&](CEntityInstance* entity) { return GetEntDataEnt2(entity, offset); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntDataEnt2(entity, offset, value, changeState, chainOffset); });
}

/**
 * @brief Peeks into an entity's object data and retrieves a pointer to the data at the given offset.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @return A pointer to the data at the given memory location.
 */
extern "C" PLUGIN_API void* GetEntDataPtr(int entityHandle, int offset) {
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntDataPtr2(entity, offset); });
}

/**
 * @brief Updates the networked state of a schema field for a given entity handle.
 *
 * @param entityHandle The handle of the entity from which the value is to be retrieved.
 * @param offset The offset of the schema to use.
 * @param chainOffset The offset of the chain entity in the class (-2 for non-entity classes).
 */
extern "C" PLUGIN_API void ChangeEntityState(int entityHandle, int offset, int chainOffset) {
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return ChangeEntityState2(entity, offset, chainOffset); });
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
	const auto field = schema::FindField(entity, className, memberName);
	if (!field) {
		return {};
	}

	return schema::GetArrayView(field->Base(), field->key.type, schema::GetElementType(field->key.type)).count;
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
	const auto field = schema::FindField(entity, className, memberName);
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
	return schema::GetNumeric<int64_t>(entity, className, memberName, element);
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
	schema::SetNumeric<int64_t>(entity, className, memberName, value, changeState, element);
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
	return schema::GetNumeric<double>(entity, className, memberName, element);
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
	schema::SetNumeric<double>(entity, className, memberName, value, changeState, element);
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
	return schema::GetArray<int64_t>(entity, className, memberName);
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
	schema::SetArray(entity, className, memberName, value, changeState);
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
	return schema::GetArray<double>(entity, className, memberName);
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
	schema::SetArray(entity, className, memberName, value, changeState);
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
	const auto field = schema::FindField(entity, className, memberName);
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
	const auto field = schema::FindWritableField(entity, className, memberName);
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
	const auto field = schema::FindField(entity, className, memberName);
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
	const auto field = schema::FindWritableField(entity, className, memberName);
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
	return schema::GetPlain<plg::vec3, sizeof(Vector)>(entity, className, memberName, element, "vector");
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
	schema::SetPlain<plg::vec3, sizeof(Vector)>(entity, className, memberName, value, changeState, element, "vector");
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
	return schema::GetPlain<plg::vec2, sizeof(Vector2D)>(entity, className, memberName, element, "vector");
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
	schema::SetPlain<plg::vec2, sizeof(Vector2D)>(entity, className, memberName, value, changeState, element, "vector");
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
	return schema::GetPlain<plg::vec4, sizeof(Vector4D)>(entity, className, memberName, element, "vector");
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
	schema::SetPlain<plg::vec4, sizeof(Vector4D)>(entity, className, memberName, value, changeState, element, "vector");
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
	const auto field = schema::FindField(entity, className, memberName);
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
	const auto field = schema::FindWritableField(entity, className, memberName);
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
	const auto field = schema::FindWritableField(entity, className, memberName, "push into");
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
	const auto field = schema::FindWritableField(entity, className, memberName, "erase from");
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
	if (const auto field = schema::FindField(entity, className, memberName)) {
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaArraySize2(entity, className, memberName); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaPtr2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchema2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchema2(entity, className, memberName, value, changeState, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaFloat2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaFloat2(entity, className, memberName, value, changeState, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaArray2(entity, className, memberName); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaArray2(entity, className, memberName, value, changeState); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaArrayFloat2(entity, className, memberName); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaArrayFloat2(entity, className, memberName, value, changeState); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaColor2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaColor2(entity, className, memberName, value, changeState, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaString2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaString2(entity, className, memberName, value, changeState, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaVector3D2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaVector3D2(entity, className, memberName, value, changeState, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaVector2D2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaVector2D2(entity, className, memberName, value, changeState, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return GetEntSchemaVector4D2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaVector4D2(entity, className, memberName, value, changeState, element); });
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
	return helpers::WithEntity(entityHandle, static_cast<int>(INVALID_EHANDLE_INDEX), [&](CEntityInstance* entity) { return GetEntSchemaEnt2(entity, className, memberName, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return SetEntSchemaEnt2(entity, className, memberName, value, changeState, element); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return PushEntSchemaEnt2(entity, className, memberName, value, changeState); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return EraseEntSchemaEnt2(entity, className, memberName, element, changeState); });
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
	return helpers::WithEntity(entityHandle, [&](CEntityInstance* entity) { return NetworkStateChanged2(entity, className, memberName); });
}

//
//
//
//
//
//

PLUGIFY_WARN_POP()
