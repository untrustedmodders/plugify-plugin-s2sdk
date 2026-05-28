#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: navigationmethods)

namespace configs {

  using _JumpFirst = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_JumpFirst __configs_JumpFirst;
namespace configs {
  /**
   * @brief Jumps to the first child node in the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the jump was successful, false otherwise.
   */
  inline bool JumpFirst(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::JumpFirst", __location);
    return __configs_JumpFirst(config);
  }

  using _JumpLast = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_JumpLast __configs_JumpLast;
namespace configs {
  /**
   * @brief Jumps to the last child node in the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the jump was successful, false otherwise.
   */
  inline bool JumpLast(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::JumpLast", __location);
    return __configs_JumpLast(config);
  }

  using _JumpNext = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_JumpNext __configs_JumpNext;
namespace configs {
  /**
   * @brief Jumps to the next sibling node in the current configuration level.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the jump was successful, false otherwise.
   */
  inline bool JumpNext(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::JumpNext", __location);
    return __configs_JumpNext(config);
  }

  using _JumpPrev = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_JumpPrev __configs_JumpPrev;
namespace configs {
  /**
   * @brief Jumps to the previous sibling node in the current configuration level.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the jump was successful, false otherwise.
   */
  inline bool JumpPrev(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::JumpPrev", __location);
    return __configs_JumpPrev(config);
  }

  using _JumpKey = bool (*)(void*, const plg::string&, bool);
}
extern "C" PLUGIN_API configs::_JumpKey __configs_JumpKey;
namespace configs {
  /**
   * @brief Jumps to a child node with the specified key, optionally creating it if it doesn't exist.
   * @param config (ptr64): Pointer to the Config object.
   * @param key (string): The key of the node to jump to.
   * @param create (bool): If true, creates the node if it doesn't exist.
   * @return bool: True if the jump was successful, false otherwise.
   */
  inline bool JumpKey(void* config, const plg::string& key, bool create = false, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::JumpKey", __location);
    return __configs_JumpKey(config, key, create);
  }

  using _JumpN = bool (*)(void*, int32_t);
}
extern "C" PLUGIN_API configs::_JumpN __configs_JumpN;
namespace configs {
  /**
   * @brief Jumps to the nth child node in the current configuration node.
   * @param config (ptr64): Pointer to the Config object.
   * @param n (int32): The index of the child node to jump to.
   * @return bool: True if the jump was successful, false otherwise.
   */
  inline bool JumpN(void* config, int32_t n, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::JumpN", __location);
    return __configs_JumpN(config, n);
  }

  using _JumpBack = bool (*)(void*);
}
extern "C" PLUGIN_API configs::_JumpBack __configs_JumpBack;
namespace configs {
  /**
   * @brief Jumps back to the parent node in the configuration hierarchy.
   * @param config (ptr64): Pointer to the Config object.
   * @return bool: True if the jump was successful, false otherwise.
   */
  inline bool JumpBack(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::JumpBack", __location);
    return __configs_JumpBack(config);
  }

  using _JumpRoot = void (*)(void*);
}
extern "C" PLUGIN_API configs::_JumpRoot __configs_JumpRoot;
namespace configs {
  /**
   * @brief Jumps back to the root node of the configuration.
   * @param config (ptr64): Pointer to the Config object.
   */
  inline void JumpRoot(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::JumpRoot", __location);
    return __configs_JumpRoot(config);
  }

} // namespace configs
