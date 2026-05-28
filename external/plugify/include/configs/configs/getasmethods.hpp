#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: getasmethods)

namespace configs {

  using _GetAsBool = bool (*)(void*, const plg::string&, bool);
}
extern "C" PLUGIN_API configs::_GetAsBool __configs_GetAsBool;
namespace configs {
  /**
   * @brief Retrieves a value from the configuration and converts it to boolean.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to retrieve.
   * @param defaultValue (bool): The default value to return if the key is not found.
   * @return bool: The boolean value if found and convertible, otherwise the default value.
   */
  inline bool GetAsBool(void* config, const plg::string& key, bool defaultValue, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetAsBool", __location);
    return __configs_GetAsBool(config, key, defaultValue);
  }

  using _GetAsInt = int64_t (*)(void*, const plg::string&, int64_t);
}
extern "C" PLUGIN_API configs::_GetAsInt __configs_GetAsInt;
namespace configs {
  /**
   * @brief Retrieves a value from the configuration and converts it to integer.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to retrieve.
   * @param defaultValue (int64): The default value to return if the key is not found.
   * @return int64: The integer value if found and convertible, otherwise the default value.
   */
  inline int64_t GetAsInt(void* config, const plg::string& key, int64_t defaultValue, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetAsInt", __location);
    return __configs_GetAsInt(config, key, defaultValue);
  }

  using _GetAsFloat = double (*)(void*, const plg::string&, double);
}
extern "C" PLUGIN_API configs::_GetAsFloat __configs_GetAsFloat;
namespace configs {
  /**
   * @brief Retrieves a value from the configuration and converts it to float.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to retrieve.
   * @param defaultValue (double): The default value to return if the key is not found.
   * @return double: The float value if found and convertible, otherwise the default value.
   */
  inline double GetAsFloat(void* config, const plg::string& key, double defaultValue, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetAsFloat", __location);
    return __configs_GetAsFloat(config, key, defaultValue);
  }

  using _GetAsString = plg::string (*)(void*, const plg::string&, const plg::string&);
}
extern "C" PLUGIN_API configs::_GetAsString __configs_GetAsString;
namespace configs {
  /**
   * @brief Retrieves a value from the configuration and converts it to string.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to retrieve.
   * @param defaultValue (string): The default value to return if the key is not found.
   * @return string: The string value if found and convertible, otherwise the default value.
   */
  inline plg::string GetAsString(void* config, const plg::string& key, const plg::string& defaultValue, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetAsString", __location);
    return __configs_GetAsString(config, key, defaultValue);
  }

} // namespace configs
