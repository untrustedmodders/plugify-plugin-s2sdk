#include <tier1/keyvalues3.h>
#include <core/sdk/utils.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif
// ============================================================================
// Constructors and Destructor - Implementation
// ============================================================================

/**
 * @brief Creates a new KeyValues3 object with specified type and subtype
 * @param type The KV3 type enumeration value
 * @param subtype The KV3 subtype enumeration value
 * @return Pointer to the newly created KeyValues3 object
 */
extern "C" PLUGIN_API KeyValues3* Kv3Create(int32_t type, int32_t subtype) {
    return new KeyValues3(static_cast<KV3TypeEx_t>(type), static_cast<KV3SubType_t>(subtype));
}

/**
 * @brief Creates a new KeyValues3 object with cluster element, type, and subtype
 * @param cluster_elem The cluster element index
 * @param type The KV3 type enumeration value
 * @param subtype The KV3 subtype enumeration value
 * @return Pointer to the newly created KeyValues3 object
 */
extern "C" PLUGIN_API KeyValues3* Kv3CreateWithCluster(int32_t cluster_elem, int32_t type, int32_t subtype) {
    return new KeyValues3(cluster_elem, static_cast<KV3TypeEx_t>(type), static_cast<KV3SubType_t>(subtype));
}

/**
 * @brief Creates a copy of an existing KeyValues3 object
 * @param other Pointer to the KeyValues3 object to copy
 * @return Pointer to the newly created copy, or nullptr if other is null
 */
extern "C" PLUGIN_API KeyValues3* Kv3CreateCopy(const KeyValues3* other) {
    if (!other) return nullptr;
    return new KeyValues3(*other);
}

/**
 * @brief Destroys a KeyValues3 object and frees its memory
 * @param kv Pointer to the KeyValues3 object to destroy
 */
extern "C" PLUGIN_API void Kv3Destroy(KeyValues3* kv) {
    delete kv;
}

// ============================================================================
// Copy Operations - Implementation
// ============================================================================

/**
 * @brief Copies data from another KeyValues3 object
 * @param kv Pointer to the destination KeyValues3 object
 * @param other Pointer to the source KeyValues3 object
 */
extern "C" PLUGIN_API void Kv3CopyFrom(KeyValues3* kv, const KeyValues3* other) {
    if (kv && other) kv->CopyFrom(*other);
}

/**
 * @brief Overlays keys from another KeyValues3 object
 * @param kv Pointer to the destination KeyValues3 object
 * @param other Pointer to the source KeyValues3 object
 * @param depth Whether to perform a deep overlay
 */
extern "C" PLUGIN_API void Kv3OverlayKeysFrom(KeyValues3* kv, const KeyValues3* other, bool depth) {
    if (kv && other) kv->OverlayKeysFrom(*other, depth);
}

// ============================================================================
// Context and Metadata - Implementation
// ============================================================================

/**
 * @brief Gets the context associated with a KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @return Pointer to the CKeyValues3Context, or nullptr if kv is null
 */
extern "C" PLUGIN_API CKeyValues3Context* Kv3GetContext(const KeyValues3* kv) {
    return kv ? kv->GetContext() : nullptr;
}

/**
 * @brief Gets the metadata associated with a KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param ppCtx Pointer to store the context pointer
 * @return Pointer to the KV3MetaData_t structure, or nullptr if kv is null
 */
extern "C" PLUGIN_API KV3MetaData_t* Kv3GetMetaData(const KeyValues3* kv, CKeyValues3Context** ppCtx) {
    return kv ? kv->GetMetaData(ppCtx) : nullptr;
}

// ============================================================================
// Flags - Implementation
// ============================================================================

/**
 * @brief Checks if a specific flag is set
 * @param kv Pointer to the KeyValues3 object
 * @param flag The flag to check
 * @return true if the flag is set, false otherwise
 */
extern "C" PLUGIN_API bool Kv3HasFlag(const KeyValues3* kv, uint8_t flag) {
    return kv ? kv->HasFlag(static_cast<KeyValues3Flag_t>(flag)) : false;
}

/**
 * @brief Checks if any flags are set
 * @param kv Pointer to the KeyValues3 object
 * @return true if any flags are set, false otherwise
 */
extern "C" PLUGIN_API bool Kv3HasAnyFlags(const KeyValues3* kv) {
    return kv ? kv->HasAnyFlags() : false;
}

/**
 * @brief Gets all flags as a bitmask
 * @param kv Pointer to the KeyValues3 object
 * @return Bitmask of all flags, or 0 if kv is null
 */
extern "C" PLUGIN_API uint8_t Kv3GetAllFlags(const KeyValues3* kv) {
    return kv ? static_cast<uint8_t>(kv->GetAllFlags()) : 0;
}

/**
 * @brief Sets all flags from a bitmask
 * @param kv Pointer to the KeyValues3 object
 * @param flags Bitmask of flags to set
 */
extern "C" PLUGIN_API void Kv3SetAllFlags(KeyValues3* kv, uint8_t flags) {
    if (kv) kv->SetAllFlags(static_cast<KeyValues3Flag_t>(flags));
}

/**
 * @brief Sets or clears a specific flag
 * @param kv Pointer to the KeyValues3 object
 * @param flag The flag to modify
 * @param state true to set the flag, false to clear it
 */
extern "C" PLUGIN_API void Kv3SetFlag(KeyValues3* kv, uint8_t flag, bool state) {
    if (kv) kv->SetFlag(static_cast<KeyValues3Flag_t>(flag), state);
}

// ============================================================================
// Type Information - Implementation
// ============================================================================

/**
 * @brief Gets the basic type of the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @return The type enumeration value, or 0 if kv is null
 */
extern "C" PLUGIN_API uint8_t Kv3GetType(const KeyValues3* kv) {
    return kv ? static_cast<uint8_t>(kv->GetType()) : 0;
}

/**
 * @brief Gets the extended type of the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @return The extended type enumeration value, or 0 if kv is null
 */
extern "C" PLUGIN_API uint8_t Kv3GetTypeEx(const KeyValues3* kv) {
    return kv ? static_cast<uint8_t>(kv->GetTypeEx()) : 0;
}

/**
 * @brief Gets the subtype of the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @return The subtype enumeration value, or 0 if kv is null
 */
extern "C" PLUGIN_API uint8_t Kv3GetSubType(const KeyValues3* kv) {
    return kv ? static_cast<uint8_t>(kv->GetSubType()) : 0;
}

/**
 * @brief Checks if the object has invalid member names
 * @param kv Pointer to the KeyValues3 object
 * @return true if invalid member names exist, false otherwise
 */
extern "C" PLUGIN_API bool Kv3HasInvalidMemberNames(const KeyValues3* kv) {
    return kv ? kv->HasInvalidMemberNames() : false;
}

/**
 * @brief Sets the invalid member names flag
 * @param kv Pointer to the KeyValues3 object
 * @param bValue true to mark as having invalid member names, false otherwise
 */
extern "C" PLUGIN_API void Kv3SetHasInvalidMemberNames(KeyValues3* kv, bool bValue) {
    if (kv) kv->SetHasInvalidMemberNames(bValue);
}

/**
 * @brief Gets the type as a string representation
 * @param kv Pointer to the KeyValues3 object
 * @return String representation of the type, or empty string if kv is null
 */
extern "C" PLUGIN_API plg::string Kv3GetTypeAsString(const KeyValues3* kv) {
    if (!kv) return plg::string();
    const char* str = kv->GetTypeAsString();
    return plg::string(str ? str : "");
}

/**
 * @brief Gets the subtype as a string representation
 * @param kv Pointer to the KeyValues3 object
 * @return String representation of the subtype, or empty string if kv is null
 */
extern "C" PLUGIN_API plg::string Kv3GetSubTypeAsString(const KeyValues3* kv) {
    if (!kv) return plg::string();
    const char* str = kv->GetSubTypeAsString();
    return plg::string(str ? str : "");
}

/**
 * @brief Converts the KeyValues3 object to a string representation
 * @param kv Pointer to the KeyValues3 object
 * @param flags Formatting flags for the string conversion
 * @return String representation of the object, or empty string if kv is null
 */
extern "C" PLUGIN_API plg::string Kv3ToString(const KeyValues3* kv, uint32_t flags) {
    if (!kv) return plg::string();
    CBufferString buff;
    const char* str = kv->ToString(buff, flags);
    return plg::string(str ? str : "");
}

// ============================================================================
// Type Checks - Implementation
// ============================================================================

/**
 * @brief Checks if the KeyValues3 object is null
 * @param kv Pointer to the KeyValues3 object
 * @return true if the object is null or the pointer is null, false otherwise
 */
extern "C" PLUGIN_API bool Kv3IsNull(const KeyValues3* kv) {
    return kv ? kv->IsNull() : true;
}

/**
 * @brief Sets the KeyValues3 object to null
 * @param kv Pointer to the KeyValues3 object
 */
extern "C" PLUGIN_API void Kv3SetToNull(KeyValues3* kv) {
    if (kv) kv->SetToNull();
}

/**
 * @brief Checks if the KeyValues3 object is an array
 * @param kv Pointer to the KeyValues3 object
 * @return true if the object is an array, false otherwise
 */
extern "C" PLUGIN_API bool Kv3IsArray(const KeyValues3* kv) {
    return kv ? kv->IsArray() : false;
}

/**
 * @brief Checks if the KeyValues3 object is a KV3 array
 * @param kv Pointer to the KeyValues3 object
 * @return true if the object is a KV3 array, false otherwise
 */
extern "C" PLUGIN_API bool Kv3IsKV3Array(const KeyValues3* kv) {
    return kv ? kv->IsKV3Array() : false;
}

/**
 * @brief Checks if the KeyValues3 object is a table
 * @param kv Pointer to the KeyValues3 object
 * @return true if the object is a table, false otherwise
 */
extern "C" PLUGIN_API bool Kv3IsTable(const KeyValues3* kv) {
    return kv ? kv->IsTable() : false;
}

/**
 * @brief Checks if the KeyValues3 object is a string
 * @param kv Pointer to the KeyValues3 object
 * @return true if the object is a string, false otherwise
 */
extern "C" PLUGIN_API bool Kv3IsString(const KeyValues3* kv) {
    return kv ? kv->IsString() : false;
}

// ============================================================================
// Basic Type Getters - Implementation
// ============================================================================

/**
 * @brief Gets the boolean value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return Boolean value or defaultValue
 */
extern "C" PLUGIN_API bool Kv3GetBool(const KeyValues3* kv, bool defaultValue) {
    return kv ? kv->GetBool(defaultValue) : defaultValue;
}

/**
 * @brief Gets the char value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return Char value or defaultValue
 */
extern "C" PLUGIN_API char Kv3GetChar(const KeyValues3* kv, char defaultValue) {
    return kv ? kv->GetChar(defaultValue) : defaultValue;
}

/**
 * @brief Gets the 32-bit Unicode character value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return 32-bit Unicode character value or defaultValue
 */
extern "C" PLUGIN_API uint32_t Kv3GetUChar32(const KeyValues3* kv, uint32_t defaultValue) {
    return kv ? kv->GetUChar32(defaultValue) : defaultValue;
}

/**
 * @brief Gets the signed 8-bit integer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return int8_t value or defaultValue
 */
extern "C" PLUGIN_API int8_t Kv3GetInt8(const KeyValues3* kv, int8_t defaultValue) {
    return kv ? kv->GetInt8(defaultValue) : defaultValue;
}

/**
 * @brief Gets the unsigned 8-bit integer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return uint8_t value or defaultValue
 */
extern "C" PLUGIN_API uint8_t Kv3GetUInt8(const KeyValues3* kv, uint8_t defaultValue) {
    return kv ? kv->GetUInt8(defaultValue) : defaultValue;
}

/**
 * @brief Gets the signed 16-bit integer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return int16_t value or defaultValue
 */
extern "C" PLUGIN_API int16_t Kv3GetShort(const KeyValues3* kv, int16_t defaultValue) {
    return kv ? kv->GetShort(defaultValue) : defaultValue;
}

/**
 * @brief Gets the unsigned 16-bit integer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return uint16_t value or defaultValue
 */
extern "C" PLUGIN_API uint16_t Kv3GetUShort(const KeyValues3* kv, uint16_t defaultValue) {
    return kv ? kv->GetUShort(defaultValue) : defaultValue;
}

/**
 * @brief Gets the signed 32-bit integer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return int32_t value or defaultValue
 */
extern "C" PLUGIN_API int32_t Kv3GetInt(const KeyValues3* kv, int32_t defaultValue) {
    return kv ? kv->GetInt(defaultValue) : defaultValue;
}

/**
 * @brief Gets the unsigned 32-bit integer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return uint32_t value or defaultValue
 */
extern "C" PLUGIN_API uint32_t Kv3GetUInt(const KeyValues3* kv, uint32_t defaultValue) {
    return kv ? kv->GetUInt(defaultValue) : defaultValue;
}

/**
 * @brief Gets the signed 64-bit integer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return int64_t value or defaultValue
 */
extern "C" PLUGIN_API int64_t Kv3GetInt64(const KeyValues3* kv, int64_t defaultValue) {
    return kv ? kv->GetInt64(defaultValue) : defaultValue;
}

/**
 * @brief Gets the unsigned 64-bit integer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return uint64_t value or defaultValue
 */
extern "C" PLUGIN_API uint64_t Kv3GetUInt64(const KeyValues3* kv, uint64_t defaultValue) {
    return kv ? kv->GetUInt64(defaultValue) : defaultValue;
}

/**
 * @brief Gets the float value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return Float value or defaultValue
 */
extern "C" PLUGIN_API float Kv3GetFloat(const KeyValues3* kv, float defaultValue) {
    return kv ? kv->GetFloat(defaultValue) : defaultValue;
}

/**
 * @brief Gets the double value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null or conversion fails
 * @return Double value or defaultValue
 */
extern "C" PLUGIN_API double Kv3GetDouble(const KeyValues3* kv, double defaultValue) {
    return kv ? kv->GetDouble(defaultValue) : defaultValue;
}

// ============================================================================
// Basic Type Setters - Implementation
// ============================================================================

/**
 * @brief Sets the KeyValues3 object to a boolean value
 * @param kv Pointer to the KeyValues3 object
 * @param value Boolean value to set
 */
extern "C" PLUGIN_API void Kv3SetBool(KeyValues3* kv, bool value) {
    if (kv) kv->SetBool(value);
}

/**
 * @brief Sets the KeyValues3 object to a char value
 * @param kv Pointer to the KeyValues3 object
 * @param value Char value to set
 */
extern "C" PLUGIN_API void Kv3SetChar(KeyValues3* kv, char value) {
    if (kv) kv->SetChar(value);
}

/**
 * @brief Sets the KeyValues3 object to a 32-bit Unicode character value
 * @param kv Pointer to the KeyValues3 object
 * @param value 32-bit Unicode character value to set
 */
extern "C" PLUGIN_API void Kv3SetUChar32(KeyValues3* kv, uint32_t value) {
    if (kv) kv->SetUChar32(value);
}

/**
 * @brief Sets the KeyValues3 object to a signed 8-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param value int8_t value to set
 */
extern "C" PLUGIN_API void Kv3SetInt8(KeyValues3* kv, int8_t value) {
    if (kv) kv->SetInt8(value);
}

/**
 * @brief Sets the KeyValues3 object to an unsigned 8-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param value uint8_t value to set
 */
extern "C" PLUGIN_API void Kv3SetUInt8(KeyValues3* kv, uint8_t value) {
    if (kv) kv->SetUInt8(value);
}

/**
 * @brief Sets the KeyValues3 object to a signed 16-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param value int16_t value to set
 */
extern "C" PLUGIN_API void Kv3SetShort(KeyValues3* kv, int16_t value) {
    if (kv) kv->SetShort(value);
}

/**
 * @brief Sets the KeyValues3 object to an unsigned 16-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param value uint16_t value to set
 */
extern "C" PLUGIN_API void Kv3SetUShort(KeyValues3* kv, uint16_t value) {
    if (kv) kv->SetUShort(value);
}

/**
 * @brief Sets the KeyValues3 object to a signed 32-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param value int32_t value to set
 */
extern "C" PLUGIN_API void Kv3SetInt(KeyValues3* kv, int32_t value) {
    if (kv) kv->SetInt(value);
}

/**
 * @brief Sets the KeyValues3 object to an unsigned 32-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param value uint32_t value to set
 */
extern "C" PLUGIN_API void Kv3SetUInt(KeyValues3* kv, uint32_t value) {
    if (kv) kv->SetUInt(value);
}

/**
 * @brief Sets the KeyValues3 object to a signed 64-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param value int64_t value to set
 */
extern "C" PLUGIN_API void Kv3SetInt64(KeyValues3* kv, int64_t value) {
    if (kv) kv->SetInt64(value);
}

/**
 * @brief Sets the KeyValues3 object to an unsigned 64-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param value uint64_t value to set
 */
extern "C" PLUGIN_API void Kv3SetUInt64(KeyValues3* kv, uint64_t value) {
    if (kv) kv->SetUInt64(value);
}

/**
 * @brief Sets the KeyValues3 object to a float value
 * @param kv Pointer to the KeyValues3 object
 * @param value Float value to set
 */
extern "C" PLUGIN_API void Kv3SetFloat(KeyValues3* kv, float value) {
    if (kv) kv->SetFloat(value);
}

/**
 * @brief Sets the KeyValues3 object to a double value
 * @param kv Pointer to the KeyValues3 object
 * @param value Double value to set
 */
extern "C" PLUGIN_API void Kv3SetDouble(KeyValues3* kv, double value) {
    if (kv) kv->SetDouble(value);
}

// ============================================================================
// Pointer Handling - Implementation
// ============================================================================

/**
 * @brief Gets the pointer value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default value to return if kv is null
 * @return Pointer value as uintptr_t or defaultValue
 */
extern "C" PLUGIN_API uintptr_t Kv3GetPointer(const KeyValues3* kv, uintptr_t defaultValue) {
    if (!kv) return defaultValue;
    void* ptr = kv->GetPointer(reinterpret_cast<void*>(defaultValue));
    return reinterpret_cast<uintptr_t>(ptr);
}

/**
 * @brief Sets the KeyValues3 object to a pointer value
 * @param kv Pointer to the KeyValues3 object
 * @param ptr Pointer value as uintptr_t to set
 */
extern "C" PLUGIN_API void Kv3SetPointer(KeyValues3* kv, uintptr_t ptr) {
    if (kv) kv->SetPointer(reinterpret_cast<void*>(ptr));
}

// ============================================================================
// String Token Handling - Implementation
// ============================================================================

/**
 * @brief Gets the string token value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default token value to return if kv is null
 * @return String token hash code or defaultValue
 */
extern "C" PLUGIN_API uint32_t Kv3GetStringToken(const KeyValues3* kv, uint32_t defaultValue) {
    if (!kv) return defaultValue;
    CUtlStringToken token(defaultValue);
    return kv->GetStringToken(token).GetHashCode();
}

/**
 * @brief Sets the KeyValues3 object to a string token value
 * @param kv Pointer to the KeyValues3 object
 * @param token String token hash code to set
 */
extern "C" PLUGIN_API void Kv3SetStringToken(KeyValues3* kv, uint32_t token) {
    if (kv) kv->SetStringToken(CUtlStringToken(token));
}

// ============================================================================
// Entity Handle - Implementation
// ============================================================================

/**
 * @brief Gets the entity handle value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default entity handle value to return if kv is null
 * @return Entity handle as int32_t or defaultValue
 */
extern "C" PLUGIN_API int32_t Kv3GetEHandle(const KeyValues3* kv, int32_t defaultValue) {
    if (!kv) return defaultValue;
    CEntityHandle handle(defaultValue);
    return kv->GetEHandle(handle).ToInt();
}

/**
 * @brief Sets the KeyValues3 object to an entity handle value
 * @param kv Pointer to the KeyValues3 object
 * @param ehandle Entity handle value to set
 */
extern "C" PLUGIN_API void Kv3SetEHandle(KeyValues3* kv, int32_t ehandle) {
    if (kv) kv->SetEHandle(CEntityHandle(ehandle));
}

// ============================================================================
// String Operations - Implementation
// ============================================================================

/**
 * @brief Gets the string value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default string to return if kv is null or value is empty
 * @return String value or defaultValue
 */
extern "C" PLUGIN_API plg::string Kv3GetString(const KeyValues3* kv, const plg::string& defaultValue) {
    if (!kv) return defaultValue;
    const char* str = kv->GetString(defaultValue.c_str());
    return plg::string(str ? str : "");
}

/**
 * @brief Sets the KeyValues3 object to a string value (copies the string)
 * @param kv Pointer to the KeyValues3 object
 * @param str String value to set
 * @param subtype String subtype enumeration value
 */
extern "C" PLUGIN_API void Kv3SetString(KeyValues3* kv, const plg::string& str, uint8_t subtype) {
    if (kv) kv->SetString(str.c_str(), static_cast<KV3SubType_t>(subtype));
}

/**
 * @brief Sets the KeyValues3 object to an external string value (does not copy)
 * @param kv Pointer to the KeyValues3 object
 * @param str External string value to reference
 * @param subtype String subtype enumeration value
 */
extern "C" PLUGIN_API void Kv3SetStringExternal(KeyValues3* kv, const plg::string& str, uint8_t subtype) {
    if (kv) kv->SetStringExternal(str.c_str(), static_cast<KV3SubType_t>(subtype));
}

// ============================================================================
// Binary Blob Operations - Implementation
// ============================================================================

/**
 * @brief Gets the binary blob from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @return Vector containing the binary blob data, or empty vector if kv is null
 */
extern "C" PLUGIN_API plg::vector<uint8_t> Kv3GetBinaryBlob(const KeyValues3* kv) {
    plg::vector<uint8_t> result;
    if (!kv) return result;

    const byte* blob = kv->GetBinaryBlob();
    int size = kv->GetBinaryBlobSize();

    if (blob && size > 0) {
        result.assign(blob, blob + size);
    }

    return result;
}

/**
 * @brief Gets the size of the binary blob in the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @return Size of the binary blob in bytes, or 0 if kv is null
 */
extern "C" PLUGIN_API int32_t Kv3GetBinaryBlobSize(const KeyValues3* kv) {
    return kv ? kv->GetBinaryBlobSize() : 0;
}

/**
 * @brief Sets the KeyValues3 object to a binary blob (copies the data)
 * @param kv Pointer to the KeyValues3 object
 * @param blob Vector containing the binary blob data
 */
extern "C" PLUGIN_API void Kv3SetToBinaryBlob(KeyValues3* kv, const plg::vector<uint8_t>& blob) {
    if (kv && !blob.empty()) {
        kv->SetToBinaryBlob(blob.data(), static_cast<int>(blob.size()));
    }
}

/**
 * @brief Sets the KeyValues3 object to an external binary blob (does not copy)
 * @param kv Pointer to the KeyValues3 object
 * @param blob Vector containing the external binary blob data
 * @param free_mem Whether to free the memory when the object is destroyed
 */
extern "C" PLUGIN_API void Kv3SetToBinaryBlobExternal(KeyValues3* kv, const plg::vector<uint8_t>& blob, bool free_mem) {
    if (kv && !blob.empty()) {
        kv->SetToBinaryBlobExternal(blob.data(), static_cast<int>(blob.size()), free_mem);
    }
}

// ============================================================================
// Color Operations - Implementation
// ============================================================================

/**
 * @brief Gets the color value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default color value to return if kv is null
 * @return Color value as int32_t or defaultValue
 */
extern "C" PLUGIN_API int32_t Kv3GetColor(const KeyValues3* kv, int32_t defaultValue) {
    if (!kv) return defaultValue;
    Color defaultColor = *reinterpret_cast<const Color*>(&defaultValue);
    Color result = kv->GetColor(defaultColor);
    return *reinterpret_cast<const uint32_t*>(&result);
}

/**
 * @brief Sets the KeyValues3 object to a color value
 * @param kv Pointer to the KeyValues3 object
 * @param color Color value as int32_t to set
 */
extern "C" PLUGIN_API void Kv3SetColor(KeyValues3* kv, int32_t color) {
    if (kv) kv->SetColor(*reinterpret_cast<const Color*>(&color));
}

// ============================================================================
// Vector/Matrix Getters - Implementation
// ============================================================================

/**
 * @brief Gets the 3D vector value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default vector to return if kv is null
 * @return 3D vector or defaultValue
 */
extern "C" PLUGIN_API plg::vec3 Kv3GetVector(const KeyValues3* kv, const plg::vec3& defaultValue) {
    if (!kv) return defaultValue;
    // Vector is binary compatible with plg::vec3
    Vector result = kv->GetVector(*reinterpret_cast<const Vector*>(&defaultValue));
    return *reinterpret_cast<const plg::vec3*>(&result);
}

/**
 * @brief Gets the 2D vector value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default 2D vector to return if kv is null
 * @return 2D vector or defaultValue
 */
extern "C" PLUGIN_API plg::vec2 Kv3GetVector2D(const KeyValues3* kv, const plg::vec2& defaultValue) {
    if (!kv) return defaultValue;
    // Vector2D is binary compatible with plg::vec2
    Vector2D result = kv->GetVector2D(*reinterpret_cast<const Vector2D*>(&defaultValue));
    return *reinterpret_cast<const plg::vec2*>(&result);
}

/**
 * @brief Gets the 4D vector value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default 4D vector to return if kv is null
 * @return 4D vector or defaultValue
 */
extern "C" PLUGIN_API plg::vec4 Kv3GetVector4D(const KeyValues3* kv, const plg::vec4& defaultValue) {
    if (!kv) return defaultValue;
    // Vector4D is binary compatible with plg::vec4
    Vector4D result = kv->GetVector4D(*reinterpret_cast<const Vector4D*>(&defaultValue));
    return *reinterpret_cast<const plg::vec4*>(&result);
}

/**
 * @brief Gets the quaternion value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default quaternion to return if kv is null
 * @return Quaternion as vec4 or defaultValue
 */
extern "C" PLUGIN_API plg::vec4 Kv3GetQuaternion(const KeyValues3* kv, const plg::vec4& defaultValue) {
    if (!kv) return defaultValue;
    // Quaternion is binary compatible with plg::vec4
    Quaternion result = kv->GetQuaternion(*reinterpret_cast<const Quaternion*>(&defaultValue));
    return *reinterpret_cast<const plg::vec4*>(&result);
}

/**
 * @brief Gets the angle (QAngle) value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default angle to return if kv is null
 * @return QAngle as vec3 or defaultValue
 */
extern "C" PLUGIN_API plg::vec3 Kv3GetQAngle(const KeyValues3* kv, const plg::vec3& defaultValue) {
    if (!kv) return defaultValue;
    // QAngle is binary compatible with plg::vec3
    QAngle result = kv->GetQAngle(*reinterpret_cast<const QAngle*>(&defaultValue));
    return *reinterpret_cast<const plg::vec3*>(&result);
}

/**
 * @brief Gets the 3x4 matrix value from the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 * @param defaultValue Default matrix to return if kv is null
 * @return 3x4 matrix as mat4x4 or defaultValue
 */
extern "C" PLUGIN_API plg::mat4x4 Kv3GetMatrix3x4(const KeyValues3* kv, const plg::mat4x4& defaultValue) {
    if (!kv) return defaultValue;
    // matrix3x4_t is binary compatible with first 3 rows of plg::mat4x4
    matrix3x4_t result = kv->GetMatrix3x4(*reinterpret_cast<const matrix3x4_t*>(&defaultValue));
    return *reinterpret_cast<const plg::mat4x4*>(&result);
}

// ============================================================================
// Vector/Matrix Setters - Implementation
// ============================================================================

/**
 * @brief Sets the KeyValues3 object to a 3D vector value
 * @param kv Pointer to the KeyValues3 object
 * @param vec 3D vector to set
 */
extern "C" PLUGIN_API void Kv3SetVector(KeyValues3* kv, const plg::vec3& vec) {
    if (kv) kv->SetVector(*reinterpret_cast<const Vector*>(&vec));
}

/**
 * @brief Sets the KeyValues3 object to a 2D vector value
 * @param kv Pointer to the KeyValues3 object
 * @param vec2d 2D vector to set
 */
extern "C" PLUGIN_API void Kv3SetVector2D(KeyValues3* kv, const plg::vec2& vec2d) {
    if (kv) kv->SetVector2D(*reinterpret_cast<const Vector2D*>(&vec2d));
}

/**
 * @brief Sets the KeyValues3 object to a 4D vector value
 * @param kv Pointer to the KeyValues3 object
 * @param vec4d 4D vector to set
 */
extern "C" PLUGIN_API void Kv3SetVector4D(KeyValues3* kv, const plg::vec4& vec4d) {
    if (kv) kv->SetVector4D(*reinterpret_cast<const Vector4D*>(&vec4d));
}

/**
 * @brief Sets the KeyValues3 object to a quaternion value
 * @param kv Pointer to the KeyValues3 object
 * @param quat Quaternion to set (as vec4)
 */
extern "C" PLUGIN_API void Kv3SetQuaternion(KeyValues3* kv, const plg::vec4& quat) {
    if (kv) kv->SetQuaternion(*reinterpret_cast<const Quaternion*>(&quat));
}

/**
 * @brief Sets the KeyValues3 object to an angle (QAngle) value
 * @param kv Pointer to the KeyValues3 object
 * @param ang QAngle to set (as vec3)
 */
extern "C" PLUGIN_API void Kv3SetQAngle(KeyValues3* kv, const plg::vec3& ang) {
    if (kv) kv->SetQAngle(*reinterpret_cast<const QAngle*>(&ang));
}

/**
 * @brief Sets the KeyValues3 object to a 3x4 matrix value
 * @param kv Pointer to the KeyValues3 object
 * @param matrix 3x4 matrix to set (as mat4x4)
 */
extern "C" PLUGIN_API void Kv3SetMatrix3x4(KeyValues3* kv, const plg::mat4x4& matrix) {
    if (kv) kv->SetMatrix3x4(*reinterpret_cast<const matrix3x4_t*>(&matrix));
}

// ============================================================================
// Array Operations - Implementation
// ============================================================================

/**
 * @brief Gets the number of elements in the array
 * @param kv Pointer to the KeyValues3 object
 * @return Number of array elements, or 0 if kv is null or not an array
 */
extern "C" PLUGIN_API int32_t Kv3GetArrayElementCount(const KeyValues3* kv) {
    return kv ? kv->GetArrayElementCount() : 0;
}

/**
 * @brief Sets the number of elements in the array
 * @param kv Pointer to the KeyValues3 object
 * @param count Number of elements to set
 * @param type Type of array elements
 * @param subtype Subtype of array elements
 */
extern "C" PLUGIN_API void Kv3SetArrayElementCount(KeyValues3* kv, int32_t count, uint8_t type, uint8_t subtype) {
    if (kv) kv->SetArrayElementCount(count, static_cast<KV3TypeEx_t>(type), static_cast<KV3SubType_t>(subtype));
}

/**
 * @brief Sets the KeyValues3 object to an empty KV3 array
 * @param kv Pointer to the KeyValues3 object
 */
extern "C" PLUGIN_API void Kv3SetToEmptyKV3Array(KeyValues3* kv) {
    if (kv) kv->SetToEmptyKV3Array();
}

/**
 * @brief Gets an array element at the specified index
 * @param kv Pointer to the KeyValues3 object
 * @param elem Index of the element to get
 * @return Pointer to the element KeyValues3 object, or nullptr if invalid
 */
extern "C" PLUGIN_API KeyValues3* Kv3GetArrayElement(KeyValues3* kv, int32_t elem) {
    return kv ? kv->GetArrayElement(elem) : nullptr;
}

/**
 * @brief Inserts a new element before the specified index
 * @param kv Pointer to the KeyValues3 object
 * @param elem Index before which to insert
 * @return Pointer to the newly inserted element, or nullptr if invalid
 */
extern "C" PLUGIN_API KeyValues3* Kv3ArrayInsertElementBefore(KeyValues3* kv, int32_t elem) {
    return kv ? kv->ArrayInsertElementBefore(elem) : nullptr;
}

/**
 * @brief Inserts a new element after the specified index
 * @param kv Pointer to the KeyValues3 object
 * @param elem Index after which to insert
 * @return Pointer to the newly inserted element, or nullptr if invalid
 */
extern "C" PLUGIN_API KeyValues3* Kv3ArrayInsertElementAfter(KeyValues3* kv, int32_t elem) {
    return kv ? kv->ArrayInsertElementAfter(elem) : nullptr;
}

/**
 * @brief Adds a new element to the end of the array
 * @param kv Pointer to the KeyValues3 object
 * @return Pointer to the newly added element, or nullptr if invalid
 */
extern "C" PLUGIN_API KeyValues3* Kv3ArrayAddElementToTail(KeyValues3* kv) {
    return kv ? kv->ArrayAddElementToTail() : nullptr;
}

/**
 * @brief Swaps two array elements
 * @param kv Pointer to the KeyValues3 object
 * @param idx1 Index of the first element
 * @param idx2 Index of the second element
 */
extern "C" PLUGIN_API void Kv3ArraySwapItems(KeyValues3* kv, int32_t idx1, int32_t idx2) {
    if (kv) kv->ArraySwapItems(idx1, idx2);
}

/**
 * @brief Removes an element from the array
 * @param kv Pointer to the KeyValues3 object
 * @param elem Index of the element to remove
 */
extern "C" PLUGIN_API void Kv3ArrayRemoveElement(KeyValues3* kv, int32_t elem) {
    if (kv) kv->ArrayRemoveElement(elem);
}

// ============================================================================
// Table Operations - Implementation
// ============================================================================

/**
 * @brief Sets the KeyValues3 object to an empty table
 * @param kv Pointer to the KeyValues3 object
 */
extern "C" PLUGIN_API void Kv3SetToEmptyTable(KeyValues3* kv) {
    if (kv) kv->SetToEmptyTable();
}

/**
 * @brief Gets the number of members in the table
 * @param kv Pointer to the KeyValues3 object
 * @return Number of table members, or 0 if kv is null or not a table
 */
extern "C" PLUGIN_API int32_t Kv3GetMemberCount(const KeyValues3* kv) {
    return kv ? kv->GetMemberCount() : 0;
}

/**
 * @brief Checks if a member with the specified name exists
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member to check
 * @return true if the member exists, false otherwise
 */
extern "C" PLUGIN_API bool Kv3HasMember(const KeyValues3* kv, const plg::string& name) {
    return kv ? kv->FindMember(CKV3MemberName::Make(name)) != nullptr : false;
}

/**
 * @brief Finds a member by name
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member to find
 * @return Pointer to the member KeyValues3 object, or nullptr if not found
 */
extern "C" PLUGIN_API KeyValues3* Kv3FindMember(KeyValues3* kv, const plg::string& name) {
    return kv ? kv->FindMember(CKV3MemberName::Make(name)) : nullptr;
}

/**
 * @brief Finds a member by name, or creates it if it doesn't exist
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member to find or create
 * @return Pointer to the member KeyValues3 object, or nullptr if kv is null
 */
extern "C" PLUGIN_API KeyValues3* Kv3FindOrCreateMember(KeyValues3* kv, const plg::string& name) {
    return kv ? kv->FindOrCreateMember(CKV3MemberName::Make(name)) : nullptr;
}

/**
 * @brief Removes a member from the table
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member to remove
 * @return true if the member was removed, false otherwise
 */
extern "C" PLUGIN_API bool Kv3RemoveMember(KeyValues3* kv, const plg::string& name) {
    return kv ? kv->RemoveMember(CKV3MemberName::Make(name)) : false;
}

/**
 * @brief Gets the name of a member at the specified index
 * @param kv Pointer to the KeyValues3 object
 * @param index Index of the member
 * @return Name of the member, or empty string if invalid
 */
extern "C" PLUGIN_API plg::string Kv3GetMemberName(const KeyValues3* kv, int32_t index) {
    if (!kv) return plg::string();
    const char* name = kv->GetMemberName(index);
    return plg::string(name ? name : "");
}

/**
 * @brief Gets a member by index
 * @param kv Pointer to the KeyValues3 object
 * @param index Index of the member to get
 * @return Pointer to the member KeyValues3 object, or nullptr if invalid
 */
extern "C" PLUGIN_API KeyValues3* Kv3GetMemberByIndex(KeyValues3* kv, int32_t index) {
    return kv ? kv->GetMember(index) : nullptr;
}

// ============================================================================
// Member Value Getters - Implementation
// ============================================================================

/**
 * @brief Gets a boolean value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return Boolean value or defaultValue
 */
extern "C" PLUGIN_API bool Kv3GetMemberBool(const KeyValues3* kv, const plg::string& name, bool defaultValue) {
    return kv ? kv->GetMemberBool(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a char value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return Char value or defaultValue
 */
extern "C" PLUGIN_API char Kv3GetMemberChar(const KeyValues3* kv, const plg::string& name, char defaultValue) {
    return kv ? kv->GetMemberChar(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a 32-bit Unicode character value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return 32-bit Unicode character value or defaultValue
 */
extern "C" PLUGIN_API uint32_t Kv3GetMemberUChar32(const KeyValues3* kv, const plg::string& name, uint32_t defaultValue) {
    return kv ? kv->GetMemberUChar32(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a signed 8-bit integer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return int8_t value or defaultValue
 */
extern "C" PLUGIN_API int8_t Kv3GetMemberInt8(const KeyValues3* kv, const plg::string& name, int8_t defaultValue) {
    return kv ? kv->GetMemberInt8(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets an unsigned 8-bit integer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return uint8_t value or defaultValue
 */
extern "C" PLUGIN_API uint8_t Kv3GetMemberUInt8(const KeyValues3* kv, const plg::string& name, uint8_t defaultValue) {
    return kv ? kv->GetMemberUInt8(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a signed 16-bit integer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return int16_t value or defaultValue
 */
extern "C" PLUGIN_API int16_t Kv3GetMemberShort(const KeyValues3* kv, const plg::string& name, int16_t defaultValue) {
    return kv ? kv->GetMemberShort(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets an unsigned 16-bit integer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return uint16_t value or defaultValue
 */
extern "C" PLUGIN_API uint16_t Kv3GetMemberUShort(const KeyValues3* kv, const plg::string& name, uint16_t defaultValue) {
    return kv ? kv->GetMemberUShort(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a signed 32-bit integer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return int32_t value or defaultValue
 */
extern "C" PLUGIN_API int32_t Kv3GetMemberInt(const KeyValues3* kv, const plg::string& name, int32_t defaultValue) {
    return kv ? kv->GetMemberInt(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets an unsigned 32-bit integer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return uint32_t value or defaultValue
 */
extern "C" PLUGIN_API uint32_t Kv3GetMemberUInt(const KeyValues3* kv, const plg::string& name, uint32_t defaultValue) {
    return kv ? kv->GetMemberUInt(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a signed 64-bit integer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return int64_t value or defaultValue
 */
extern "C" PLUGIN_API int64_t Kv3GetMemberInt64(const KeyValues3* kv, const plg::string& name, int64_t defaultValue) {
    return kv ? kv->GetMemberInt64(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets an unsigned 64-bit integer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return uint64_t value or defaultValue
 */
extern "C" PLUGIN_API uint64_t Kv3GetMemberUInt64(const KeyValues3* kv, const plg::string& name, uint64_t defaultValue) {
    return kv ? kv->GetMemberUInt64(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a float value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return Float value or defaultValue
 */
extern "C" PLUGIN_API float Kv3GetMemberFloat(const KeyValues3* kv, const plg::string& name, float defaultValue) {
    return kv ? kv->GetMemberFloat(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a double value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return Double value or defaultValue
 */
extern "C" PLUGIN_API double Kv3GetMemberDouble(const KeyValues3* kv, const plg::string& name, double defaultValue) {
    return kv ? kv->GetMemberDouble(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

/**
 * @brief Gets a pointer value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default value to return if member not found
 * @return Pointer value as uintptr_t or defaultValue
 */
extern "C" PLUGIN_API uintptr_t Kv3GetMemberPointer(const KeyValues3* kv, const plg::string& name, uintptr_t defaultValue) {
    if (!kv) return defaultValue;
    void* ptr = kv->GetMemberPointer(CKV3MemberName::Make(name), reinterpret_cast<void*>(defaultValue));
    return reinterpret_cast<uintptr_t>(ptr);
}

/**
 * @brief Gets a string token value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default token value to return if member not found
 * @return String token hash code or defaultValue
 */
extern "C" PLUGIN_API uint32_t Kv3GetMemberStringToken(const KeyValues3* kv, const plg::string& name, uint32_t defaultValue) {
    if (!kv) return defaultValue;
    CUtlStringToken token(defaultValue);
    return kv->GetMemberStringToken(CKV3MemberName::Make(name), token).GetHashCode();
}

/**
 * @brief Gets an entity handle value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default entity handle value to return if member not found
 * @return Entity handle as int32_t or defaultValue
 */
extern "C" PLUGIN_API int32_t Kv3GetMemberEHandle(const KeyValues3* kv, const plg::string& name, int32_t defaultValue) {
    if (!kv) return defaultValue;
    CEntityHandle handle(defaultValue);
    return kv->GetMemberEHandle(CKV3MemberName::Make(name), handle).ToInt();
}

/**
 * @brief Gets a string value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default string to return if member not found
 * @return String value or defaultValue
 */
extern "C" PLUGIN_API plg::string Kv3GetMemberString(const KeyValues3* kv, const plg::string& name, const plg::string& defaultValue) {
    if (!kv) return defaultValue;
    const char* str = kv->GetMemberString(CKV3MemberName::Make(name), defaultValue.c_str());
    return plg::string(str ? str : "");
}

/**
 * @brief Gets a color value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default color value to return if member not found
 * @return Color value as int32_t or defaultValue
 */
extern "C" PLUGIN_API int32_t Kv3GetMemberColor(const KeyValues3* kv, const plg::string& name, int32_t defaultValue) {
    if (!kv) return defaultValue;
    Color defaultColor = *reinterpret_cast<const Color*>(&defaultValue);
    Color result = kv->GetMemberColor(CKV3MemberName::Make(name), defaultColor);
    return *reinterpret_cast<const int32_t*>(&result);
}

/**
 * @brief Gets a 3D vector value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default vector to return if member not found
 * @return 3D vector or defaultValue
 */
extern "C" PLUGIN_API plg::vec3 Kv3GetMemberVector(const KeyValues3* kv, const plg::string& name, const plg::vec3& defaultValue) {
    if (!kv) return defaultValue;
    Vector result = kv->GetMemberVector(CKV3MemberName::Make(name), *reinterpret_cast<const Vector*>(&defaultValue));
    return *reinterpret_cast<const plg::vec3*>(&result);
}

/**
 * @brief Gets a 2D vector value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default 2D vector to return if member not found
 * @return 2D vector or defaultValue
 */
extern "C" PLUGIN_API plg::vec2 Kv3GetMemberVector2D(const KeyValues3* kv, const plg::string& name, const plg::vec2& defaultValue) {
    if (!kv) return defaultValue;
    Vector2D result = kv->GetMemberVector2D(CKV3MemberName::Make(name), *reinterpret_cast<const Vector2D*>(&defaultValue));
    return *reinterpret_cast<const plg::vec2*>(&result);
}

/**
 * @brief Gets a 4D vector value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default 4D vector to return if member not found
 * @return 4D vector or defaultValue
 */
extern "C" PLUGIN_API plg::vec4 Kv3GetMemberVector4D(const KeyValues3* kv, const plg::string& name, const plg::vec4& defaultValue) {
    if (!kv) return defaultValue;
    Vector4D result = kv->GetMemberVector4D(CKV3MemberName::Make(name), *reinterpret_cast<const Vector4D*>(&defaultValue));
    return *reinterpret_cast<const plg::vec4*>(&result);
}

/**
 * @brief Gets a quaternion value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default quaternion to return if member not found
 * @return Quaternion as vec4 or defaultValue
 */
extern "C" PLUGIN_API plg::vec4 Kv3GetMemberQuaternion(const KeyValues3* kv, const plg::string& name, const plg::vec4& defaultValue) {
    if (!kv) return defaultValue;
    Quaternion result = kv->GetMemberQuaternion(CKV3MemberName::Make(name), *reinterpret_cast<const Quaternion*>(&defaultValue));
    return *reinterpret_cast<const plg::vec4*>(&result);
}

/**
 * @brief Gets an angle (QAngle) value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default angle to return if member not found
 * @return QAngle as vec3 or defaultValue
 */
extern "C" PLUGIN_API plg::vec3 Kv3GetMemberQAngle(const KeyValues3* kv, const plg::string& name, const plg::vec3& defaultValue) {
    if (!kv) return defaultValue;
    QAngle result = kv->GetMemberQAngle(CKV3MemberName::Make(name), *reinterpret_cast<const QAngle*>(&defaultValue));
    return *reinterpret_cast<const plg::vec3*>(&result);
}

/**
 * @brief Gets a 3x4 matrix value from a table member
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param defaultValue Default matrix to return if member not found
 * @return 3x4 matrix as mat4x4 or defaultValue
 */
extern "C" PLUGIN_API plg::mat4x4 Kv3GetMemberMatrix3x4(const KeyValues3* kv, const plg::string& name, const plg::mat4x4& defaultValue) {
    if (!kv) return defaultValue;
    matrix3x4_t result = kv->GetMemberMatrix3x4(CKV3MemberName::Make(name), *reinterpret_cast<const matrix3x4_t*>(&defaultValue));
    return *reinterpret_cast<const plg::mat4x4*>(&result);
}

// ============================================================================
// Member Value Setters - Special - Implementation
// ============================================================================

/**
 * @brief Sets a table member to null
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 */
extern "C" PLUGIN_API void Kv3SetMemberToNull(KeyValues3* kv, const plg::string& name) {
    if (kv) kv->SetMemberToNull(CKV3MemberName::Make(name));
}

/**
 * @brief Sets a table member to an empty array
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 */
extern "C" PLUGIN_API void Kv3SetMemberToEmptyArray(KeyValues3* kv, const plg::string& name) {
    if (kv) kv->SetMemberToEmptyArray(CKV3MemberName::Make(name));
}

/**
 * @brief Sets a table member to an empty table
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 */
extern "C" PLUGIN_API void Kv3SetMemberToEmptyTable(KeyValues3* kv, const plg::string& name) {
    if (kv) kv->SetMemberToEmptyTable(CKV3MemberName::Make(name));
}

/**
 * @brief Sets a table member to a binary blob (copies the data)
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param blob Vector containing the binary blob data
 */
extern "C" PLUGIN_API void Kv3SetMemberToBinaryBlob(KeyValues3* kv, const plg::string& name, const plg::vector<uint8_t>& blob) {
    if (kv && !blob.empty()) {
        kv->SetMemberToBinaryBlob(CKV3MemberName::Make(name), blob.data(), static_cast<int>(blob.size()));
    }
}

/**
 * @brief Sets a table member to an external binary blob (does not copy)
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param blob Vector containing the external binary blob data
 * @param free_mem Whether to free the memory when the object is destroyed
 */
extern "C" PLUGIN_API void Kv3SetMemberToBinaryBlobExternal(KeyValues3* kv, const plg::string& name, const plg::vector<uint8_t>& blob, bool free_mem) {
    if (kv && !blob.empty()) {
        kv->SetMemberToBinaryBlobExternal(CKV3MemberName::Make(name), blob.data(), static_cast<int>(blob.size()), free_mem);
    }
}

/**
 * @brief Sets a table member to a copy of another KeyValues3 value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param other Pointer to the KeyValues3 object to copy
 */
extern "C" PLUGIN_API void Kv3SetMemberToCopyOfValue(KeyValues3* kv, const plg::string& name, KeyValues3* other) {
    if (kv && other) kv->SetMemberToCopyOfValue(CKV3MemberName::Make(name), other);
}

// ============================================================================
// Member Value Setters - Basic Types - Implementation
// ============================================================================

/**
 * @brief Sets a table member to a boolean value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value Boolean value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberBool(KeyValues3* kv, const plg::string& name, bool value) {
    if (kv) kv->SetMemberBool(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a char value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value Char value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberChar(KeyValues3* kv, const plg::string& name, char value) {
    if (kv) kv->SetMemberChar(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a 32-bit Unicode character value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value 32-bit Unicode character value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberUChar32(KeyValues3* kv, const plg::string& name, uint32_t value) {
    if (kv) kv->SetMemberUChar32(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a signed 8-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value int8_t value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberInt8(KeyValues3* kv, const plg::string& name, int8_t value) {
    if (kv) kv->SetMemberInt8(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to an unsigned 8-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value uint8_t value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberUInt8(KeyValues3* kv, const plg::string& name, uint8_t value) {
    if (kv) kv->SetMemberUInt8(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a signed 16-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value int16_t value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberShort(KeyValues3* kv, const plg::string& name, int16_t value) {
    if (kv) kv->SetMemberShort(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to an unsigned 16-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value uint16_t value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberUShort(KeyValues3* kv, const plg::string& name, uint16_t value) {
    if (kv) kv->SetMemberUShort(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a signed 32-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value int32_t value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberInt(KeyValues3* kv, const plg::string& name, int32_t value) {
    if (kv) kv->SetMemberInt(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to an unsigned 32-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value uint32_t value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberUInt(KeyValues3* kv, const plg::string& name, uint32_t value) {
    if (kv) kv->SetMemberUInt(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a signed 64-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value int64_t value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberInt64(KeyValues3* kv, const plg::string& name, int64_t value) {
    if (kv) kv->SetMemberInt64(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to an unsigned 64-bit integer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value uint64_t value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberUInt64(KeyValues3* kv, const plg::string& name, uint64_t value) {
    if (kv) kv->SetMemberUInt64(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a float value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value Float value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberFloat(KeyValues3* kv, const plg::string& name, float value) {
    if (kv) kv->SetMemberFloat(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a double value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param value Double value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberDouble(KeyValues3* kv, const plg::string& name, double value) {
    if (kv) kv->SetMemberDouble(CKV3MemberName::Make(name), value);
}

/**
 * @brief Sets a table member to a pointer value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param ptr Pointer value as uintptr_t to set
 */
extern "C" PLUGIN_API void Kv3SetMemberPointer(KeyValues3* kv, const plg::string& name, uintptr_t ptr) {
    if (kv) kv->SetMemberPointer(CKV3MemberName::Make(name), reinterpret_cast<void*>(ptr));
}

/**
 * @brief Sets a table member to a string token value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param token String token hash code to set
 */
extern "C" PLUGIN_API void Kv3SetMemberStringToken(KeyValues3* kv, const plg::string& name, uint32_t token) {
    if (kv) kv->SetMemberStringToken(CKV3MemberName::Make(name), CUtlStringToken(token));
}

/**
 * @brief Sets a table member to an entity handle value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param ehandle Entity handle value to set
 */
extern "C" PLUGIN_API void Kv3SetMemberEHandle(KeyValues3* kv, const plg::string& name, int32_t ehandle) {
    if (kv) kv->SetMemberEHandle(CKV3MemberName::Make(name), CEntityHandle(ehandle));
}

/**
 * @brief Sets a table member to a string value (copies the string)
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param str String value to set
 * @param subtype String subtype enumeration value
 */
extern "C" PLUGIN_API void Kv3SetMemberString(KeyValues3* kv, const plg::string& name, const plg::string& str, uint8_t subtype) {
    if (kv) kv->SetMemberString(CKV3MemberName::Make(name), str.c_str(), static_cast<KV3SubType_t>(subtype));
}

/**
 * @brief Sets a table member to an external string value (does not copy)
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param str External string value to reference
 * @param subtype String subtype enumeration value
 */
extern "C" PLUGIN_API void Kv3SetMemberStringExternal(KeyValues3* kv, const plg::string& name, const plg::string& str, uint8_t subtype) {
    if (kv) kv->SetMemberStringExternal(CKV3MemberName::Make(name), str.c_str(), static_cast<KV3SubType_t>(subtype));
}

/**
 * @brief Sets a table member to a color value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param color Color value as int32_t to set
 */
extern "C" PLUGIN_API void Kv3SetMemberColor(KeyValues3* kv, const plg::string& name, int32_t color) {
    if (kv) kv->SetMemberColor(CKV3MemberName::Make(name), *reinterpret_cast<const Color*>(&color));
}

/**
 * @brief Sets a table member to a 3D vector value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param vec 3D vector to set
 */
extern "C" PLUGIN_API void Kv3SetMemberVector(KeyValues3* kv, const plg::string& name, const plg::vec3& vec) {
    if (kv) kv->SetMemberVector(CKV3MemberName::Make(name), *reinterpret_cast<const Vector*>(&vec));
}

/**
 * @brief Sets a table member to a 2D vector value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param vec2d 2D vector to set
 */
extern "C" PLUGIN_API void Kv3SetMemberVector2D(KeyValues3* kv, const plg::string& name, const plg::vec2& vec2d) {
    if (kv) kv->SetMemberVector2D(CKV3MemberName::Make(name), *reinterpret_cast<const Vector2D*>(&vec2d));
}

/**
 * @brief Sets a table member to a 4D vector value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param vec4d 4D vector to set
 */
extern "C" PLUGIN_API void Kv3SetMemberVector4D(KeyValues3* kv, const plg::string& name, const plg::vec4& vec4d) {
    if (kv) kv->SetMemberVector4D(CKV3MemberName::Make(name), *reinterpret_cast<const Vector4D*>(&vec4d));
}

/**
 * @brief Sets a table member to a quaternion value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param quat Quaternion to set (as vec4)
 */
extern "C" PLUGIN_API void Kv3SetMemberQuaternion(KeyValues3* kv, const plg::string& name, const plg::vec4& quat) {
    if (kv) kv->SetMemberQuaternion(CKV3MemberName::Make(name), *reinterpret_cast<const Quaternion*>(&quat));
}

/**
 * @brief Sets a table member to an angle (QAngle) value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param ang QAngle to set (as vec3)
 */
extern "C" PLUGIN_API void Kv3SetMemberQAngle(KeyValues3* kv, const plg::string& name, const plg::vec3& ang) {
    if (kv) kv->SetMemberQAngle(CKV3MemberName::Make(name), *reinterpret_cast<const QAngle*>(&ang));
}

/**
 * @brief Sets a table member to a 3x4 matrix value
 * @param kv Pointer to the KeyValues3 object
 * @param name Name of the member
 * @param matrix 3x4 matrix to set (as mat4x4)
 */
extern "C" PLUGIN_API void Kv3SetMemberMatrix3x4(KeyValues3* kv, const plg::string& name, const plg::mat4x4& matrix) {
    if (kv) kv->SetMemberMatrix3x4(CKV3MemberName::Make(name), *reinterpret_cast<const matrix3x4_t*>(&matrix));
}


// ============================================================================
// Debug and Utility Functions - Implementation
// ============================================================================

/**
 * @brief Prints debug information about the KeyValues3 object
 * @param kv Pointer to the KeyValues3 object
 */
extern "C" PLUGIN_API void Kv3DebugPrint(const KeyValues3* kv) {
    if (kv) DebugPrintKV3(kv);
}

// ============================================================================
// Load Functions - Implementation
// ============================================================================

/**
 * @brief Loads KeyValues3 data from a buffer into a context
 * @param context Pointer to the KeyValues3 context
 * @param error Output string for error messages
 * @param input Vector containing the input buffer data
 * @param kv_name Name for the KeyValues3 object
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromBuffer(CKeyValues3Context* context, plg::string& error, const plg::vector<uint8_t>& input, const plg::string& kv_name, uint32_t flags) {
    CUtlBuffer buffer = utils::CreateUtlBufferFromVector(input);
    CUtlString err;
    bool result = LoadKV3(context, &err, &buffer, g_KV3Format_Generic, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a buffer
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param input Vector containing the input buffer data
 * @param kv_name Name for the KeyValues3 object
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3Load(KeyValues3* kv, plg::string& error, const plg::vector<uint8_t>& input, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer = utils::CreateUtlBufferFromVector(input);
    CUtlString err;
    bool result = LoadKV3(kv, &err, &buffer, g_KV3Format_Generic, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a text string
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param input Text string containing KV3 data
 * @param kv_name Name for the KeyValues3 object
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromText(KeyValues3* kv, plg::string& error, const plg::string& input, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3(kv, &err, input.c_str(), g_KV3Format_Generic, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a file into a context
 * @param context Pointer to the KeyValues3 context
 * @param error Output string for error messages
 * @param filename Name of the file to load
 * @param path Path to the file
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromFileToContext(CKeyValues3Context* context, plg::string& error, const plg::string& filename, const plg::string& path, uint32_t flags) {
    CUtlString err;
    bool result = LoadKV3FromFile(context, &err, filename.c_str(), path.c_str(), g_KV3Format_Generic, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a file
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param filename Name of the file to load
 * @param path Path to the file
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromFile(KeyValues3* kv, plg::string& error, const plg::string& filename, const plg::string& path, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromFile(kv, &err, filename.c_str(), path.c_str(), g_KV3Format_Generic, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a JSON string
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param input JSON string
 * @param kv_name Name for the KeyValues3 object
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromJSON(KeyValues3* kv, plg::string& error, const plg::string& input, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromJSON(kv, &err, input.c_str(), kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a JSON file
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param path Path to the file
 * @param filename Name of the file to load
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromJSONFile(KeyValues3* kv, plg::string& error, const plg::string& path, const plg::string& filename, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromJSONFile(kv, &err, path.c_str(), filename.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a KeyValues1 file
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param path Path to the file
 * @param filename Name of the file to load
 * @param esc_behavior Escape sequence behavior for KV1 text
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromKV1File(KeyValues3* kv, plg::string& error, const plg::string& path, const plg::string& filename, uint8_t esc_behavior, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromKV1File(kv, &err, path.c_str(), filename.c_str(), static_cast<KV1TextEscapeBehavior_t>(esc_behavior), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a KeyValues1 text string
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param input KV1 text string
 * @param esc_behavior Escape sequence behavior for KV1 text
 * @param kv_name Name for the KeyValues3 object
 * @param unk Unknown boolean parameter
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromKV1Text(KeyValues3* kv, plg::string& error, const plg::string& input, uint8_t esc_behavior, const plg::string& kv_name, bool unk, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromKV1Text(kv, &err, input.c_str(), static_cast<KV1TextEscapeBehavior_t>(esc_behavior), kv_name.c_str(), unk, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from a KeyValues1 text string with translation
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param input KV1 text string
 * @param esc_behavior Escape sequence behavior for KV1 text
 * @param translation Pointer to translation table
 * @param unk1 Unknown integer parameter
 * @param kv_name Name for the KeyValues3 object
 * @param unk2 Unknown boolean parameter
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromKV1TextTranslated(KeyValues3* kv, plg::string& error, const plg::string& input, uint8_t esc_behavior, const KV1ToKV3Translation_t* translation, int32_t unk1, const plg::string& kv_name, bool unk2, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromKV1Text_Translated(kv, &err, input.c_str(), static_cast<KV1TextEscapeBehavior_t>(esc_behavior), translation, unk1, kv_name.c_str(), unk2, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads data from a buffer that may be KV3 or KV1 format
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param input Vector containing the input buffer data
 * @param kv_name Name for the KeyValues3 object
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromKV3OrKV1(KeyValues3* kv, plg::string& error, const plg::vector<uint8_t>& input, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer = utils::CreateUtlBufferFromVector(input);
    CUtlString err;
    bool result = LoadKV3FromKV3OrKV1(kv, &err, &buffer, g_KV3Format_Generic, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 data from old schema text format
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param input Vector containing the input buffer data
 * @param kv_name Name for the KeyValues3 object
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadFromOldSchemaText(KeyValues3* kv, plg::string& error, const plg::vector<uint8_t>& input, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer = utils::CreateUtlBufferFromVector(input);
    CUtlString err;
    bool result = LoadKV3FromOldSchemaText(kv, &err, &buffer, g_KV3Format_Generic, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

/**
 * @brief Loads KeyValues3 text without a header
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param input Text string containing KV3 data
 * @param kv_name Name for the KeyValues3 object
 * @param flags Loading flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3LoadTextNoHeader(KeyValues3* kv, plg::string& error, const plg::string& input, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3Text_NoHeader(kv, &err, input.c_str(), g_KV3Format_Generic, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

// ============================================================================
// Save Functions - Implementation
// ============================================================================

/**
 * @brief Saves KeyValues3 data to a buffer
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param output Vector to store the output buffer data
 * @param flags Saving flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3Save(const KeyValues3* kv, plg::string& error, plg::vector<uint8_t>& output, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer;
    CUtlString err;
    bool result = SaveKV3(g_KV3Encoding_Text, g_KV3Format_Generic, kv, &err, &buffer, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result) {
        utils::ConvertUtlBufferToVector(&buffer, output);
    }
    return result;
}

/**
 * @brief Saves KeyValues3 data as JSON to a buffer
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param output Vector to store the output JSON data
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3SaveAsJSON(const KeyValues3* kv, plg::string& error, plg::vector<uint8_t>& output) {
    if (!kv) return false;
    CUtlBuffer buffer;
    CUtlString err;
    bool result = SaveKV3AsJSON(kv, &err, &buffer);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result) {
        utils::ConvertUtlBufferToVector(&buffer, output);
    }
    return result;
}

/**
 * @brief Saves KeyValues3 data as a JSON string
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param output String to store the JSON output
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3SaveAsJSONString(const KeyValues3* kv, plg::string& error, plg::string& output) {
    if (!kv) return false;
    CUtlString outStr;
    CUtlString err;
    bool result = SaveKV3AsJSON(kv, &err, &outStr);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result && outStr.Get()) {
        output = plg::string(outStr.Get());
    }
    return result;
}

/**
 * @brief Saves KeyValues3 data as KeyValues1 text to a buffer
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param output Vector to store the output KV1 text data
 * @param esc_behavior Escape sequence behavior for KV1 text
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3SaveAsKV1Text(const KeyValues3* kv, plg::string& error, plg::vector<uint8_t>& output, uint8_t esc_behavior) {
    if (!kv) return false;
    CUtlBuffer buffer;
    CUtlString err;
    bool result = SaveKV3AsKV1Text(kv, &err, &buffer, static_cast<KV1TextEscapeBehavior_t>(esc_behavior));
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result) {
        utils::ConvertUtlBufferToVector(&buffer, output);
    }
    return result;
}

/**
 * @brief Saves KeyValues3 data as KeyValues1 text with translation to a buffer
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param output Vector to store the output KV1 text data
 * @param esc_behavior Escape sequence behavior for KV1 text
 * @param translation Pointer to translation table
 * @param unk Unknown integer parameter
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3SaveAsKV1TextTranslated(const KeyValues3* kv, plg::string& error, plg::vector<uint8_t>& output, uint8_t esc_behavior, const KV3ToKV1Translation_t* translation, int32_t unk) {
    if (!kv) return false;
    CUtlBuffer buffer;
    CUtlString err;
    bool result = SaveKV3AsKV1Text_Translated(kv, &err, &buffer, static_cast<KV1TextEscapeBehavior_t>(esc_behavior), translation, unk);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result) {
        utils::ConvertUtlBufferToVector(&buffer, output);
    }
    return result;
}

/**
 * @brief Saves KeyValues3 text without a header to a buffer
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param output Vector to store the output text data
 * @param flags Saving flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3SaveTextNoHeaderToBuffer(const KeyValues3* kv, plg::string& error, plg::vector<uint8_t>& output, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer;
    CUtlString err;
    bool result = SaveKV3Text_NoHeader(kv, &err, &buffer, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result) {
        utils::ConvertUtlBufferToVector(&buffer, output);
    }
    return result;
}

/**
 * @brief Saves KeyValues3 text without a header to a string
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param output String to store the text output
 * @param flags Saving flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3SaveTextNoHeader(const KeyValues3* kv, plg::string& error, plg::string& output, uint32_t flags) {
    if (!kv) return false;
    CUtlString outStr;
    CUtlString err;
    bool result = SaveKV3Text_NoHeader(kv, &err, &outStr, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result && outStr.Get()) {
        output = plg::string(outStr.Get());
    }
    return result;
}

/**
 * @brief Saves KeyValues3 text to a string
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param output String to store the text output
 * @param flags Saving flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3SaveTextToString(const KeyValues3* kv, plg::string& error, plg::string& output, uint32_t flags) {
    if (!kv) return false;
    CUtlString outStr;
    CUtlString err;
    bool result = SaveKV3Text_ToString(g_KV3Format_Generic, kv, &err, &outStr, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result && outStr.Get()) {
        output = plg::string(outStr.Get());
    }
    return result;
}

/**
 * @brief Saves KeyValues3 data to a file
 * @param kv Pointer to the KeyValues3 object
 * @param error Output string for error messages
 * @param filename Name of the file to save
 * @param path Path to save the file
 * @param flags Saving flags
 * @return true if successful, false otherwise
 */
extern "C" PLUGIN_API bool Kv3SaveToFile(const KeyValues3* kv, plg::string& error, const plg::string& filename, const plg::string& path, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = SaveKV3ToFile(g_KV3Encoding_Text, g_KV3Format_Generic, kv, &err, filename.c_str(), path.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

PLUGIFY_WARN_POP()