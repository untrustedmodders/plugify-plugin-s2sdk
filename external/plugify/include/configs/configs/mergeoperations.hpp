#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: mergeoperations)

namespace configs {

  using _Merge = void (*)(void*, void*);
}
extern "C" PLUGIN_API configs::_Merge __configs_Merge;
namespace configs {
  /**
   * @brief Merges another configuration object into the current one.
   * @param config (ptr64): Pointer to the target Config object.
   * @param other (ptr64): Pointer to the Config object to merge from.
   */
  inline void Merge(void* config, void* other, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::Merge", __location);
    return __configs_Merge(config, other);
  }

  using _MergeMove = void (*)(void*, void*);
}
extern "C" PLUGIN_API configs::_MergeMove __configs_MergeMove;
namespace configs {
  /**
   * @brief Merges another configuration object into the current one with move semantics.
   * @param config (ptr64): Pointer to the target Config object.
   * @param other (ptr64): Pointer to the Config object to merge from (will be moved).
   */
  inline void MergeMove(void* config, void* other, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::MergeMove", __location);
    return __configs_MergeMove(config, other);
  }

} // namespace configs
