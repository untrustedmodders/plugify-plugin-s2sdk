#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: setmethods)

namespace configs {

  using _SetNull = void (*)(void*, const plg::string&);
}
extern "C" PLUGIN_API configs::_SetNull __configs_SetNull;
namespace configs {
  /**
   * @brief Sets a configuration value to null.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   */
  inline void SetNull(void* config, const plg::string& key, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::SetNull", __location);
    return __configs_SetNull(config, key);
  }

  using _SetObject = void (*)(void*, const plg::string&);
}
extern "C" PLUGIN_API configs::_SetObject __configs_SetObject;
namespace configs {
  /**
   * @brief Sets a configuration value to an empty object.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   */
  inline void SetObject(void* config, const plg::string& key, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::SetObject", __location);
    return __configs_SetObject(config, key);
  }

  using _SetArray = void (*)(void*, const plg::string&);
}
extern "C" PLUGIN_API configs::_SetArray __configs_SetArray;
namespace configs {
  /**
   * @brief Sets a configuration value to an empty array.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   */
  inline void SetArray(void* config, const plg::string& key, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::SetArray", __location);
    return __configs_SetArray(config, key);
  }

  using _SetBool = void (*)(void*, const plg::string&, bool);
}
extern "C" PLUGIN_API configs::_SetBool __configs_SetBool;
namespace configs {
  /**
   * @brief Sets a boolean configuration value.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   * @param value (bool): The boolean value to set.
   */
  inline void SetBool(void* config, const plg::string& key, bool value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::SetBool", __location);
    return __configs_SetBool(config, key, value);
  }

  using _SetInt = void (*)(void*, const plg::string&, int64_t);
}
extern "C" PLUGIN_API configs::_SetInt __configs_SetInt;
namespace configs {
  /**
   * @brief Sets an integer configuration value.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   * @param value (int64): The integer value to set.
   */
  inline void SetInt(void* config, const plg::string& key, int64_t value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::SetInt", __location);
    return __configs_SetInt(config, key, value);
  }

  using _SetFloat = void (*)(void*, const plg::string&, double);
}
extern "C" PLUGIN_API configs::_SetFloat __configs_SetFloat;
namespace configs {
  /**
   * @brief Sets a float configuration value.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   * @param value (double): The float value to set.
   */
  inline void SetFloat(void* config, const plg::string& key, double value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::SetFloat", __location);
    return __configs_SetFloat(config, key, value);
  }

  using _SetString = void (*)(void*, const plg::string&, const plg::string&);
}
extern "C" PLUGIN_API configs::_SetString __configs_SetString;
namespace configs {
  /**
   * @brief Sets a string configuration value.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   * @param value (string): The string value to set.
   */
  inline void SetString(void* config, const plg::string& key, const plg::string& value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::SetString", __location);
    return __configs_SetString(config, key, value);
  }

} // namespace configs
