#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: trysetmethods)

namespace configs {

  using _TrySetFromBool = bool (*)(void*, const plg::string&, bool);
}
extern "C" PLUGIN_API configs::_TrySetFromBool __configs_TrySetFromBool;
namespace configs {
  /**
   * @brief Attempts to set a configuration value from a boolean, with type conversion if needed.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   * @param value (bool): The boolean value to set.
   * @return bool: True if the value was set successfully, false otherwise.
   */
  inline bool TrySetFromBool(void* config, const plg::string& key, bool value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::TrySetFromBool", __location);
    return __configs_TrySetFromBool(config, key, value);
  }

  using _TrySetFromInt = bool (*)(void*, const plg::string&, int64_t);
}
extern "C" PLUGIN_API configs::_TrySetFromInt __configs_TrySetFromInt;
namespace configs {
  /**
   * @brief Attempts to set a configuration value from an integer, with type conversion if needed.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   * @param value (int64): The integer value to set.
   * @return bool: True if the value was set successfully, false otherwise.
   */
  inline bool TrySetFromInt(void* config, const plg::string& key, int64_t value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::TrySetFromInt", __location);
    return __configs_TrySetFromInt(config, key, value);
  }

  using _TrySetFromFloat = bool (*)(void*, const plg::string&, double);
}
extern "C" PLUGIN_API configs::_TrySetFromFloat __configs_TrySetFromFloat;
namespace configs {
  /**
   * @brief Attempts to set a configuration value from a float, with type conversion if needed.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   * @param value (double): The float value to set.
   * @return bool: True if the value was set successfully, false otherwise.
   */
  inline bool TrySetFromFloat(void* config, const plg::string& key, double value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::TrySetFromFloat", __location);
    return __configs_TrySetFromFloat(config, key, value);
  }

  using _TrySetFromString = bool (*)(void*, const plg::string&, const plg::string&);
}
extern "C" PLUGIN_API configs::_TrySetFromString __configs_TrySetFromString;
namespace configs {
  /**
   * @brief Attempts to set a configuration value from a string, with type conversion if needed.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to set.
   * @param value (string): The string value to set.
   * @return bool: True if the value was set successfully, false otherwise.
   */
  inline bool TrySetFromString(void* config, const plg::string& key, const plg::string& value, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::TrySetFromString", __location);
    return __configs_TrySetFromString(config, key, value);
  }

} // namespace configs
