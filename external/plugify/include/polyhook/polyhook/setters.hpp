#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from polyhook.pplugin (group: setters)

namespace polyhook {

  using _SetArgumentBool = void (*)(ParametersHandle, uint64_t, bool);
}
extern "C" PLUGIN_API polyhook::_SetArgumentBool __polyhook_SetArgumentBool;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (bool): Value to set
   */
  inline void SetArgumentBool(ParametersHandle params, uint64_t index, bool value) {
    return __polyhook_SetArgumentBool(params, index, value);
  }

  using _SetArgumentInt8 = void (*)(ParametersHandle, uint64_t, int8_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentInt8 __polyhook_SetArgumentInt8;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (int8): Value to set
   */
  inline void SetArgumentInt8(ParametersHandle params, uint64_t index, int8_t value) {
    return __polyhook_SetArgumentInt8(params, index, value);
  }

  using _SetArgumentUInt8 = void (*)(ParametersHandle, uint64_t, uint8_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentUInt8 __polyhook_SetArgumentUInt8;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (uint8): Value to set
   */
  inline void SetArgumentUInt8(ParametersHandle params, uint64_t index, uint8_t value) {
    return __polyhook_SetArgumentUInt8(params, index, value);
  }

  using _SetArgumentInt16 = void (*)(ParametersHandle, uint64_t, int16_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentInt16 __polyhook_SetArgumentInt16;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (int16): Value to set
   */
  inline void SetArgumentInt16(ParametersHandle params, uint64_t index, int16_t value) {
    return __polyhook_SetArgumentInt16(params, index, value);
  }

  using _SetArgumentUInt16 = void (*)(ParametersHandle, uint64_t, uint16_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentUInt16 __polyhook_SetArgumentUInt16;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (uint16): Value to set
   */
  inline void SetArgumentUInt16(ParametersHandle params, uint64_t index, uint16_t value) {
    return __polyhook_SetArgumentUInt16(params, index, value);
  }

  using _SetArgumentInt32 = void (*)(ParametersHandle, uint64_t, int32_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentInt32 __polyhook_SetArgumentInt32;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (int32): Value to set
   */
  inline void SetArgumentInt32(ParametersHandle params, uint64_t index, int32_t value) {
    return __polyhook_SetArgumentInt32(params, index, value);
  }

  using _SetArgumentUInt32 = void (*)(ParametersHandle, uint64_t, uint32_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentUInt32 __polyhook_SetArgumentUInt32;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (uint32): Value to set
   */
  inline void SetArgumentUInt32(ParametersHandle params, uint64_t index, uint32_t value) {
    return __polyhook_SetArgumentUInt32(params, index, value);
  }

  using _SetArgumentInt64 = void (*)(ParametersHandle, uint64_t, int64_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentInt64 __polyhook_SetArgumentInt64;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (int64): Value to set
   */
  inline void SetArgumentInt64(ParametersHandle params, uint64_t index, int64_t value) {
    return __polyhook_SetArgumentInt64(params, index, value);
  }

  using _SetArgumentUInt64 = void (*)(ParametersHandle, uint64_t, uint64_t);
}
extern "C" PLUGIN_API polyhook::_SetArgumentUInt64 __polyhook_SetArgumentUInt64;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (uint64): Value to set
   */
  inline void SetArgumentUInt64(ParametersHandle params, uint64_t index, uint64_t value) {
    return __polyhook_SetArgumentUInt64(params, index, value);
  }

  using _SetArgumentFloat = void (*)(ParametersHandle, uint64_t, float);
}
extern "C" PLUGIN_API polyhook::_SetArgumentFloat __polyhook_SetArgumentFloat;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (float): Value to set
   */
  inline void SetArgumentFloat(ParametersHandle params, uint64_t index, float value) {
    return __polyhook_SetArgumentFloat(params, index, value);
  }

  using _SetArgumentDouble = void (*)(ParametersHandle, uint64_t, double);
}
extern "C" PLUGIN_API polyhook::_SetArgumentDouble __polyhook_SetArgumentDouble;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (double): Value to set
   */
  inline void SetArgumentDouble(ParametersHandle params, uint64_t index, double value) {
    return __polyhook_SetArgumentDouble(params, index, value);
  }

  using _SetArgumentPointer = void (*)(ParametersHandle, uint64_t, void*);
}
extern "C" PLUGIN_API polyhook::_SetArgumentPointer __polyhook_SetArgumentPointer;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (ptr64): Value to set
   */
  inline void SetArgumentPointer(ParametersHandle params, uint64_t index, void* value) {
    return __polyhook_SetArgumentPointer(params, index, value);
  }

  using _SetArgumentString = void (*)(HookHandle, ParametersHandle, uint64_t, const plg::string&);
}
extern "C" PLUGIN_API polyhook::_SetArgumentString __polyhook_SetArgumentString;
namespace polyhook {
  /**
   * @brief Set argument value
   * @param hook (ptr64): Hook pointer
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to set
   * @param value (string): Value to set
   */
  inline void SetArgumentString(HookHandle hook, ParametersHandle params, uint64_t index, const plg::string& value) {
    return __polyhook_SetArgumentString(hook, params, index, value);
  }

  using _SetArgument = void (*)(HookHandle, ParametersHandle, uint64_t, const plg::any&);
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
  inline void SetArgument(HookHandle hook, ParametersHandle params, uint64_t index, const plg::any& value) {
    return __polyhook_SetArgument(hook, params, index, value);
  }

  using _SetReturnBool = void (*)(ReturnHandle, bool);
}
extern "C" PLUGIN_API polyhook::_SetReturnBool __polyhook_SetReturnBool;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (bool): Value to set
   */
  inline void SetReturnBool(ReturnHandle ret, bool value) {
    return __polyhook_SetReturnBool(ret, value);
  }

  using _SetReturnInt8 = void (*)(ReturnHandle, int8_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnInt8 __polyhook_SetReturnInt8;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (int8): Value to set
   */
  inline void SetReturnInt8(ReturnHandle ret, int8_t value) {
    return __polyhook_SetReturnInt8(ret, value);
  }

  using _SetReturnUInt8 = void (*)(ReturnHandle, uint8_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnUInt8 __polyhook_SetReturnUInt8;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (uint8): Value to set
   */
  inline void SetReturnUInt8(ReturnHandle ret, uint8_t value) {
    return __polyhook_SetReturnUInt8(ret, value);
  }

  using _SetReturnInt16 = void (*)(ReturnHandle, int16_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnInt16 __polyhook_SetReturnInt16;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (int16): Value to set
   */
  inline void SetReturnInt16(ReturnHandle ret, int16_t value) {
    return __polyhook_SetReturnInt16(ret, value);
  }

  using _SetReturnUInt16 = void (*)(ReturnHandle, uint16_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnUInt16 __polyhook_SetReturnUInt16;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (uint16): Value to set
   */
  inline void SetReturnUInt16(ReturnHandle ret, uint16_t value) {
    return __polyhook_SetReturnUInt16(ret, value);
  }

  using _SetReturnInt32 = void (*)(ReturnHandle, int32_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnInt32 __polyhook_SetReturnInt32;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (int32): Value to set
   */
  inline void SetReturnInt32(ReturnHandle ret, int32_t value) {
    return __polyhook_SetReturnInt32(ret, value);
  }

  using _SetReturnUInt32 = void (*)(ReturnHandle, uint32_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnUInt32 __polyhook_SetReturnUInt32;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (uint32): Value to set
   */
  inline void SetReturnUInt32(ReturnHandle ret, uint32_t value) {
    return __polyhook_SetReturnUInt32(ret, value);
  }

  using _SetReturnInt64 = void (*)(ReturnHandle, int64_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnInt64 __polyhook_SetReturnInt64;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (int64): Value to set
   */
  inline void SetReturnInt64(ReturnHandle ret, int64_t value) {
    return __polyhook_SetReturnInt64(ret, value);
  }

  using _SetReturnUInt64 = void (*)(ReturnHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_SetReturnUInt64 __polyhook_SetReturnUInt64;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (uint64): Value to set
   */
  inline void SetReturnUInt64(ReturnHandle ret, uint64_t value) {
    return __polyhook_SetReturnUInt64(ret, value);
  }

  using _SetReturnFloat = void (*)(ReturnHandle, float);
}
extern "C" PLUGIN_API polyhook::_SetReturnFloat __polyhook_SetReturnFloat;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (float): Value to set
   */
  inline void SetReturnFloat(ReturnHandle ret, float value) {
    return __polyhook_SetReturnFloat(ret, value);
  }

  using _SetReturnDouble = void (*)(ReturnHandle, double);
}
extern "C" PLUGIN_API polyhook::_SetReturnDouble __polyhook_SetReturnDouble;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (double): Value to set
   */
  inline void SetReturnDouble(ReturnHandle ret, double value) {
    return __polyhook_SetReturnDouble(ret, value);
  }

  using _SetReturnPointer = void (*)(ReturnHandle, void*);
}
extern "C" PLUGIN_API polyhook::_SetReturnPointer __polyhook_SetReturnPointer;
namespace polyhook {
  /**
   * @brief Set return value
   * @param ret (ptr64): Pointer to return structure
   * @param value (ptr64): Value to set
   */
  inline void SetReturnPointer(ReturnHandle ret, void* value) {
    return __polyhook_SetReturnPointer(ret, value);
  }

  using _SetReturnString = void (*)(HookHandle, ReturnHandle, const plg::string&);
}
extern "C" PLUGIN_API polyhook::_SetReturnString __polyhook_SetReturnString;
namespace polyhook {
  /**
   * @brief Set return value
   * @param hook (ptr64): Hook pointer
   * @param ret (ptr64): Pointer to return structure
   * @param value (string): Value to set
   */
  inline void SetReturnString(HookHandle hook, ReturnHandle ret, const plg::string& value) {
    return __polyhook_SetReturnString(hook, ret, value);
  }

  using _SetReturn = void (*)(HookHandle, ReturnHandle, const plg::any&);
}
extern "C" PLUGIN_API polyhook::_SetReturn __polyhook_SetReturn;
namespace polyhook {
  /**
   * @brief Set return value
   * @param hook (ptr64): Hook pointer
   * @param ret (ptr64): Pointer to return structure
   * @param value (any): Value to set
   */
  inline void SetReturn(HookHandle hook, ReturnHandle ret, const plg::any& value) {
    return __polyhook_SetReturn(hook, ret, value);
  }

  using _SetRegisterBool = void (*)(RegistersHandle, RegisterType, bool);
}
extern "C" PLUGIN_API polyhook::_SetRegisterBool __polyhook_SetRegisterBool;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (bool): Value to set
   */
  inline void SetRegisterBool(RegistersHandle registers, RegisterType reg, bool value) {
    return __polyhook_SetRegisterBool(registers, reg, value);
  }

  using _SetRegisterInt8 = void (*)(RegistersHandle, RegisterType, int8_t);
}
extern "C" PLUGIN_API polyhook::_SetRegisterInt8 __polyhook_SetRegisterInt8;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (int8): Value to set
   */
  inline void SetRegisterInt8(RegistersHandle registers, RegisterType reg, int8_t value) {
    return __polyhook_SetRegisterInt8(registers, reg, value);
  }

  using _SetRegisterUInt8 = void (*)(RegistersHandle, RegisterType, uint8_t);
}
extern "C" PLUGIN_API polyhook::_SetRegisterUInt8 __polyhook_SetRegisterUInt8;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (uint8): Value to set
   */
  inline void SetRegisterUInt8(RegistersHandle registers, RegisterType reg, uint8_t value) {
    return __polyhook_SetRegisterUInt8(registers, reg, value);
  }

  using _SetRegisterInt16 = void (*)(RegistersHandle, RegisterType, int16_t);
}
extern "C" PLUGIN_API polyhook::_SetRegisterInt16 __polyhook_SetRegisterInt16;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (int16): Value to set
   */
  inline void SetRegisterInt16(RegistersHandle registers, RegisterType reg, int16_t value) {
    return __polyhook_SetRegisterInt16(registers, reg, value);
  }

  using _SetRegisterUInt16 = void (*)(RegistersHandle, RegisterType, uint16_t);
}
extern "C" PLUGIN_API polyhook::_SetRegisterUInt16 __polyhook_SetRegisterUInt16;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (uint16): Value to set
   */
  inline void SetRegisterUInt16(RegistersHandle registers, RegisterType reg, uint16_t value) {
    return __polyhook_SetRegisterUInt16(registers, reg, value);
  }

  using _SetRegisterInt32 = void (*)(RegistersHandle, RegisterType, int32_t);
}
extern "C" PLUGIN_API polyhook::_SetRegisterInt32 __polyhook_SetRegisterInt32;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (int32): Value to set
   */
  inline void SetRegisterInt32(RegistersHandle registers, RegisterType reg, int32_t value) {
    return __polyhook_SetRegisterInt32(registers, reg, value);
  }

  using _SetRegisterUInt32 = void (*)(RegistersHandle, RegisterType, uint32_t);
}
extern "C" PLUGIN_API polyhook::_SetRegisterUInt32 __polyhook_SetRegisterUInt32;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (uint32): Value to set
   */
  inline void SetRegisterUInt32(RegistersHandle registers, RegisterType reg, uint32_t value) {
    return __polyhook_SetRegisterUInt32(registers, reg, value);
  }

  using _SetRegisterInt64 = void (*)(RegistersHandle, RegisterType, int64_t);
}
extern "C" PLUGIN_API polyhook::_SetRegisterInt64 __polyhook_SetRegisterInt64;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (int64): Value to set
   */
  inline void SetRegisterInt64(RegistersHandle registers, RegisterType reg, int64_t value) {
    return __polyhook_SetRegisterInt64(registers, reg, value);
  }

  using _SetRegisterUInt64 = void (*)(RegistersHandle, RegisterType, uint64_t);
}
extern "C" PLUGIN_API polyhook::_SetRegisterUInt64 __polyhook_SetRegisterUInt64;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (uint64): Value to set
   */
  inline void SetRegisterUInt64(RegistersHandle registers, RegisterType reg, uint64_t value) {
    return __polyhook_SetRegisterUInt64(registers, reg, value);
  }

  using _SetRegisterFloat = void (*)(RegistersHandle, RegisterType, float);
}
extern "C" PLUGIN_API polyhook::_SetRegisterFloat __polyhook_SetRegisterFloat;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (float): Value to set
   */
  inline void SetRegisterFloat(RegistersHandle registers, RegisterType reg, float value) {
    return __polyhook_SetRegisterFloat(registers, reg, value);
  }

  using _SetRegisterDouble = void (*)(RegistersHandle, RegisterType, double);
}
extern "C" PLUGIN_API polyhook::_SetRegisterDouble __polyhook_SetRegisterDouble;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (double): Value to set
   */
  inline void SetRegisterDouble(RegistersHandle registers, RegisterType reg, double value) {
    return __polyhook_SetRegisterDouble(registers, reg, value);
  }

  using _SetRegisterPointer = void (*)(RegistersHandle, RegisterType, void*);
}
extern "C" PLUGIN_API polyhook::_SetRegisterPointer __polyhook_SetRegisterPointer;
namespace polyhook {
  /**
   * @brief Set register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (ptr64): Value to set
   */
  inline void SetRegisterPointer(RegistersHandle registers, RegisterType reg, void* value) {
    return __polyhook_SetRegisterPointer(registers, reg, value);
  }

  using _SetRegisterString = void (*)(HookHandle, RegistersHandle, RegisterType, const plg::string&);
}
extern "C" PLUGIN_API polyhook::_SetRegisterString __polyhook_SetRegisterString;
namespace polyhook {
  /**
   * @brief Set register value
   * @param hook (ptr64): Hook pointer
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (string): Value to set
   */
  inline void SetRegisterString(HookHandle hook, RegistersHandle registers, RegisterType reg, const plg::string& value) {
    return __polyhook_SetRegisterString(hook, registers, reg, value);
  }

  using _SetRegister = void (*)(HookHandle, RegistersHandle, RegisterType, const plg::any&);
}
extern "C" PLUGIN_API polyhook::_SetRegister __polyhook_SetRegister;
namespace polyhook {
  /**
   * @brief Set register value
   * @param hook (ptr64): Hook pointer
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to set
   * @param value (any): Value to set
   */
  inline void SetRegister(HookHandle hook, RegistersHandle registers, RegisterType reg, const plg::any& value) {
    return __polyhook_SetRegister(hook, registers, reg, value);
  }

} // namespace polyhook
