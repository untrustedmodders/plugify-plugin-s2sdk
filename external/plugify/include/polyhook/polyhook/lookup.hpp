#pragma once

#include "enums.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from polyhook.pplugin (group: lookup)

namespace polyhook {

  using _FindDetour = void* (*)(void*);
}
extern "C" PLUGIN_API polyhook::_FindDetour __polyhook_FindDetour;
namespace polyhook {
  /**
   * @brief Attempts to find existing detour hook
   * @param pFunc (ptr64): Function address
   * @return ptr64: Returns hook pointer
   */
  inline void* FindDetour(void* pFunc) {
    return __polyhook_FindDetour(pFunc);
  }

  using _FindVirtual = void* (*)(void*, int32_t);
}
extern "C" PLUGIN_API polyhook::_FindVirtual __polyhook_FindVirtual;
namespace polyhook {
  /**
   * @brief Attempts to find existing virtual hook
   * @param pClass (ptr64): Object pointer
   * @param index (int32): Value to set
   * @return ptr64: Returns hook pointer
   */
  inline void* FindVirtual(void* pClass, int32_t index) {
    return __polyhook_FindVirtual(pClass, index);
  }

  using _FindVirtual2 = void* (*)(void*, void*);
}
extern "C" PLUGIN_API polyhook::_FindVirtual2 __polyhook_FindVirtual2;
namespace polyhook {
  /**
   * @brief Attempts to find existing detour hook by pointer
   * @param pClass (ptr64): Object pointer
   * @param pFunc (ptr64): Function address
   * @return ptr64: Returns hook pointer
   */
  inline void* FindVirtual2(void* pClass, void* pFunc) {
    return __polyhook_FindVirtual2(pClass, pFunc);
  }

  using _GetVirtualIndex = int32_t (*)(void*);
}
extern "C" PLUGIN_API polyhook::_GetVirtualIndex __polyhook_GetVirtualIndex;
namespace polyhook {
  /**
   * @brief Attempts to find virtual table index of virtual function
   * @param pFunc (ptr64): Function address
   * @return int32: Virtual table index
   */
  inline int32_t GetVirtualIndex(void* pFunc) {
    return __polyhook_GetVirtualIndex(pFunc);
  }

} // namespace polyhook
