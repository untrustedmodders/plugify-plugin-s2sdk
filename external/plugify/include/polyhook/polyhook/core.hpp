#pragma once

#include "enums.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from polyhook.pplugin (group: core)

namespace polyhook {

  using _HookDetour = void* (*)(void*, DataType, const plg::vector<DataType>&, int32_t);
}
extern "C" PLUGIN_API polyhook::_HookDetour __polyhook_HookDetour;
namespace polyhook {
  /**
   * @brief Sets a detour hook
   * @param pFunc (ptr64): Function address
   * @param returnType (uint8): Return type
   * @param arguments (uint8[]): Arguments type array
   * @param varIndex (int32): Index of a first variadic argument or -1
   * @return ptr64: Returns hook pointer
   */
  inline void* HookDetour(void* pFunc, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
    return __polyhook_HookDetour(pFunc, returnType, arguments, varIndex);
  }

  using _HookVirtualTable = void* (*)(void*, int32_t, DataType, const plg::vector<DataType>&, int32_t);
}
extern "C" PLUGIN_API polyhook::_HookVirtualTable __polyhook_HookVirtualTable;
namespace polyhook {
  /**
   * @brief Sets a virtual table hook
   * @param pClass (ptr64): Object pointer
   * @param index (int32): Vtable offset
   * @param returnType (uint8): Return type
   * @param arguments (uint8[]): Arguments type array
   * @param varIndex (int32): Index of a first variadic argument or -1
   * @return ptr64: Returns hook pointer
   */
  inline void* HookVirtualTable(void* pClass, int32_t index, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
    return __polyhook_HookVirtualTable(pClass, index, returnType, arguments, varIndex);
  }

  using _HookVirtualTable2 = void* (*)(void*, void*, DataType, const plg::vector<DataType>&, int32_t);
}
extern "C" PLUGIN_API polyhook::_HookVirtualTable2 __polyhook_HookVirtualTable2;
namespace polyhook {
  /**
   * @brief Sets a virtual table hook by pointer
   * @param pClass (ptr64): Object pointer
   * @param pFunc (ptr64): Vtable member function address
   * @param returnType (uint8): Return type
   * @param arguments (uint8[]): Arguments type array
   * @param varIndex (int32): Index of a first variadic argument or -1
   * @return ptr64: Returns hook pointer
   */
  inline void* HookVirtualTable2(void* pClass, void* pFunc, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
    return __polyhook_HookVirtualTable2(pClass, pFunc, returnType, arguments, varIndex);
  }

  using _HookVirtualFunc = void* (*)(void*, int32_t, DataType, const plg::vector<DataType>&, int32_t);
}
extern "C" PLUGIN_API polyhook::_HookVirtualFunc __polyhook_HookVirtualFunc;
namespace polyhook {
  /**
   * @brief Sets a virtual function hook
   * @param pClass (ptr64): Object pointer
   * @param index (int32): Vtable offset
   * @param returnType (uint8): Return type
   * @param arguments (uint8[]): Arguments type array
   * @param varIndex (int32): Index of a first variadic argument or -1
   * @return ptr64: Returns hook pointer
   */
  inline void* HookVirtualFunc(void* pClass, int32_t index, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
    return __polyhook_HookVirtualFunc(pClass, index, returnType, arguments, varIndex);
  }

  using _HookVirtualFunc2 = void* (*)(void*, void*, DataType, const plg::vector<DataType>&, int32_t);
}
extern "C" PLUGIN_API polyhook::_HookVirtualFunc2 __polyhook_HookVirtualFunc2;
namespace polyhook {
  /**
   * @brief Sets a virtual function hook by pointer
   * @param pClass (ptr64): Object pointer
   * @param pFunc (ptr64): Vtable member function address
   * @param returnType (uint8): Return type
   * @param arguments (uint8[]): Arguments type array
   * @param varIndex (int32): Index of a first variadic argument or -1
   * @return ptr64: Returns hook pointer
   */
  inline void* HookVirtualFunc2(void* pClass, void* pFunc, DataType returnType, const plg::vector<DataType>& arguments, int32_t varIndex = -1) {
    return __polyhook_HookVirtualFunc2(pClass, pFunc, returnType, arguments, varIndex);
  }

  using _UnhookDetour = bool (*)(void*);
}
extern "C" PLUGIN_API polyhook::_UnhookDetour __polyhook_UnhookDetour;
namespace polyhook {
  /**
   * @brief Removes a detour hook
   * @param pFunc (ptr64): Function address
   * @return bool: Returns true on success, false otherwise
   */
  inline bool UnhookDetour(void* pFunc) {
    return __polyhook_UnhookDetour(pFunc);
  }

  using _UnhookVirtualTable = bool (*)(void*, int32_t);
}
extern "C" PLUGIN_API polyhook::_UnhookVirtualTable __polyhook_UnhookVirtualTable;
namespace polyhook {
  /**
   * @brief Removes a virtual hook table
   * @param pClass (ptr64): Object pointer
   * @param index (int32): Value to set
   * @return bool: Returns true on success, false otherwise
   */
  inline bool UnhookVirtualTable(void* pClass, int32_t index) {
    return __polyhook_UnhookVirtualTable(pClass, index);
  }

  using _UnhookVirtualTable2 = bool (*)(void*, void*);
}
extern "C" PLUGIN_API polyhook::_UnhookVirtualTable2 __polyhook_UnhookVirtualTable2;
namespace polyhook {
  /**
   * @brief Removes a virtual table hook by pointer
   * @param pClass (ptr64): Object pointer
   * @param pFunc (ptr64): Vtable member function address
   * @return bool: Returns true on success, false otherwise
   */
  inline bool UnhookVirtualTable2(void* pClass, void* pFunc) {
    return __polyhook_UnhookVirtualTable2(pClass, pFunc);
  }

  using _UnhookVirtualFunc = bool (*)(void*, int32_t);
}
extern "C" PLUGIN_API polyhook::_UnhookVirtualFunc __polyhook_UnhookVirtualFunc;
namespace polyhook {
  /**
   * @brief Removes a virtual function table
   * @param pClass (ptr64): Object pointer
   * @param index (int32): Value to set
   * @return bool: Returns true on success, false otherwise
   */
  inline bool UnhookVirtualFunc(void* pClass, int32_t index) {
    return __polyhook_UnhookVirtualFunc(pClass, index);
  }

  using _UnhookVirtualFunc2 = bool (*)(void*, void*);
}
extern "C" PLUGIN_API polyhook::_UnhookVirtualFunc2 __polyhook_UnhookVirtualFunc2;
namespace polyhook {
  /**
   * @brief Removes a virtual function hook by pointer
   * @param pClass (ptr64): Object pointer
   * @param pFunc (ptr64): Vtable member function address
   * @return bool: Returns true on success, false otherwise
   */
  inline bool UnhookVirtualFunc2(void* pClass, void* pFunc) {
    return __polyhook_UnhookVirtualFunc2(pClass, pFunc);
  }

  using _UnhookAll = void (*)();
}
extern "C" PLUGIN_API polyhook::_UnhookAll __polyhook_UnhookAll;
namespace polyhook {
  /**
   * @brief Removes all previously set hooks
   */
  inline void UnhookAll() {
    return __polyhook_UnhookAll();
  }

  using _UnhookAllVirtual = void (*)(void*);
}
extern "C" PLUGIN_API polyhook::_UnhookAllVirtual __polyhook_UnhookAllVirtual;
namespace polyhook {
  /**
   * @brief Removes all previously set hooks on the object
   * @param pClass (ptr64): Object pointer
   */
  inline void UnhookAllVirtual(void* pClass) {
    return __polyhook_UnhookAllVirtual(pClass);
  }

  using _AddCallback = bool (*)(void*, CallbackType, CallbackHandler);
}
extern "C" PLUGIN_API polyhook::_AddCallback __polyhook_AddCallback;
namespace polyhook {
  /**
   * @brief Adds a callback to existing hook
   * @param hook (ptr64): Hook pointer
   * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @param handler (function): Callback function which trigger by hook.
   * @return bool: Returns true on success, false otherwise
   */
  inline bool AddCallback(void* hook, CallbackType type, CallbackHandler handler) {
    return __polyhook_AddCallback(hook, type, handler);
  }

  using _AddCallback2 = bool (*)(void*, CallbackType, CallbackHandler, int32_t);
}
extern "C" PLUGIN_API polyhook::_AddCallback2 __polyhook_AddCallback2;
namespace polyhook {
  /**
   * @brief Adds a callback to existing hook
   * @param hook (ptr64): Hook pointer
   * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @param handler (function): Callback function which trigger by hook.
   * @param priority (int32): Priority of callback among others
   * @return bool: Returns true on success, false otherwise
   */
  inline bool AddCallback2(void* hook, CallbackType type, CallbackHandler handler, int32_t priority) {
    return __polyhook_AddCallback2(hook, type, handler, priority);
  }

  using _RemoveCallback = bool (*)(void*, CallbackType, CallbackHandler);
}
extern "C" PLUGIN_API polyhook::_RemoveCallback __polyhook_RemoveCallback;
namespace polyhook {
  /**
   * @brief Removes a callback from existing hook
   * @param hook (ptr64): Hook pointer
   * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @param handler (function): Callback function which trigger by hook.
   * @return bool: Returns true on success, false otherwise
   */
  inline bool RemoveCallback(void* hook, CallbackType type, CallbackHandler handler) {
    return __polyhook_RemoveCallback(hook, type, handler);
  }

  using _IsCallbackRegistered = bool (*)(void*, CallbackType, CallbackHandler);
}
extern "C" PLUGIN_API polyhook::_IsCallbackRegistered __polyhook_IsCallbackRegistered;
namespace polyhook {
  /**
   * @brief Checks that a callback exists on existing hook
   * @param hook (ptr64): Hook pointer
   * @param type (uint8): Whether the hook was in post mode (after processing) or pre mode (before processing).
   * @param handler (function): Callback function which trigger by hook.
   * @return bool: Returns true on success, false otherwise
   */
  inline bool IsCallbackRegistered(void* hook, CallbackType type, CallbackHandler handler) {
    return __polyhook_IsCallbackRegistered(hook, type, handler);
  }

  using _AreCallbacksRegistered = bool (*)(void*);
}
extern "C" PLUGIN_API polyhook::_AreCallbacksRegistered __polyhook_AreCallbacksRegistered;
namespace polyhook {
  /**
   * @brief Checks that a any callback exists on existing hook
   * @param hook (ptr64): Hook pointer
   * @return bool: Returns true on success, false otherwise
   */
  inline bool AreCallbacksRegistered(void* hook) {
    return __polyhook_AreCallbacksRegistered(hook);
  }

} // namespace polyhook
