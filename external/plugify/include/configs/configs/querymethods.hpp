#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: querymethods)

namespace configs {

  using _HasKey = bool (*)(void*, const plg::string&);
}
extern "C" PLUGIN_API configs::_HasKey __configs_HasKey;
namespace configs {
  /**
   * @brief Checks if a key exists in the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key to check.
   * @return bool: True if the key exists, false otherwise.
   */
  inline bool HasKey(void* config, const plg::string& key, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::HasKey", __location);
    return __configs_HasKey(config, key);
  }

  using _Empty = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_Empty __configs_Empty;
namespace configs {
  /**
   * @brief Checks if the current configuration node is empty.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the node is empty, false otherwise.
   */
  inline bool Empty(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::Empty", __location);
    return __configs_Empty(config);
  }

  using _GetSize = int64_t (*)(void*);
}
extern "C" PLUGIN_API configs::_GetSize __configs_GetSize;
namespace configs {
  /**
   * @brief Returns the size of the current configuration node (number of elements in array or object).
   * @param config (ptr64): Pointer to the Config object.
   * @return int64: The size of the node.
   */
  inline int64_t GetSize(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetSize", __location);
    return __configs_GetSize(config);
  }

  using _GetName = plg::string (*)(void*);
}
extern "C" PLUGIN_API configs::_GetName __configs_GetName;
namespace configs {
  /**
   * @brief Returns the name of the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @return string: The name of the node.
   */
  inline plg::string GetName(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetName", __location);
    return __configs_GetName(config);
  }

  using _GetPath = plg::string (*)(void*);
}
extern "C" PLUGIN_API configs::_GetPath __configs_GetPath;
namespace configs {
  /**
   * @brief Returns the full path to the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @return string: The path to the node.
   */
  inline plg::string GetPath(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetPath", __location);
    return __configs_GetPath(config);
  }

} // namespace configs
