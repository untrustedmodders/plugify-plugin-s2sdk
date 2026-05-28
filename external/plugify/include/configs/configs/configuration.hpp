#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from configs.pplugin (group: configuration)

namespace configs {

  using _Read = void* (*)(const plg::string&);
}
extern "C" PLUGIN_API configs::_Read __configs_Read;
namespace configs {
  /**
   * @brief Reads a configuration file from the specified path.
   * @param path (string): Path to the configuration file.
   * @return ptr64: Pointer to the Config object, or nullptr if reading failed.
   */
  inline void* Read(const plg::string& path, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::Read", __location);
    return __configs_Read(path);
  }

  using _ReadMultiple = void* (*)(const plg::vector<plg::string>&);
}
extern "C" PLUGIN_API configs::_ReadMultiple __configs_ReadMultiple;
namespace configs {
  /**
   * @brief Reads multiple configuration files and merges them.
   * @param paths (string[]): Vector of paths to configuration files.
   * @return ptr64: Pointer to the merged Config object, or nullptr if reading failed.
   */
  inline void* ReadMultiple(const plg::vector<plg::string>& paths, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::ReadMultiple", __location);
    return __configs_ReadMultiple(paths);
  }

  using _Make = void* (*)();
}
extern "C" PLUGIN_API configs::_Make __configs_Make;
namespace configs {
  /**
   * @brief Creates a new empty configuration object.
   * @return ptr64: Pointer to the newly created Config object.
   */
  inline void* Make(plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::Make", __location);
    return __configs_Make();
  }

  using _Delete = void (*)(void*);
}
extern "C" PLUGIN_API configs::_Delete __configs_Delete;
namespace configs {
  /**
   * @brief Deletes a configuration object and frees its memory.
   * @param config (ptr64): Pointer to the Config object to delete.
   */
  inline void Delete(void* config, plg::source_location __location = plg::source_location::current()) {
    [[maybe_unused]] auto __scope = plg::Scope("configs::Delete", __location);
    return __configs_Delete(config);
  }

} // namespace configs
