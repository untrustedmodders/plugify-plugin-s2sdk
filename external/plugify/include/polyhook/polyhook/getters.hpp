#pragma once

#include "enums.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from polyhook.pplugin (group: getters)

namespace polyhook {

  using _GetFunctionAddr = void* (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetFunctionAddr __polyhook_GetFunctionAddr;
namespace polyhook {
  /**
   * @brief Get generated function address
   * @param hook (ptr64): Hook pointer
   * @return ptr64: Returns jit generated function pointer
   */
  inline void* GetFunctionAddr(void* hook) {
    return __polyhook_GetFunctionAddr(hook);
  }

  using _GetOriginalAddr = void* (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetOriginalAddr __polyhook_GetOriginalAddr;
namespace polyhook {
  /**
   * @brief Get original function address
   * @param hook (ptr64): Hook pointer
   * @return ptr64: Returns original function pointer
   */
  inline void* GetOriginalAddr(void* hook) {
    return __polyhook_GetOriginalAddr(hook);
  }

  using _GetArgumentBool = bool (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentBool __polyhook_GetArgumentBool;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return bool
   */
  inline bool GetArgumentBool(void* params, uint64_t index) {
    return __polyhook_GetArgumentBool(params, index);
  }

  using _GetArgumentInt8 = int8_t (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentInt8 __polyhook_GetArgumentInt8;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return int8
   */
  inline int8_t GetArgumentInt8(void* params, uint64_t index) {
    return __polyhook_GetArgumentInt8(params, index);
  }

  using _GetArgumentUInt8 = uint8_t (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentUInt8 __polyhook_GetArgumentUInt8;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return uint8
   */
  inline uint8_t GetArgumentUInt8(void* params, uint64_t index) {
    return __polyhook_GetArgumentUInt8(params, index);
  }

  using _GetArgumentInt16 = int16_t (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentInt16 __polyhook_GetArgumentInt16;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return int16
   */
  inline int16_t GetArgumentInt16(void* params, uint64_t index) {
    return __polyhook_GetArgumentInt16(params, index);
  }

  using _GetArgumentUInt16 = uint16_t (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentUInt16 __polyhook_GetArgumentUInt16;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return uint16
   */
  inline uint16_t GetArgumentUInt16(void* params, uint64_t index) {
    return __polyhook_GetArgumentUInt16(params, index);
  }

  using _GetArgumentInt32 = int32_t (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentInt32 __polyhook_GetArgumentInt32;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return int32
   */
  inline int32_t GetArgumentInt32(void* params, uint64_t index) {
    return __polyhook_GetArgumentInt32(params, index);
  }

  using _GetArgumentUInt32 = uint32_t (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentUInt32 __polyhook_GetArgumentUInt32;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return uint32
   */
  inline uint32_t GetArgumentUInt32(void* params, uint64_t index) {
    return __polyhook_GetArgumentUInt32(params, index);
  }

  using _GetArgumentInt64 = int64_t (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentInt64 __polyhook_GetArgumentInt64;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return int64
   */
  inline int64_t GetArgumentInt64(void* params, uint64_t index) {
    return __polyhook_GetArgumentInt64(params, index);
  }

  using _GetArgumentUInt64 = uint64_t (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentUInt64 __polyhook_GetArgumentUInt64;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return uint64
   */
  inline uint64_t GetArgumentUInt64(void* params, uint64_t index) {
    return __polyhook_GetArgumentUInt64(params, index);
  }

  using _GetArgumentFloat = float (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentFloat __polyhook_GetArgumentFloat;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return float
   */
  inline float GetArgumentFloat(void* params, uint64_t index) {
    return __polyhook_GetArgumentFloat(params, index);
  }

  using _GetArgumentDouble = double (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentDouble __polyhook_GetArgumentDouble;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return double
   */
  inline double GetArgumentDouble(void* params, uint64_t index) {
    return __polyhook_GetArgumentDouble(params, index);
  }

  using _GetArgumentPointer = void* (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentPointer __polyhook_GetArgumentPointer;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return ptr64
   */
  inline void* GetArgumentPointer(void* params, uint64_t index) {
    return __polyhook_GetArgumentPointer(params, index);
  }

  using _GetArgumentString = plg::string (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentString __polyhook_GetArgumentString;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return string
   */
  inline plg::string GetArgumentString(void* params, uint64_t index) {
    return __polyhook_GetArgumentString(params, index);
  }

  using _GetArgument = plg::any (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgument __polyhook_GetArgument;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @return any
   */
  inline plg::any GetArgument(void* params, uint64_t index) {
    return __polyhook_GetArgument(params, index);
  }

  using _GetReturnBool = bool (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnBool __polyhook_GetReturnBool;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return bool
   */
  inline bool GetReturnBool(void* ret) {
    return __polyhook_GetReturnBool(ret);
  }

  using _GetReturnInt8 = int8_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnInt8 __polyhook_GetReturnInt8;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return int8
   */
  inline int8_t GetReturnInt8(void* ret) {
    return __polyhook_GetReturnInt8(ret);
  }

  using _GetReturnUInt8 = uint8_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnUInt8 __polyhook_GetReturnUInt8;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return uint8
   */
  inline uint8_t GetReturnUInt8(void* ret) {
    return __polyhook_GetReturnUInt8(ret);
  }

  using _GetReturnInt16 = int16_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnInt16 __polyhook_GetReturnInt16;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return int16
   */
  inline int16_t GetReturnInt16(void* ret) {
    return __polyhook_GetReturnInt16(ret);
  }

  using _GetReturnUInt16 = uint16_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnUInt16 __polyhook_GetReturnUInt16;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return uint16
   */
  inline uint16_t GetReturnUInt16(void* ret) {
    return __polyhook_GetReturnUInt16(ret);
  }

  using _GetReturnInt32 = int32_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnInt32 __polyhook_GetReturnInt32;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return int32
   */
  inline int32_t GetReturnInt32(void* ret) {
    return __polyhook_GetReturnInt32(ret);
  }

  using _GetReturnUInt32 = uint32_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnUInt32 __polyhook_GetReturnUInt32;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return uint32
   */
  inline uint32_t GetReturnUInt32(void* ret) {
    return __polyhook_GetReturnUInt32(ret);
  }

  using _GetReturnInt64 = int64_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnInt64 __polyhook_GetReturnInt64;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return int64
   */
  inline int64_t GetReturnInt64(void* ret) {
    return __polyhook_GetReturnInt64(ret);
  }

  using _GetReturnUInt64 = uint64_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnUInt64 __polyhook_GetReturnUInt64;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return uint64
   */
  inline uint64_t GetReturnUInt64(void* ret) {
    return __polyhook_GetReturnUInt64(ret);
  }

  using _GetReturnFloat = float (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnFloat __polyhook_GetReturnFloat;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return float
   */
  inline float GetReturnFloat(void* ret) {
    return __polyhook_GetReturnFloat(ret);
  }

  using _GetReturnDouble = double (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnDouble __polyhook_GetReturnDouble;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return double
   */
  inline double GetReturnDouble(void* ret) {
    return __polyhook_GetReturnDouble(ret);
  }

  using _GetReturnPointer = void* (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnPointer __polyhook_GetReturnPointer;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return ptr64
   */
  inline void* GetReturnPointer(void* ret) {
    return __polyhook_GetReturnPointer(ret);
  }

  using _GetReturnString = plg::string (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturnString __polyhook_GetReturnString;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return string
   */
  inline plg::string GetReturnString(void* ret) {
    return __polyhook_GetReturnString(ret);
  }

  using _GetReturn = plg::any (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetReturn __polyhook_GetReturn;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return any
   */
  inline plg::any GetReturn(void* ret) {
    return __polyhook_GetReturn(ret);
  }

} // namespace polyhook
