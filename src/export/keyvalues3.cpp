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

extern "C" PLUGIN_API KeyValues3* Kv3Create(int32_t type, int32_t subtype) {
    return new KeyValues3(static_cast<KV3TypeEx_t>(type), static_cast<KV3SubType_t>(subtype));
}

extern "C" PLUGIN_API KeyValues3* Kv3CreateWithCluster(int32_t cluster_elem, int32_t type, int32_t subtype) {
    return new KeyValues3(cluster_elem, static_cast<KV3TypeEx_t>(type), static_cast<KV3SubType_t>(subtype));
}

extern "C" PLUGIN_API KeyValues3* Kv3CreateCopy(const KeyValues3* other) {
    if (!other) return nullptr;
    return new KeyValues3(*other);
}

extern "C" PLUGIN_API void Kv3Destroy(KeyValues3* kv) {
    delete kv;
}

// ============================================================================
// Copy Operations - Implementation
// ============================================================================

extern "C" PLUGIN_API void Kv3CopyFrom(KeyValues3* kv, const KeyValues3* other) {
    if (kv && other) kv->CopyFrom(*other);
}

extern "C" PLUGIN_API void Kv3OverlayKeysFrom(KeyValues3* kv, const KeyValues3* other, bool depth) {
    if (kv && other) kv->OverlayKeysFrom(*other, depth);
}

// ============================================================================
// Context and Metadata - Implementation
// ============================================================================

extern "C" PLUGIN_API CKeyValues3Context* Kv3GetContext(const KeyValues3* kv) {
    return kv ? kv->GetContext() : nullptr;
}

extern "C" PLUGIN_API KV3MetaData_t* Kv3GetMetaData(const KeyValues3* kv, CKeyValues3Context** ppCtx) {
    return kv ? kv->GetMetaData(ppCtx) : nullptr;
}

// ============================================================================
// Flags - Implementation
// ============================================================================

extern "C" PLUGIN_API bool Kv3HasFlag(const KeyValues3* kv, uint8_t flag) {
    return kv ? kv->HasFlag(static_cast<KeyValues3Flag_t>(flag)) : false;
}

extern "C" PLUGIN_API bool Kv3HasAnyFlags(const KeyValues3* kv) {
    return kv ? kv->HasAnyFlags() : false;
}

extern "C" PLUGIN_API uint8_t Kv3GetAllFlags(const KeyValues3* kv) {
    return kv ? static_cast<uint8_t>(kv->GetAllFlags()) : 0;
}

extern "C" PLUGIN_API void Kv3SetAllFlags(KeyValues3* kv, uint8_t flags) {
    if (kv) kv->SetAllFlags(static_cast<KeyValues3Flag_t>(flags));
}

extern "C" PLUGIN_API void Kv3SetFlag(KeyValues3* kv, uint8_t flag, bool state) {
    if (kv) kv->SetFlag(static_cast<KeyValues3Flag_t>(flag), state);
}

// ============================================================================
// Type Information - Implementation
// ============================================================================

extern "C" PLUGIN_API uint8_t Kv3GetType(const KeyValues3* kv) {
    return kv ? static_cast<uint8_t>(kv->GetType()) : 0;
}

extern "C" PLUGIN_API uint8_t Kv3GetTypeEx(const KeyValues3* kv) {
    return kv ? static_cast<uint8_t>(kv->GetTypeEx()) : 0;
}

extern "C" PLUGIN_API uint8_t Kv3GetSubType(const KeyValues3* kv) {
    return kv ? static_cast<uint8_t>(kv->GetSubType()) : 0;
}

extern "C" PLUGIN_API bool Kv3HasInvalidMemberNames(const KeyValues3* kv) {
    return kv ? kv->HasInvalidMemberNames() : false;
}

extern "C" PLUGIN_API void Kv3SetHasInvalidMemberNames(KeyValues3* kv, bool bValue) {
    if (kv) kv->SetHasInvalidMemberNames(bValue);
}

extern "C" PLUGIN_API plg::string Kv3GetTypeAsString(const KeyValues3* kv) {
    if (!kv) return plg::string();
    const char* str = kv->GetTypeAsString();
    return plg::string(str ? str : "");
}

extern "C" PLUGIN_API plg::string Kv3GetSubTypeAsString(const KeyValues3* kv) {
    if (!kv) return plg::string();
    const char* str = kv->GetSubTypeAsString();
    return plg::string(str ? str : "");
}

extern "C" PLUGIN_API plg::string Kv3ToString(const KeyValues3* kv, uint32_t flags) {
    if (!kv) return plg::string();
    CBufferString buff;
    const char* str = kv->ToString(buff, flags);
    return plg::string(str ? str : "");
}

// ============================================================================
// Type Checks - Implementation
// ============================================================================

extern "C" PLUGIN_API bool Kv3IsNull(const KeyValues3* kv) {
    return kv ? kv->IsNull() : true;
}

extern "C" PLUGIN_API void Kv3SetToNull(KeyValues3* kv) {
    if (kv) kv->SetToNull();
}

extern "C" PLUGIN_API bool Kv3IsArray(const KeyValues3* kv) {
    return kv ? kv->IsArray() : false;
}

extern "C" PLUGIN_API bool Kv3IsKV3Array(const KeyValues3* kv) {
    return kv ? kv->IsKV3Array() : false;
}

extern "C" PLUGIN_API bool Kv3IsTable(const KeyValues3* kv) {
    return kv ? kv->IsTable() : false;
}

extern "C" PLUGIN_API bool Kv3IsString(const KeyValues3* kv) {
    return kv ? kv->IsString() : false;
}

// ============================================================================
// Basic Type Getters - Implementation
// ============================================================================

extern "C" PLUGIN_API bool Kv3GetBool(const KeyValues3* kv, bool defaultValue) {
    return kv ? kv->GetBool(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API char Kv3GetChar(const KeyValues3* kv, char defaultValue) {
    return kv ? kv->GetChar(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint32_t Kv3GetUChar32(const KeyValues3* kv, uint32_t defaultValue) {
    return kv ? kv->GetUChar32(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API int8_t Kv3GetInt8(const KeyValues3* kv, int8_t defaultValue) {
    return kv ? kv->GetInt8(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint8_t Kv3GetUInt8(const KeyValues3* kv, uint8_t defaultValue) {
    return kv ? kv->GetUInt8(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API int16_t Kv3GetShort(const KeyValues3* kv, int16_t defaultValue) {
    return kv ? kv->GetShort(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint16_t Kv3GetUShort(const KeyValues3* kv, uint16_t defaultValue) {
    return kv ? kv->GetUShort(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API int32_t Kv3GetInt(const KeyValues3* kv, int32_t defaultValue) {
    return kv ? kv->GetInt(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint32_t Kv3GetUInt(const KeyValues3* kv, uint32_t defaultValue) {
    return kv ? kv->GetUInt(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API int64_t Kv3GetInt64(const KeyValues3* kv, int64_t defaultValue) {
    return kv ? kv->GetInt64(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint64_t Kv3GetUInt64(const KeyValues3* kv, uint64_t defaultValue) {
    return kv ? kv->GetUInt64(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API float Kv3GetFloat(const KeyValues3* kv, float defaultValue) {
    return kv ? kv->GetFloat(defaultValue) : defaultValue;
}

extern "C" PLUGIN_API double Kv3GetDouble(const KeyValues3* kv, double defaultValue) {
    return kv ? kv->GetDouble(defaultValue) : defaultValue;
}

// ============================================================================
// Basic Type Setters - Implementation
// ============================================================================

extern "C" PLUGIN_API void Kv3SetBool(KeyValues3* kv, bool value) {
    if (kv) kv->SetBool(value);
}

extern "C" PLUGIN_API void Kv3SetChar(KeyValues3* kv, char value) {
    if (kv) kv->SetChar(value);
}

extern "C" PLUGIN_API void Kv3SetUChar32(KeyValues3* kv, uint32_t value) {
    if (kv) kv->SetUChar32(value);
}

extern "C" PLUGIN_API void Kv3SetInt8(KeyValues3* kv, int8_t value) {
    if (kv) kv->SetInt8(value);
}

extern "C" PLUGIN_API void Kv3SetUInt8(KeyValues3* kv, uint8_t value) {
    if (kv) kv->SetUInt8(value);
}

extern "C" PLUGIN_API void Kv3SetShort(KeyValues3* kv, int16_t value) {
    if (kv) kv->SetShort(value);
}

extern "C" PLUGIN_API void Kv3SetUShort(KeyValues3* kv, uint16_t value) {
    if (kv) kv->SetUShort(value);
}

extern "C" PLUGIN_API void Kv3SetInt(KeyValues3* kv, int32_t value) {
    if (kv) kv->SetInt(value);
}

extern "C" PLUGIN_API void Kv3SetUInt(KeyValues3* kv, uint32_t value) {
    if (kv) kv->SetUInt(value);
}

extern "C" PLUGIN_API void Kv3SetInt64(KeyValues3* kv, int64_t value) {
    if (kv) kv->SetInt64(value);
}

extern "C" PLUGIN_API void Kv3SetUInt64(KeyValues3* kv, uint64_t value) {
    if (kv) kv->SetUInt64(value);
}

extern "C" PLUGIN_API void Kv3SetFloat(KeyValues3* kv, float value) {
    if (kv) kv->SetFloat(value);
}

extern "C" PLUGIN_API void Kv3SetDouble(KeyValues3* kv, double value) {
    if (kv) kv->SetDouble(value);
}

// ============================================================================
// Pointer Handling - Implementation
// ============================================================================

extern "C" PLUGIN_API uintptr_t Kv3GetPointer(const KeyValues3* kv, uintptr_t defaultValue) {
    if (!kv) return defaultValue;
    void* ptr = kv->GetPointer(reinterpret_cast<void*>(defaultValue));
    return reinterpret_cast<uintptr_t>(ptr);
}

extern "C" PLUGIN_API void Kv3SetPointer(KeyValues3* kv, uintptr_t ptr) {
    if (kv) kv->SetPointer(reinterpret_cast<void*>(ptr));
}

// ============================================================================
// String Token Handling - Implementation
// ============================================================================

extern "C" PLUGIN_API uint32_t Kv3GetStringToken(const KeyValues3* kv, uint32_t defaultValue) {
    if (!kv) return defaultValue;
    CUtlStringToken token(defaultValue);
    return kv->GetStringToken(token).GetHashCode();
}

extern "C" PLUGIN_API void Kv3SetStringToken(KeyValues3* kv, uint32_t token) {
    if (kv) kv->SetStringToken(CUtlStringToken(token));
}

// ============================================================================
// Entity Handle - Implementation
// ============================================================================

extern "C" PLUGIN_API int32_t Kv3GetEHandle(const KeyValues3* kv, int32_t defaultValue) {
    if (!kv) return defaultValue;
    CEntityHandle handle(defaultValue);
    return kv->GetEHandle(handle).ToInt();
}

extern "C" PLUGIN_API void Kv3SetEHandle(KeyValues3* kv, int32_t ehandle) {
    if (kv) kv->SetEHandle(CEntityHandle(ehandle));
}

// ============================================================================
// String Operations - Implementation
// ============================================================================

extern "C" PLUGIN_API plg::string Kv3GetString(const KeyValues3* kv, const plg::string& defaultValue) {
    if (!kv) return defaultValue;
    const char* str = kv->GetString(defaultValue.c_str());
    return plg::string(str ? str : "");
}

extern "C" PLUGIN_API void Kv3SetString(KeyValues3* kv, const plg::string& str, uint8_t subtype) {
    if (kv) kv->SetString(str.c_str(), static_cast<KV3SubType_t>(subtype));
}

extern "C" PLUGIN_API void Kv3SetStringExternal(KeyValues3* kv, const plg::string& str, uint8_t subtype) {
    if (kv) kv->SetStringExternal(str.c_str(), static_cast<KV3SubType_t>(subtype));
}

// ============================================================================
// Binary Blob Operations - Implementation
// ============================================================================

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

extern "C" PLUGIN_API int32_t Kv3GetBinaryBlobSize(const KeyValues3* kv) {
    return kv ? kv->GetBinaryBlobSize() : 0;
}

extern "C" PLUGIN_API void Kv3SetToBinaryBlob(KeyValues3* kv, const plg::vector<uint8_t>& blob) {
    if (kv && !blob.empty()) {
        kv->SetToBinaryBlob(blob.data(), static_cast<int>(blob.size()));
    }
}

extern "C" PLUGIN_API void Kv3SetToBinaryBlobExternal(KeyValues3* kv, const plg::vector<uint8_t>& blob, bool free_mem) {
    if (kv && !blob.empty()) {
        kv->SetToBinaryBlobExternal(blob.data(), static_cast<int>(blob.size()), free_mem);
    }
}

// ============================================================================
// Color Operations - Implementation
// ============================================================================

extern "C" PLUGIN_API int32_t Kv3GetColor(const KeyValues3* kv, int32_t defaultValue) {
    if (!kv) return defaultValue;
    Color defaultColor = *reinterpret_cast<const Color*>(&defaultValue);
    Color result = kv->GetColor(defaultColor);
    return *reinterpret_cast<const uint32_t*>(&result);
}

extern "C" PLUGIN_API void Kv3SetColor(KeyValues3* kv, int32_t color) {
    if (kv) kv->SetColor(*reinterpret_cast<const Color*>(&color));
}

// ============================================================================
// Vector/Matrix Getters - Implementation
// ============================================================================

extern "C" PLUGIN_API plg::vec3 Kv3GetVector(const KeyValues3* kv, const plg::vec3& defaultValue) {
    if (!kv) return defaultValue;
    // Vector is binary compatible with plg::vec3
    Vector result = kv->GetVector(*reinterpret_cast<const Vector*>(&defaultValue));
    return *reinterpret_cast<const plg::vec3*>(&result);
}

extern "C" PLUGIN_API plg::vec2 Kv3GetVector2D(const KeyValues3* kv, const plg::vec2& defaultValue) {
    if (!kv) return defaultValue;
    // Vector2D is binary compatible with plg::vec2
    Vector2D result = kv->GetVector2D(*reinterpret_cast<const Vector2D*>(&defaultValue));
    return *reinterpret_cast<const plg::vec2*>(&result);
}

extern "C" PLUGIN_API plg::vec4 Kv3GetVector4D(const KeyValues3* kv, const plg::vec4& defaultValue) {
    if (!kv) return defaultValue;
    // Vector4D is binary compatible with plg::vec4
    Vector4D result = kv->GetVector4D(*reinterpret_cast<const Vector4D*>(&defaultValue));
    return *reinterpret_cast<const plg::vec4*>(&result);
}

extern "C" PLUGIN_API plg::vec4 Kv3GetQuaternion(const KeyValues3* kv, const plg::vec4& defaultValue) {
    if (!kv) return defaultValue;
    // Quaternion is binary compatible with plg::vec4
    Quaternion result = kv->GetQuaternion(*reinterpret_cast<const Quaternion*>(&defaultValue));
    return *reinterpret_cast<const plg::vec4*>(&result);
}

extern "C" PLUGIN_API plg::vec3 Kv3GetQAngle(const KeyValues3* kv, const plg::vec3& defaultValue) {
    if (!kv) return defaultValue;
    // QAngle is binary compatible with plg::vec3
    QAngle result = kv->GetQAngle(*reinterpret_cast<const QAngle*>(&defaultValue));
    return *reinterpret_cast<const plg::vec3*>(&result);
}

extern "C" PLUGIN_API plg::mat4x4 Kv3GetMatrix3x4(const KeyValues3* kv, const plg::mat4x4& defaultValue) {
    if (!kv) return defaultValue;
    // matrix3x4_t is binary compatible with first 3 rows of plg::mat4x4
    matrix3x4_t result = kv->GetMatrix3x4(*reinterpret_cast<const matrix3x4_t*>(&defaultValue));
    return *reinterpret_cast<const plg::mat4x4*>(&result);
}

// ============================================================================
// Vector/Matrix Setters - Implementation
// ============================================================================

extern "C" PLUGIN_API void Kv3SetVector(KeyValues3* kv, const plg::vec3& vec) {
    if (kv) kv->SetVector(*reinterpret_cast<const Vector*>(&vec));
}

extern "C" PLUGIN_API void Kv3SetVector2D(KeyValues3* kv, const plg::vec2& vec2d) {
    if (kv) kv->SetVector2D(*reinterpret_cast<const Vector2D*>(&vec2d));
}

extern "C" PLUGIN_API void Kv3SetVector4D(KeyValues3* kv, const plg::vec4& vec4d) {
    if (kv) kv->SetVector4D(*reinterpret_cast<const Vector4D*>(&vec4d));
}

extern "C" PLUGIN_API void Kv3SetQuaternion(KeyValues3* kv, const plg::vec4& quat) {
    if (kv) kv->SetQuaternion(*reinterpret_cast<const Quaternion*>(&quat));
}

extern "C" PLUGIN_API void Kv3SetQAngle(KeyValues3* kv, const plg::vec3& ang) {
    if (kv) kv->SetQAngle(*reinterpret_cast<const QAngle*>(&ang));
}

extern "C" PLUGIN_API void Kv3SetMatrix3x4(KeyValues3* kv, const plg::mat4x4& matrix) {
    if (kv) kv->SetMatrix3x4(*reinterpret_cast<const matrix3x4_t*>(&matrix));
}

// ============================================================================
// Array Operations - Implementation
// ============================================================================

extern "C" PLUGIN_API int32_t Kv3GetArrayElementCount(const KeyValues3* kv) {
    return kv ? kv->GetArrayElementCount() : 0;
}

extern "C" PLUGIN_API void Kv3SetArrayElementCount(KeyValues3* kv, int32_t count, uint8_t type, uint8_t subtype) {
    if (kv) kv->SetArrayElementCount(count, static_cast<KV3TypeEx_t>(type), static_cast<KV3SubType_t>(subtype));
}

extern "C" PLUGIN_API void Kv3SetToEmptyKV3Array(KeyValues3* kv) {
    if (kv) kv->SetToEmptyKV3Array();
}

extern "C" PLUGIN_API KeyValues3* Kv3GetArrayElement(KeyValues3* kv, int32_t elem) {
    return kv ? kv->GetArrayElement(elem) : nullptr;
}

extern "C" PLUGIN_API KeyValues3* Kv3ArrayInsertElementBefore(KeyValues3* kv, int32_t elem) {
    return kv ? kv->ArrayInsertElementBefore(elem) : nullptr;
}

extern "C" PLUGIN_API KeyValues3* Kv3ArrayInsertElementAfter(KeyValues3* kv, int32_t elem) {
    return kv ? kv->ArrayInsertElementAfter(elem) : nullptr;
}

extern "C" PLUGIN_API KeyValues3* Kv3ArrayAddElementToTail(KeyValues3* kv) {
    return kv ? kv->ArrayAddElementToTail() : nullptr;
}

extern "C" PLUGIN_API void Kv3ArraySwapItems(KeyValues3* kv, int32_t idx1, int32_t idx2) {
    if (kv) kv->ArraySwapItems(idx1, idx2);
}

extern "C" PLUGIN_API void Kv3ArrayRemoveElement(KeyValues3* kv, int32_t elem) {
    if (kv) kv->ArrayRemoveElement(elem);
}

// ============================================================================
// Table Operations - Implementation
// ============================================================================

extern "C" PLUGIN_API void Kv3SetToEmptyTable(KeyValues3* kv) {
    if (kv) kv->SetToEmptyTable();
}

extern "C" PLUGIN_API int32_t Kv3GetMemberCount(const KeyValues3* kv) {
    return kv ? kv->GetMemberCount() : 0;
}

extern "C" PLUGIN_API bool Kv3HasMember(const KeyValues3* kv, const plg::string& name) {
    return kv ? kv->FindMember(CKV3MemberName::Make(name)) != nullptr : false;
}

extern "C" PLUGIN_API KeyValues3* Kv3FindMember(KeyValues3* kv, const plg::string& name) {
    return kv ? kv->FindMember(CKV3MemberName::Make(name)) : nullptr;
}

extern "C" PLUGIN_API KeyValues3* Kv3FindOrCreateMember(KeyValues3* kv, const plg::string& name) {
    return kv ? kv->FindOrCreateMember(CKV3MemberName::Make(name)) : nullptr;
}

extern "C" PLUGIN_API bool Kv3RemoveMember(KeyValues3* kv, const plg::string& name) {
    return kv ? kv->RemoveMember(CKV3MemberName::Make(name)) : false;
}

extern "C" PLUGIN_API plg::string Kv3GetMemberName(const KeyValues3* kv, int32_t index) {
    if (!kv) return plg::string();
    const char* name = kv->GetMemberName(index);
    return plg::string(name ? name : "");
}

extern "C" PLUGIN_API KeyValues3* Kv3GetMemberByIndex(KeyValues3* kv, int32_t index) {
    return kv ? kv->GetMember(index) : nullptr;
}

// ============================================================================
// Member Value Getters - Implementation
// ============================================================================

extern "C" PLUGIN_API bool Kv3GetMemberBool(const KeyValues3* kv, const plg::string& name, bool defaultValue) {
    return kv ? kv->GetMemberBool(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API char Kv3GetMemberChar(const KeyValues3* kv, const plg::string& name, char defaultValue) {
    return kv ? kv->GetMemberChar(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint32_t Kv3GetMemberUChar32(const KeyValues3* kv, const plg::string& name, uint32_t defaultValue) {
    return kv ? kv->GetMemberUChar32(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API int8_t Kv3GetMemberInt8(const KeyValues3* kv, const plg::string& name, int8_t defaultValue) {
    return kv ? kv->GetMemberInt8(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint8_t Kv3GetMemberUInt8(const KeyValues3* kv, const plg::string& name, uint8_t defaultValue) {
    return kv ? kv->GetMemberUInt8(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API int16_t Kv3GetMemberShort(const KeyValues3* kv, const plg::string& name, int16_t defaultValue) {
    return kv ? kv->GetMemberShort(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint16_t Kv3GetMemberUShort(const KeyValues3* kv, const plg::string& name, uint16_t defaultValue) {
    return kv ? kv->GetMemberUShort(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API int32_t Kv3GetMemberInt(const KeyValues3* kv, const plg::string& name, int32_t defaultValue) {
    return kv ? kv->GetMemberInt(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint32_t Kv3GetMemberUInt(const KeyValues3* kv, const plg::string& name, uint32_t defaultValue) {
    return kv ? kv->GetMemberUInt(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API int64_t Kv3GetMemberInt64(const KeyValues3* kv, const plg::string& name, int64_t defaultValue) {
    return kv ? kv->GetMemberInt64(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uint64_t Kv3GetMemberUInt64(const KeyValues3* kv, const plg::string& name, uint64_t defaultValue) {
    return kv ? kv->GetMemberUInt64(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API float Kv3GetMemberFloat(const KeyValues3* kv, const plg::string& name, float defaultValue) {
    return kv ? kv->GetMemberFloat(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API double Kv3GetMemberDouble(const KeyValues3* kv, const plg::string& name, double defaultValue) {
    return kv ? kv->GetMemberDouble(CKV3MemberName::Make(name), defaultValue) : defaultValue;
}

extern "C" PLUGIN_API uintptr_t Kv3GetMemberPointer(const KeyValues3* kv, const plg::string& name, uintptr_t defaultValue) {
    if (!kv) return defaultValue;
    void* ptr = kv->GetMemberPointer(CKV3MemberName::Make(name), reinterpret_cast<void*>(defaultValue));
    return reinterpret_cast<uintptr_t>(ptr);
}

extern "C" PLUGIN_API uint32_t Kv3GetMemberStringToken(const KeyValues3* kv, const plg::string& name, uint32_t defaultValue) {
    if (!kv) return defaultValue;
    CUtlStringToken token(defaultValue);
    return kv->GetMemberStringToken(CKV3MemberName::Make(name), token).GetHashCode();
}

extern "C" PLUGIN_API int32_t Kv3GetMemberEHandle(const KeyValues3* kv, const plg::string& name, int32_t defaultValue) {
    if (!kv) return defaultValue;
    CEntityHandle handle(defaultValue);
    return kv->GetMemberEHandle(CKV3MemberName::Make(name), handle).ToInt();
}

extern "C" PLUGIN_API plg::string Kv3GetMemberString(const KeyValues3* kv, const plg::string& name, const plg::string& defaultValue) {
    if (!kv) return defaultValue;
    const char* str = kv->GetMemberString(CKV3MemberName::Make(name), defaultValue.c_str());
    return plg::string(str ? str : "");
}

extern "C" PLUGIN_API int32_t Kv3GetMemberColor(const KeyValues3* kv, const plg::string& name, int32_t defaultValue) {
    if (!kv) return defaultValue;
    Color defaultColor = *reinterpret_cast<const Color*>(&defaultValue);
    Color result = kv->GetMemberColor(CKV3MemberName::Make(name), defaultColor);
    return *reinterpret_cast<const int32_t*>(&result);
}

extern "C" PLUGIN_API plg::vec3 Kv3GetMemberVector(const KeyValues3* kv, const plg::string& name, const plg::vec3& defaultValue) {
    if (!kv) return defaultValue;
    Vector result = kv->GetMemberVector(CKV3MemberName::Make(name), *reinterpret_cast<const Vector*>(&defaultValue));
    return *reinterpret_cast<const plg::vec3*>(&result);
}

extern "C" PLUGIN_API plg::vec2 Kv3GetMemberVector2D(const KeyValues3* kv, const plg::string& name, const plg::vec2& defaultValue) {
    if (!kv) return defaultValue;
    Vector2D result = kv->GetMemberVector2D(CKV3MemberName::Make(name), *reinterpret_cast<const Vector2D*>(&defaultValue));
    return *reinterpret_cast<const plg::vec2*>(&result);
}

extern "C" PLUGIN_API plg::vec4 Kv3GetMemberVector4D(const KeyValues3* kv, const plg::string& name, const plg::vec4& defaultValue) {
    if (!kv) return defaultValue;
    Vector4D result = kv->GetMemberVector4D(CKV3MemberName::Make(name), *reinterpret_cast<const Vector4D*>(&defaultValue));
    return *reinterpret_cast<const plg::vec4*>(&result);
}

extern "C" PLUGIN_API plg::vec4 Kv3GetMemberQuaternion(const KeyValues3* kv, const plg::string& name, const plg::vec4& defaultValue) {
    if (!kv) return defaultValue;
    Quaternion result = kv->GetMemberQuaternion(CKV3MemberName::Make(name), *reinterpret_cast<const Quaternion*>(&defaultValue));
    return *reinterpret_cast<const plg::vec4*>(&result);
}

extern "C" PLUGIN_API plg::vec3 Kv3GetMemberQAngle(const KeyValues3* kv, const plg::string& name, const plg::vec3& defaultValue) {
    if (!kv) return defaultValue;
    QAngle result = kv->GetMemberQAngle(CKV3MemberName::Make(name), *reinterpret_cast<const QAngle*>(&defaultValue));
    return *reinterpret_cast<const plg::vec3*>(&result);
}

extern "C" PLUGIN_API plg::mat4x4 Kv3GetMemberMatrix3x4(const KeyValues3* kv, const plg::string& name, const plg::mat4x4& defaultValue) {
    if (!kv) return defaultValue;
    matrix3x4_t result = kv->GetMemberMatrix3x4(CKV3MemberName::Make(name), *reinterpret_cast<const matrix3x4_t*>(&defaultValue));
    return *reinterpret_cast<const plg::mat4x4*>(&result);
}

// ============================================================================
// Member Value Setters - Special - Implementation
// ============================================================================

extern "C" PLUGIN_API void Kv3SetMemberToNull(KeyValues3* kv, const plg::string& name) {
    if (kv) kv->SetMemberToNull(CKV3MemberName::Make(name));
}

extern "C" PLUGIN_API void Kv3SetMemberToEmptyArray(KeyValues3* kv, const plg::string& name) {
    if (kv) kv->SetMemberToEmptyArray(CKV3MemberName::Make(name));
}

extern "C" PLUGIN_API void Kv3SetMemberToEmptyTable(KeyValues3* kv, const plg::string& name) {
    if (kv) kv->SetMemberToEmptyTable(CKV3MemberName::Make(name));
}

extern "C" PLUGIN_API void Kv3SetMemberToBinaryBlob(KeyValues3* kv, const plg::string& name, const plg::vector<uint8_t>& blob) {
    if (kv && !blob.empty()) {
        kv->SetMemberToBinaryBlob(CKV3MemberName::Make(name), blob.data(), static_cast<int>(blob.size()));
    }
}

extern "C" PLUGIN_API void Kv3SetMemberToBinaryBlobExternal(KeyValues3* kv, const plg::string& name, const plg::vector<uint8_t>& blob, bool free_mem) {
    if (kv && !blob.empty()) {
        kv->SetMemberToBinaryBlobExternal(CKV3MemberName::Make(name), blob.data(), static_cast<int>(blob.size()), free_mem);
    }
}

extern "C" PLUGIN_API void Kv3SetMemberToCopyOfValue(KeyValues3* kv, const plg::string& name, KeyValues3* other) {
    if (kv && other) kv->SetMemberToCopyOfValue(CKV3MemberName::Make(name), other);
}

// ============================================================================
// Member Value Setters - Basic Types - Implementation
// ============================================================================

extern "C" PLUGIN_API void Kv3SetMemberBool(KeyValues3* kv, const plg::string& name, bool value) {
    if (kv) kv->SetMemberBool(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberChar(KeyValues3* kv, const plg::string& name, char value) {
    if (kv) kv->SetMemberChar(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberUChar32(KeyValues3* kv, const plg::string& name, uint32_t value) {
    if (kv) kv->SetMemberUChar32(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberInt8(KeyValues3* kv, const plg::string& name, int8_t value) {
    if (kv) kv->SetMemberInt8(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberUInt8(KeyValues3* kv, const plg::string& name, uint8_t value) {
    if (kv) kv->SetMemberUInt8(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberShort(KeyValues3* kv, const plg::string& name, int16_t value) {
    if (kv) kv->SetMemberShort(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberUShort(KeyValues3* kv, const plg::string& name, uint16_t value) {
    if (kv) kv->SetMemberUShort(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberInt(KeyValues3* kv, const plg::string& name, int32_t value) {
    if (kv) kv->SetMemberInt(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberUInt(KeyValues3* kv, const plg::string& name, uint32_t value) {
    if (kv) kv->SetMemberUInt(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberInt64(KeyValues3* kv, const plg::string& name, int64_t value) {
    if (kv) kv->SetMemberInt64(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberUInt64(KeyValues3* kv, const plg::string& name, uint64_t value) {
    if (kv) kv->SetMemberUInt64(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberFloat(KeyValues3* kv, const plg::string& name, float value) {
    if (kv) kv->SetMemberFloat(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberDouble(KeyValues3* kv, const plg::string& name, double value) {
    if (kv) kv->SetMemberDouble(CKV3MemberName::Make(name), value);
}

extern "C" PLUGIN_API void Kv3SetMemberPointer(KeyValues3* kv, const plg::string& name, uintptr_t ptr) {
    if (kv) kv->SetMemberPointer(CKV3MemberName::Make(name), reinterpret_cast<void*>(ptr));
}

extern "C" PLUGIN_API void Kv3SetMemberStringToken(KeyValues3* kv, const plg::string& name, uint32_t token) {
    if (kv) kv->SetMemberStringToken(CKV3MemberName::Make(name), CUtlStringToken(token));
}

extern "C" PLUGIN_API void Kv3SetMemberEHandle(KeyValues3* kv, const plg::string& name, int32_t ehandle) {
    if (kv) kv->SetMemberEHandle(CKV3MemberName::Make(name), CEntityHandle(ehandle));
}

extern "C" PLUGIN_API void Kv3SetMemberString(KeyValues3* kv, const plg::string& name, const plg::string& str, uint8_t subtype) {
    if (kv) kv->SetMemberString(CKV3MemberName::Make(name), str.c_str(), static_cast<KV3SubType_t>(subtype));
}

extern "C" PLUGIN_API void Kv3SetMemberStringExternal(KeyValues3* kv, const plg::string& name, const plg::string& str, uint8_t subtype) {
    if (kv) kv->SetMemberStringExternal(CKV3MemberName::Make(name), str.c_str(), static_cast<KV3SubType_t>(subtype));
}

extern "C" PLUGIN_API void Kv3SetMemberColor(KeyValues3* kv, const plg::string& name, int32_t color) {
    if (kv) kv->SetMemberColor(CKV3MemberName::Make(name), *reinterpret_cast<const Color*>(&color));
}

extern "C" PLUGIN_API void Kv3SetMemberVector(KeyValues3* kv, const plg::string& name, const plg::vec3& vec) {
    if (kv) kv->SetMemberVector(CKV3MemberName::Make(name), *reinterpret_cast<const Vector*>(&vec));
}

extern "C" PLUGIN_API void Kv3SetMemberVector2D(KeyValues3* kv, const plg::string& name, const plg::vec2& vec2d) {
    if (kv) kv->SetMemberVector2D(CKV3MemberName::Make(name), *reinterpret_cast<const Vector2D*>(&vec2d));
}

extern "C" PLUGIN_API void Kv3SetMemberVector4D(KeyValues3* kv, const plg::string& name, const plg::vec4& vec4d) {
    if (kv) kv->SetMemberVector4D(CKV3MemberName::Make(name), *reinterpret_cast<const Vector4D*>(&vec4d));
}

extern "C" PLUGIN_API void Kv3SetMemberQuaternion(KeyValues3* kv, const plg::string& name, const plg::vec4& quat) {
    if (kv) kv->SetMemberQuaternion(CKV3MemberName::Make(name), *reinterpret_cast<const Quaternion*>(&quat));
}

extern "C" PLUGIN_API void Kv3SetMemberQAngle(KeyValues3* kv, const plg::string& name, const plg::vec3& ang) {
    if (kv) kv->SetMemberQAngle(CKV3MemberName::Make(name), *reinterpret_cast<const QAngle*>(&ang));
}

extern "C" PLUGIN_API void Kv3SetMemberMatrix3x4(KeyValues3* kv, const plg::string& name, const plg::mat4x4& matrix) {
    if (kv) kv->SetMemberMatrix3x4(CKV3MemberName::Make(name), *reinterpret_cast<const matrix3x4_t*>(&matrix));
}


// ============================================================================
// Debug and Utility Functions - Implementation
// ============================================================================

extern "C" PLUGIN_API void Kv3DebugPrint(const KeyValues3* kv) {
    if (kv) DebugPrintKV3(kv);
}

// ============================================================================
// Load Functions - Implementation
// ============================================================================

extern "C" PLUGIN_API bool Kv3LoadFromBuffer(CKeyValues3Context* context, plg::string& error, const plg::vector<uint8_t>& input, const KV3ID_t& format, const plg::string& kv_name, uint32_t flags) {
    CUtlBuffer buffer = utils::CreateUtlBufferFromVector(input);
    CUtlString err;
    bool result = LoadKV3(context, &err, &buffer, format, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3Load(KeyValues3* kv, plg::string& error, const plg::vector<uint8_t>& input, const KV3ID_t& format, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer = utils::CreateUtlBufferFromVector(input);
    CUtlString err;
    bool result = LoadKV3(kv, &err, &buffer, format, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromText(KeyValues3* kv, plg::string& error, const plg::string& input, const KV3ID_t& format, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3(kv, &err, input.c_str(), format, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromFileToContext(CKeyValues3Context* context, plg::string& error, const plg::string& filename, const plg::string& path, const KV3ID_t& format, uint32_t flags) {
    CUtlString err;
    bool result = LoadKV3FromFile(context, &err, filename.c_str(), path.c_str(), format, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromFile(KeyValues3* kv, plg::string& error, const plg::string& filename, const plg::string& path, const KV3ID_t& format, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromFile(kv, &err, filename.c_str(), path.c_str(), format, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromJSON(KeyValues3* kv, plg::string& error, const plg::string& input, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromJSON(kv, &err, input.c_str(), kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromJSONFile(KeyValues3* kv, plg::string& error, const plg::string& path, const plg::string& filename, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromJSONFile(kv, &err, path.c_str(), filename.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromKV1File(KeyValues3* kv, plg::string& error, const plg::string& path, const plg::string& filename, uint8_t esc_behavior, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromKV1File(kv, &err, path.c_str(), filename.c_str(), static_cast<KV1TextEscapeBehavior_t>(esc_behavior), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromKV1Text(KeyValues3* kv, plg::string& error, const plg::string& input, uint8_t esc_behavior, const plg::string& kv_name, bool unk, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromKV1Text(kv, &err, input.c_str(), static_cast<KV1TextEscapeBehavior_t>(esc_behavior), kv_name.c_str(), unk, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromKV1TextTranslated(KeyValues3* kv, plg::string& error, const plg::string& input, uint8_t esc_behavior, const KV1ToKV3Translation_t* translation, int32_t unk1, const plg::string& kv_name, bool unk2, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3FromKV1Text_Translated(kv, &err, input.c_str(), static_cast<KV1TextEscapeBehavior_t>(esc_behavior), translation, unk1, kv_name.c_str(), unk2, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromKV3OrKV1(KeyValues3* kv, plg::string& error, const plg::vector<uint8_t>& input, const KV3ID_t& format, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer = utils::CreateUtlBufferFromVector(input);
    CUtlString err;
    bool result = LoadKV3FromKV3OrKV1(kv, &err, &buffer, format, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadFromOldSchemaText(KeyValues3* kv, plg::string& error, const plg::vector<uint8_t>& input, const KV3ID_t& format, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer = utils::CreateUtlBufferFromVector(input);
    CUtlString err;
    bool result = LoadKV3FromOldSchemaText(kv, &err, &buffer, format, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

extern "C" PLUGIN_API bool Kv3LoadTextNoHeader(KeyValues3* kv, plg::string& error, const plg::string& input, const KV3ID_t& format, const plg::string& kv_name, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = LoadKV3Text_NoHeader(kv, &err, input.c_str(), format, kv_name.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

// ============================================================================
// Save Functions - Implementation
// ============================================================================

extern "C" PLUGIN_API bool Kv3Save(const KV3ID_t& encoding, const KV3ID_t& format, const KeyValues3* kv, plg::string& error, plg::vector<uint8_t>& output, uint32_t flags) {
    if (!kv) return false;
    CUtlBuffer buffer;
    CUtlString err;
    bool result = SaveKV3(encoding, format, kv, &err, &buffer, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result) {
        utils::ConvertUtlBufferToVector(&buffer, output);
    }
    return result;
}

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

extern "C" PLUGIN_API bool Kv3SaveTextToString(const KV3ID_t& format, const KeyValues3* kv, plg::string& error, plg::string& output, uint32_t flags) {
    if (!kv) return false;
    CUtlString outStr;
    CUtlString err;
    bool result = SaveKV3Text_ToString(format, kv, &err, &outStr, flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    if (result && outStr.Get()) {
        output = plg::string(outStr.Get());
    }
    return result;
}

extern "C" PLUGIN_API bool Kv3SaveToFile(const KV3ID_t& encoding, const KV3ID_t& format, const KeyValues3* kv, plg::string& error, const plg::string& filename, const plg::string& path, uint32_t flags) {
    if (!kv) return false;
    CUtlString err;
    bool result = SaveKV3ToFile(encoding, format, kv, &err, filename.c_str(), path.c_str(), flags);
    utils::ConvertUtlStringToPlgString(&err, error);
    return result;
}

PLUGIFY_WARN_POP()