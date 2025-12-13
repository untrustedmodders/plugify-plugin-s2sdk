#pragma once

#include "enums.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from polyhook.pplugin (group: setters)

namespace polyhook {

  using _SetArgumentBool = void (*)(void*, uint64_t, bool);
}
extern "C" PLUGIN_API polyhook::_SetArgumentBool __polyhook_SetArgumentBool;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (bool): Value to set
   */
  inline void SetArgumentBool(void* params, uint64_t index, bool value) {
    return __polyhook_SetArgumentBool(params, index, value);
  }

  using _SetArgumentInt8 = void (*)(void*, uint64_t, int8_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentInt8 __polyhook_SetArgumentInt8;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (int8): Value to set
   */
  inline void SetArgumentInt8(void* params, uint64_t index, int8_t value) {
    return __polyhook_SetArgumentInt8(params, index, value);
  }

  using _SetArgumentUInt8 = void (*)(void*, uint64_t, uint8_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentUInt8 __polyhook_SetArgumentUInt8;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (uint8): Value to set
   */
  inline void SetArgumentUInt8(void* params, uint64_t index, uint8_t value) {
    return __polyhook_SetArgumentUInt8(params, index, value);
  }

  using _SetArgumentInt16 = void (*)(void*, uint64_t, int16_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentInt16 __polyhook_SetArgumentInt16;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (int16): Value to set
   */
  inline void SetArgumentInt16(void* params, uint64_t index, int16_t value) {
    return __polyhook_SetArgumentInt16(params, index, value);
  }

  using _SetArgumentUInt16 = void (*)(void*, uint64_t, uint16_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentUInt16 __polyhook_SetArgumentUInt16;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (uint16): Value to set
   */
  inline void SetArgumentUInt16(void* params, uint64_t index, uint16_t value) {
    return __polyhook_SetArgumentUInt16(params, index, value);
  }

  using _SetArgumentInt32 = void (*)(void*, uint64_t, int32_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentInt32 __polyhook_SetArgumentInt32;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (int32): Value to set
   */
  inline void SetArgumentInt32(void* params, uint64_t index, int32_t value) {
    return __polyhook_SetArgumentInt32(params, index, value);
  }

  using _SetArgumentUInt32 = void (*)(void*, uint64_t, uint32_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentUInt32 __polyhook_SetArgumentUInt32;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (uint32): Value to set
   */
  inline void SetArgumentUInt32(void* params, uint64_t index, uint32_t value) {
    return __polyhook_SetArgumentUInt32(params, index, value);
  }

  using _SetArgumentInt64 = void (*)(void*, uint64_t, int64_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentInt64 __polyhook_SetArgumentInt64;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (int64): Value to set
   */
  inline void SetArgumentInt64(void* params, uint64_t index, int64_t value) {
    return __polyhook_SetArgumentInt64(params, index, value);
  }

  using _SetArgumentUInt64 = void (*)(void*, uint64_t, uint64_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentUInt64 __polyhook_SetArgumentUInt64;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (uint64): Value to set
   */
  inline void SetArgumentUInt64(void* params, uint64_t index, uint64_t value) {
    return __polyhook_SetArgumentUInt64(params, index, value);
  }

  using _SetArgumentFloat = void (*)(void*, uint64_t, float);
}
extern "C" PLUGIN_API polyhook::_SetArgumentFloat __polyhook_SetArgumentFloat;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (float): Value to set
   */
  inline void SetArgumentFloat(void* params, uint64_t index, float value) {
    return __polyhook_SetArgumentFloat(params, index, value);
  }

  using _SetArgumentDouble = void (*)(void*, uint64_t, double);
}
extern "C" PLUGIN_API polyhook::_SetArgumentDouble __polyhook_SetArgumentDouble;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (double): Value to set
   */
  inline void SetArgumentDouble(void* params, uint64_t index, double value) {
    return __polyhook_SetArgumentDouble(params, index, value);
  }

  using _SetArgumentPointer = void (*)(void*, uint64_t, void*);
}
extern "C" PLUGIN_API polyhook::_SetArgumentPointer __polyhook_SetArgumentPointer;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (ptr64): Value to set
   */
  inline void SetArgumentPointer(void* params, uint64_t index, void* value) {
    return __polyhook_SetArgumentPointer(params, index, value);
  }

  using _SetArgumentString = void (*)(void*, void*, uint64_t, const plg::string&);
}
extern "C" PLUGIN_API polyhook::_SetArgumentString __polyhook_SetArgumentString;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param hook (ptr64): Hook pointer
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (string): Value to set
   */
  inline void SetArgumentString(void* hook, void* params, uint64_t index, const plg::string& value) {
    return __polyhook_SetArgumentString(hook, params, index, value);
  }

  using _SetArgument = void (*)(void*, void*, uint64_t, const plg::any&);
}
extern "C" PLUGIN_API polyhook::_SetArgument __polyhook_SetArgument;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param hook (ptr64): Hook pointer
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Value to set
   * @param value (any): Value to set
   */
  inline void SetArgument(void* hook, void* params, uint64_t index, const plg::any& value) {
    return __polyhook_SetArgument(hook, params, index, value);
  }

  using _SetReturnBool = void (*)(void*, bool);
}
extern "C" PLUGIN_API polyhook::_SetReturnBool __polyhook_SetReturnBool;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (bool): Value to set
   */
  inline void SetReturnBool(void* ret, bool value) {
    return __polyhook_SetReturnBool(ret, value);
  }

  using _SetReturnInt8 = void (*)(void*, int8_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnInt8 __polyhook_SetReturnInt8;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (int8): Value to set
   */
  inline void SetReturnInt8(void* ret, int8_t value) {
    return __polyhook_SetReturnInt8(ret, value);
  }

  using _SetReturnUInt8 = void (*)(void*, uint8_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnUInt8 __polyhook_SetReturnUInt8;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (uint8): Value to set
   */
  inline void SetReturnUInt8(void* ret, uint8_t value) {
    return __polyhook_SetReturnUInt8(ret, value);
  }

  using _SetReturnInt16 = void (*)(void*, int16_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnInt16 __polyhook_SetReturnInt16;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (int16): Value to set
   */
  inline void SetReturnInt16(void* ret, int16_t value) {
    return __polyhook_SetReturnInt16(ret, value);
  }

  using _SetReturnUInt16 = void (*)(void*, uint16_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnUInt16 __polyhook_SetReturnUInt16;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (uint16): Value to set
   */
  inline void SetReturnUInt16(void* ret, uint16_t value) {
    return __polyhook_SetReturnUInt16(ret, value);
  }

  using _SetReturnInt32 = void (*)(void*, int32_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnInt32 __polyhook_SetReturnInt32;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (int32): Value to set
   */
  inline void SetReturnInt32(void* ret, int32_t value) {
    return __polyhook_SetReturnInt32(ret, value);
  }

  using _SetReturnUInt32 = void (*)(void*, uint32_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnUInt32 __polyhook_SetReturnUInt32;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (uint32): Value to set
   */
  inline void SetReturnUInt32(void* ret, uint32_t value) {
    return __polyhook_SetReturnUInt32(ret, value);
  }

  using _SetReturnInt64 = void (*)(void*, int64_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnInt64 __polyhook_SetReturnInt64;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (int64): Value to set
   */
  inline void SetReturnInt64(void* ret, int64_t value) {
    return __polyhook_SetReturnInt64(ret, value);
  }

  using _SetReturnUInt64 = void (*)(void*, uint64_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnUInt64 __polyhook_SetReturnUInt64;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (uint64): Value to set
   */
  inline void SetReturnUInt64(void* ret, uint64_t value) {
    return __polyhook_SetReturnUInt64(ret, value);
  }

  using _SetReturnFloat = void (*)(void*, float);
}
extern "C" PLUGIN_API polyhook::_SetReturnFloat __polyhook_SetReturnFloat;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (float): Value to set
   */
  inline void SetReturnFloat(void* ret, float value) {
    return __polyhook_SetReturnFloat(ret, value);
  }

  using _SetReturnDouble = void (*)(void*, double);
}
extern "C" PLUGIN_API polyhook::_SetReturnDouble __polyhook_SetReturnDouble;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (double): Value to set
   */
  inline void SetReturnDouble(void* ret, double value) {
    return __polyhook_SetReturnDouble(ret, value);
  }

  using _SetReturnPointer = void (*)(void*, void*);
}
extern "C" PLUGIN_API polyhook::_SetReturnPointer __polyhook_SetReturnPointer;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (ptr64): Value to set
   */
  inline void SetReturnPointer(void* ret, void* value) {
    return __polyhook_SetReturnPointer(ret, value);
  }

  using _SetReturnString = void (*)(void*, void*, const plg::string&);
}
extern "C" PLUGIN_API polyhook::_SetReturnString __polyhook_SetReturnString;
namespace polyhook {
  /**
   * @brief Set return value
   * @param hook (ptr64): Hook pointer
   * @param ret (ptr64): Pointer to return structure
   * @param value (string): Value to set
   */
  inline void SetReturnString(void* hook, void* ret, const plg::string& value) {
    return __polyhook_SetReturnString(hook, ret, value);
  }

  using _SetReturn = void (*)(void*, void*, const plg::any&);
}
extern "C" PLUGIN_API polyhook::_SetReturn __polyhook_SetReturn;
namespace polyhook {
  /**
   * @brief Set return value
   * @param hook (ptr64): Hook pointer
   * @param ret (ptr64): Pointer to return structure
   * @param value (any): Value to set
   */
  inline void SetReturn(void* hook, void* ret, const plg::any& value) {
    return __polyhook_SetReturn(hook, ret, value);
  }

} // namespace polyhook
