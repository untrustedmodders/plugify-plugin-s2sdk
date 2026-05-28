#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: removalmethods)

namespace configs {

  using _Remove = int32_t (*)(void*);
}
extern "C" PLUGIN_API configs::_Remove __configs_Remove;
namespace configs {
  /**
   * @brief Removes the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @return int32: The number of nodes removed.
   */
  inline int32_t Remove(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::Remove", __location);
    return __configs_Remove(config);
  }

  using _RemoveKey = bool (*)(void*, const plg::string&);
}
extern "C" PLUGIN_API configs::_RemoveKey __configs_RemoveKey;
namespace configs {
  /**
   * @brief Removes a child node with the specified key from the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key of the node to remove.
   * @return bool: True if the node was removed successfully, false otherwise.
   */
  inline bool RemoveKey(void* config, const plg::string& key, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::RemoveKey", __location);
    return __configs_RemoveKey(config, key);
  }

  using _Clear = void (*)(void*);
}
extern "C" PLUGIN_API configs::_Clear __configs_Clear;
namespace configs {
  /**
   * @brief Clears all child nodes from the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   */
  inline void Clear(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::Clear", __location);
    return __configs_Clear(config);
  }

} // namespace configs
