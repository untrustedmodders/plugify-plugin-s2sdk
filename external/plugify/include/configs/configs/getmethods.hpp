#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: getmethods)

namespace configs {

  using _GetBool = bool (*)(void*, const plg::string&, bool);
}
extern "C" PLUGIN_API configs::_GetBool __configs_GetBool;
namespace configs {
  /**
   * @brief Retrieves a boolean value from the configuration.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to retrieve.
   * @param defaultValue (bool): The default value to return if the key is not found.
   * @return bool: The boolean value if found, otherwise the default value.
   */
  inline bool GetBool(void* config, const plg::string& key, bool defaultValue, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetBool", __location);
    return __configs_GetBool(config, key, defaultValue);
  }

  using _GetInt = int64_t (*)(void*, const plg::string&, int64_t);
}
extern "C" PLUGIN_API configs::_GetInt __configs_GetInt;
namespace configs {
  /**
   * @brief Retrieves an integer value from the configuration.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to retrieve.
   * @param defaultValue (int64): The default value to return if the key is not found.
   * @return int64: The integer value if found, otherwise the default value.
   */
  inline int64_t GetInt(void* config, const plg::string& key, int64_t defaultValue, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetInt", __location);
    return __configs_GetInt(config, key, defaultValue);
  }

  using _GetFloat = double (*)(void*, const plg::string&, double);
}
extern "C" PLUGIN_API configs::_GetFloat __configs_GetFloat;
namespace configs {
  /**
   * @brief Retrieves a float value from the configuration.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to retrieve.
   * @param defaultValue (double): The default value to return if the key is not found.
   * @return double: The float value if found, otherwise the default value.
   */
  inline double GetFloat(void* config, const plg::string& key, double defaultValue, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetFloat", __location);
    return __configs_GetFloat(config, key, defaultValue);
  }

  using _GetString = plg::string (*)(void*, const plg::string&, const plg::string&);
}
extern "C" PLUGIN_API configs::_GetString __configs_GetString;
namespace configs {
  /**
   * @brief Retrieves a string value from the configuration.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to retrieve.
   * @param defaultValue (string): The default value to return if the key is not found.
   * @return string: The string value if found, otherwise the default value.
   */
  inline plg::string GetString(void* config, const plg::string& key, const plg::string& defaultValue, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetString", __location);
    return __configs_GetString(config, key, defaultValue);
  }

} // namespace configs
