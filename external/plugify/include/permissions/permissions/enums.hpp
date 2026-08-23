#pragma once

#include <cstdint>
// Generated from permissions.pplugin

namespace permissions {

  enum class Status : int32_t {
    Success = 0,
    Allow = 1,
    Disallow = 2,
    PermNotFound = 3,
    PermAlreadyGranted = 4,
    CookieNotFound = 5,
    OptionNotFound = 5,
    GroupNotFound = 6,
    ChildGroupNotFound = 7,
    ParentGroupNotFound = 8,
    ActorUserNotFound = 9,
    TargetUserNotFound = 10,
    GroupAlreadyExist = 11,
    UserAlreadyExist = 12,
    TemporalGroup = 13,
    PermanentGroup = 14,
    GroupNotDefined = 15,
    CallbackInvalid = 16,
    CallbackAlreadyExist = 17,
    CallbackNotFound = 18,
    StorageError = 20,
    DBNotReady = 21
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
