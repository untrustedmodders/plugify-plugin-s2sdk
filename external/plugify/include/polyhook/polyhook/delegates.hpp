#pragma once

#include "enums.hpp"
#include <plg/plugin.hpp>
#include <plg/any.hpp>

// Generated from polyhook.pplugin

namespace polyhook {

  // Callback function
  using CallbackHandler = ResultType (*)(void*, void*, int32_t, void*, CallbackType);

} // namespace polyhook
