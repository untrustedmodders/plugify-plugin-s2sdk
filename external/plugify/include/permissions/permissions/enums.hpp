#pragma once

#include <cstdint>
// Generated from permissions.pplugin

namespace permissions {

  enum class Status : int32_t {
    Success = 0,
    Allow = 1,
    Disallow = 2,
    PermNotFound = 3,
    TemporalGroup = 4,
    PermanentGroup = 5,
    GroupNotDefined = 6,
    ErrorsStart = 30,
    PermAlreadyGranted = 31,
    CookieNotFound = 32,
    OptionNotFound = 32,
    GroupNotFound = 33,
    ChildGroupNotFound = 34,
    ParentGroupNotFound = 35,
    GroupAlreadyExist = 36,
    GroupHierarchyCycle = 37,
    ActorUserNotFound = 38,
    TargetUserNotFound = 39,
    UserAlreadyExist = 40,
    CallbackInvalid = 41,
    CallbackAlreadyExist = 42,
    CallbackNotFound = 43,
    StorageError = 44,
    DBNotReady = 45,
    InvalidPermission = 46,
    InvalidGroupName = 47,
    InvalidCookieName = 48,
    InvalidOptionName = 48,
    PermConflict = 49
  };

  enum class Action : int32_t {
    Add = 0,
    Remove = 1,
    Replace = 2,
    ReplaceToWC = 3
  };

  enum class PlayerState : uint32_t {
    NotFound = 0,
    Online = 1,
    Offline = 2
  };

  enum class PermSource : uint32_t {
    UserTemp = 0,
    User = 1,
    GroupTemp = 2,
    Group = 3,
    NotFound = 4
  };


  /// Ownership type for RAII wrappers
  enum class Ownership : bool { Borrowed, Owned };

} // namespace permissions
