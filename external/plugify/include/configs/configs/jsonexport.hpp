#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: jsonexport)

namespace configs {

  using _NodeToJsonString = plg::string (*)(void*);
}
extern "C" PLUGIN_API configs::_NodeToJsonString __configs_NodeToJsonString;
namespace configs {
  /**
   * @brief Converts the current configuration node to a JSON string.
   * @param config (ptr64): Pointer to the Config object.
   * @return string: The JSON string representation of the current node.
   */
  inline plg::string NodeToJsonString(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::NodeToJsonString", __location);
    return __configs_NodeToJsonString(config);
  }

  using _RootToJsonString = plg::string (*)(void*);
}
extern "C" PLUGIN_API configs::_RootToJsonString __configs_RootToJsonString;
namespace configs {
  /**
   * @brief Converts the entire configuration tree to a JSON string.
   * @param config (ptr64): Pointer to the Config object.
   * @return string: The JSON string representation of the entire configuration tree.
   */
  inline plg::string RootToJsonString(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::RootToJsonString", __location);
    return __configs_RootToJsonString(config);
  }

} // namespace configs
