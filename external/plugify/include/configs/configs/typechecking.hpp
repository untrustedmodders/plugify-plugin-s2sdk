#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: typechecking)

namespace configs {

  using _GetType = int32_t (*)(void*);
}
extern "C" PLUGIN_API configs::_GetType __configs_GetType;
namespace configs {
  /**
   * @brief Returns the type of the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @return int32: The NodeType enum value.
   */
  inline int32_t GetType(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetType", __location);
    return __configs_GetType(config);
  }

  using _IsNull = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_IsNull __configs_IsNull;
namespace configs {
  /**
   * @brief Checks if the current node is null.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the node is null, false otherwise.
   */
  inline bool IsNull(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::IsNull", __location);
    return __configs_IsNull(config);
  }

  using _IsBool = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_IsBool __configs_IsBool;
namespace configs {
  /**
   * @brief Checks if the current node is a boolean.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the node is a boolean, false otherwise.
   */
  inline bool IsBool(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::IsBool", __location);
    return __configs_IsBool(config);
  }

  using _IsInt = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_IsInt __configs_IsInt;
namespace configs {
  /**
   * @brief Checks if the current node is an integer.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the node is an integer, false otherwise.
   */
  inline bool IsInt(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::IsInt", __location);
    return __configs_IsInt(config);
  }

  using _IsFloat = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_IsFloat __configs_IsFloat;
namespace configs {
  /**
   * @brief Checks if the current node is a float.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the node is a float, false otherwise.
   */
  inline bool IsFloat(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::IsFloat", __location);
    return __configs_IsFloat(config);
  }

  using _IsString = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_IsString __configs_IsString;
namespace configs {
  /**
   * @brief Checks if the current node is a string.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the node is a string, false otherwise.
   */
  inline bool IsString(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::IsString", __location);
    return __configs_IsString(config);
  }

  using _IsObject = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_IsObject __configs_IsObject;
namespace configs {
  /**
   * @brief Checks if the current node is an object.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the node is an object, false otherwise.
   */
  inline bool IsObject(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::IsObject", __location);
    return __configs_IsObject(config);
  }

  using _IsArray = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_IsArray __configs_IsArray;
namespace configs {
  /**
   * @brief Checks if the current node is an array.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the node is an array, false otherwise.
   */
  inline bool IsArray(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::IsArray", __location);
    return __configs_IsArray(config);
  }

} // namespace configs
