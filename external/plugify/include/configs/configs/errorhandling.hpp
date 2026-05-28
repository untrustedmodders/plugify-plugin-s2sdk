#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: errorhandling)

namespace configs {

  using _SetError = void (*)(const plg::string&);
}
extern "C" PLUGIN_API configs::_SetError __configs_SetError;
namespace configs {
  /**
   * @brief Sets an error message for the configuration system.
   * @param error (string): The error message to set.
   */
  inline void SetError(const plg::string& error, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::SetError", __location);
    return __configs_SetError(error);
  }

  using _GetError = plg::string (*)();
}
extern "C" PLUGIN_API configs::_GetError __configs_GetError;
namespace configs {
  /**
   * @brief Retrieves the last error message from the configuration system.
   * @return string: The last error message.
   */
  inline plg::string GetError(plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::GetError", __location);
    return __configs_GetError();
  }

} // namespace configs
