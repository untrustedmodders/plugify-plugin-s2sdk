#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: arraypushmethods)

namespace configs {

  using _PushNull = void (*)(void*);
}
extern "C" PLUGIN_API configs::_PushNull __configs_PushNull;
namespace configs {
  /**
   * @brief Pushes a null value to the current array node.
   * @param config (ptr64): Pointer to the Config object.
   */
  inline void PushNull(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::PushNull", __location);
    return __configs_PushNull(config);
  }

  using _PushBool = void (*)(void*, bool);
}
extern "C" PLUGIN_API configs::_PushBool __configs_PushBool;
namespace configs {
  /**
   * @brief Pushes a boolean value to the current array node.
   * @param config (ptr64): Pointer to the Config object.
   * @param value (bool): The boolean value to push.
   */
  inline void PushBool(void* config, bool value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::PushBool", __location);
    return __configs_PushBool(config, value);
  }

  using _PushInt = void (*)(void*, int64_t);
}
extern "C" PLUGIN_API configs::_PushInt __configs_PushInt;
namespace configs {
  /**
   * @brief Pushes an integer value to the current array node.
   * @param config (ptr64): Pointer to the Config object.
   * @param value (int64): The integer value to push.
   */
  inline void PushInt(void* config, int64_t value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::PushInt", __location);
    return __configs_PushInt(config, value);
  }

  using _PushFloat = void (*)(void*, double);
}
extern "C" PLUGIN_API configs::_PushFloat __configs_PushFloat;
namespace configs {
  /**
   * @brief Pushes a float value to the current array node.
   * @param config (ptr64): Pointer to the Config object.
   * @param value (double): The float value to push.
   */
  inline void PushFloat(void* config, double value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::PushFloat", __location);
    return __configs_PushFloat(config, value);
  }

  using _PushString = void (*)(void*, const plg::string&);
}
extern "C" PLUGIN_API configs::_PushString __configs_PushString;
namespace configs {
  /**
   * @brief Pushes a string value to the current array node.
   * @param config (ptr64): Pointer to the Config object.
   * @param value (string): The string value to push.
   */
  inline void PushString(void* config, const plg::string& value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::PushString", __location);
    return __configs_PushString(config, value);
  }

  using _PushObject = void (*)(void*);
}
extern "C" PLUGIN_API configs::_PushObject __configs_PushObject;
namespace configs {
  /**
   * @brief Pushes an empty object to the current array node.
   * @param config (ptr64): Pointer to the Config object.
   */
  inline void PushObject(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::PushObject", __location);
    return __configs_PushObject(config);
  }

  using _PushArray = void (*)(void*);
}
extern "C" PLUGIN_API configs::_PushArray __configs_PushArray;
namespace configs {
  /**
   * @brief Pushes an empty array to the current array node.
   * @param config (ptr64): Pointer to the Config object.
   */
  inline void PushArray(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::PushArray", __location);
    return __configs_PushArray(config);
  }

} // namespace configs
