#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from polyhook.pplugin (group: getters)

namespace polyhook {

  using _GetFunctionAddr = void* (*)(HookHandle);
}
extern "C" PLUGIN_API polyhook::_GetFunctionAddr __polyhook_GetFunctionAddr;
namespace polyhook {
  /**
   * @brief Get generated function address
   * @param hook (ptr64): Hook pointer
   * @return ptr64: Returns jit generated function pointer
   */
  inline void* GetFunctionAddr(HookHandle hook) {
    return __polyhook_GetFunctionAddr(hook);
  }

  using _GetOriginalAddr = void* (*)(HookHandle);
}
extern "C" PLUGIN_API polyhook::_GetOriginalAddr __polyhook_GetOriginalAddr;
namespace polyhook {
  /**
   * @brief Get original function address
   * @param hook (ptr64): Hook pointer
   * @return ptr64: Returns original function pointer
   */
  inline void* GetOriginalAddr(HookHandle hook) {
    return __polyhook_GetOriginalAddr(hook);
  }

  using _GetArgumentBool = bool (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentBool __polyhook_GetArgumentBool;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return bool
   */
  inline bool GetArgumentBool(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentBool(params, index);
  }

  using _GetArgumentInt8 = int8_t (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentInt8 __polyhook_GetArgumentInt8;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return int8
   */
  inline int8_t GetArgumentInt8(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentInt8(params, index);
  }

  using _GetArgumentUInt8 = uint8_t (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentUInt8 __polyhook_GetArgumentUInt8;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return uint8
   */
  inline uint8_t GetArgumentUInt8(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentUInt8(params, index);
  }

  using _GetArgumentInt16 = int16_t (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentInt16 __polyhook_GetArgumentInt16;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return int16
   */
  inline int16_t GetArgumentInt16(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentInt16(params, index);
  }

  using _GetArgumentUInt16 = uint16_t (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentUInt16 __polyhook_GetArgumentUInt16;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return uint16
   */
  inline uint16_t GetArgumentUInt16(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentUInt16(params, index);
  }

  using _GetArgumentInt32 = int32_t (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentInt32 __polyhook_GetArgumentInt32;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return int32
   */
  inline int32_t GetArgumentInt32(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentInt32(params, index);
  }

  using _GetArgumentUInt32 = uint32_t (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentUInt32 __polyhook_GetArgumentUInt32;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return uint32
   */
  inline uint32_t GetArgumentUInt32(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentUInt32(params, index);
  }

  using _GetArgumentInt64 = int64_t (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentInt64 __polyhook_GetArgumentInt64;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return int64
   */
  inline int64_t GetArgumentInt64(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentInt64(params, index);
  }

  using _GetArgumentUInt64 = uint64_t (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentUInt64 __polyhook_GetArgumentUInt64;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return uint64
   */
  inline uint64_t GetArgumentUInt64(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentUInt64(params, index);
  }

  using _GetArgumentFloat = float (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentFloat __polyhook_GetArgumentFloat;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return float
   */
  inline float GetArgumentFloat(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentFloat(params, index);
  }

  using _GetArgumentDouble = double (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentDouble __polyhook_GetArgumentDouble;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return double
   */
  inline double GetArgumentDouble(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentDouble(params, index);
  }

  using _GetArgumentPointer = void* (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentPointer __polyhook_GetArgumentPointer;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return ptr64
   */
  inline void* GetArgumentPointer(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentPointer(params, index);
  }

  using _GetArgumentString = plg::string (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgumentString __polyhook_GetArgumentString;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return string
   */
  inline plg::string GetArgumentString(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgumentString(params, index);
  }

  using _GetArgument = plg::any (*)(ParametersHandle, uint64_t);
}
extern "C" PLUGIN_API polyhook::_GetArgument __polyhook_GetArgument;
namespace polyhook {
  /**
   * @brief Get argument value
   * @param params (ptr64): Pointer to params structure
   * @param index (uint64): Index to get
   * @return any
   */
  inline plg::any GetArgument(ParametersHandle params, uint64_t index) {
    return __polyhook_GetArgument(params, index);
  }

  using _GetReturnBool = bool (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnBool __polyhook_GetReturnBool;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return bool
   */
  inline bool GetReturnBool(ReturnHandle ret) {
    return __polyhook_GetReturnBool(ret);
  }

  using _GetReturnInt8 = int8_t (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnInt8 __polyhook_GetReturnInt8;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return int8
   */
  inline int8_t GetReturnInt8(ReturnHandle ret) {
    return __polyhook_GetReturnInt8(ret);
  }

  using _GetReturnUInt8 = uint8_t (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnUInt8 __polyhook_GetReturnUInt8;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return uint8
   */
  inline uint8_t GetReturnUInt8(ReturnHandle ret) {
    return __polyhook_GetReturnUInt8(ret);
  }

  using _GetReturnInt16 = int16_t (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnInt16 __polyhook_GetReturnInt16;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return int16
   */
  inline int16_t GetReturnInt16(ReturnHandle ret) {
    return __polyhook_GetReturnInt16(ret);
  }

  using _GetReturnUInt16 = uint16_t (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnUInt16 __polyhook_GetReturnUInt16;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return uint16
   */
  inline uint16_t GetReturnUInt16(ReturnHandle ret) {
    return __polyhook_GetReturnUInt16(ret);
  }

  using _GetReturnInt32 = int32_t (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnInt32 __polyhook_GetReturnInt32;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return int32
   */
  inline int32_t GetReturnInt32(ReturnHandle ret) {
    return __polyhook_GetReturnInt32(ret);
  }

  using _GetReturnUInt32 = uint32_t (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnUInt32 __polyhook_GetReturnUInt32;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return uint32
   */
  inline uint32_t GetReturnUInt32(ReturnHandle ret) {
    return __polyhook_GetReturnUInt32(ret);
  }

  using _GetReturnInt64 = int64_t (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnInt64 __polyhook_GetReturnInt64;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return int64
   */
  inline int64_t GetReturnInt64(ReturnHandle ret) {
    return __polyhook_GetReturnInt64(ret);
  }

  using _GetReturnUInt64 = uint64_t (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnUInt64 __polyhook_GetReturnUInt64;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return uint64
   */
  inline uint64_t GetReturnUInt64(ReturnHandle ret) {
    return __polyhook_GetReturnUInt64(ret);
  }

  using _GetReturnFloat = float (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnFloat __polyhook_GetReturnFloat;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return float
   */
  inline float GetReturnFloat(ReturnHandle ret) {
    return __polyhook_GetReturnFloat(ret);
  }

  using _GetReturnDouble = double (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnDouble __polyhook_GetReturnDouble;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return double
   */
  inline double GetReturnDouble(ReturnHandle ret) {
    return __polyhook_GetReturnDouble(ret);
  }

  using _GetReturnPointer = void* (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnPointer __polyhook_GetReturnPointer;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return ptr64
   */
  inline void* GetReturnPointer(ReturnHandle ret) {
    return __polyhook_GetReturnPointer(ret);
  }

  using _GetReturnString = plg::string (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturnString __polyhook_GetReturnString;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return string
   */
  inline plg::string GetReturnString(ReturnHandle ret) {
    return __polyhook_GetReturnString(ret);
  }

  using _GetReturn = plg::any (*)(ReturnHandle);
}
extern "C" PLUGIN_API polyhook::_GetReturn __polyhook_GetReturn;
namespace polyhook {
  /**
   * @brief Get return value
   * @param ret (ptr64): Pointer to return structure
   * @return any
   */
  inline plg::any GetReturn(ReturnHandle ret) {
    return __polyhook_GetReturn(ret);
  }

  using _GetRegisterBool = bool (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterBool __polyhook_GetRegisterBool;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return bool
   */
  inline bool GetRegisterBool(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterBool(registers, reg);
  }

  using _GetRegisterInt8 = int8_t (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterInt8 __polyhook_GetRegisterInt8;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return int8
   */
  inline int8_t GetRegisterInt8(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterInt8(registers, reg);
  }

  using _GetRegisterUInt8 = uint8_t (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterUInt8 __polyhook_GetRegisterUInt8;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return uint8
   */
  inline uint8_t GetRegisterUInt8(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterUInt8(registers, reg);
  }

  using _GetRegisterInt16 = int16_t (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterInt16 __polyhook_GetRegisterInt16;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return int16
   */
  inline int16_t GetRegisterInt16(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterInt16(registers, reg);
  }

  using _GetRegisterUInt16 = uint16_t (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterUInt16 __polyhook_GetRegisterUInt16;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return uint16
   */
  inline uint16_t GetRegisterUInt16(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterUInt16(registers, reg);
  }

  using _GetRegisterInt32 = int32_t (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterInt32 __polyhook_GetRegisterInt32;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return int32
   */
  inline int32_t GetRegisterInt32(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterInt32(registers, reg);
  }

  using _GetRegisterUInt32 = uint32_t (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterUInt32 __polyhook_GetRegisterUInt32;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return uint32
   */
  inline uint32_t GetRegisterUInt32(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterUInt32(registers, reg);
  }

  using _GetRegisterInt64 = int64_t (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterInt64 __polyhook_GetRegisterInt64;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return int64
   */
  inline int64_t GetRegisterInt64(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterInt64(registers, reg);
  }

  using _GetRegisterUInt64 = uint64_t (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterUInt64 __polyhook_GetRegisterUInt64;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return uint64
   */
  inline uint64_t GetRegisterUInt64(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterUInt64(registers, reg);
  }

  using _GetRegisterFloat = float (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterFloat __polyhook_GetRegisterFloat;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return float
   */
  inline float GetRegisterFloat(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterFloat(registers, reg);
  }

  using _GetRegisterDouble = double (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterDouble __polyhook_GetRegisterDouble;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return double
   */
  inline double GetRegisterDouble(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterDouble(registers, reg);
  }

  using _GetRegisterPointer = void* (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterPointer __polyhook_GetRegisterPointer;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return ptr64
   */
  inline void* GetRegisterPointer(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterPointer(registers, reg);
  }

  using _GetRegisterString = plg::string (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegisterString __polyhook_GetRegisterString;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return string
   */
  inline plg::string GetRegisterString(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegisterString(registers, reg);
  }

  using _GetRegister = plg::any (*)(RegistersHandle, RegisterType);
}
extern "C" PLUGIN_API polyhook::_GetRegister __polyhook_GetRegister;
namespace polyhook {
  /**
   * @brief Get register value
   * @param registers (ptr64): Pointer to registers structure
   * @param reg (uint64): Register to get
   * @return any
   */
  inline plg::any GetRegister(RegistersHandle registers, RegisterType reg) {
    return __polyhook_GetRegister(registers, reg);
  }

} // namespace polyhook
