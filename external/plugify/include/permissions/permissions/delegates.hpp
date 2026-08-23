#pragma once

#include "enums.hpp"
#include "aliases.hpp"
// Generated from permissions.pplugin

namespace permissions {

  // Callback invoked after a group is successfully created.
  using GroupCreateStorageCallback = bool (*)(int64_t, const plg::string&, const plg::vector<plg::string>&, int32_t, const plg::string&);


  // Callback invoked after a group is successfully created.
  using GroupCreateCallback = void (*)(int64_t, const plg::string&, const plg::vector<plg::string>&, int32_t, const plg::string&);


  // Callback invoked before a group is deleted.
  using GroupDeleteStorageCallback = bool (*)(int64_t, const plg::string&);


  // Callback invoked before a group is deleted.
  using GroupDeleteCallback = void (*)(int64_t, const plg::string&);


  // Callback invoked when a group in user has been expired.
  using GroupExpirationCallback = void (*)(uint64_t, const plg::string&);


  // Callback invoked when an option value is set for a group.
  using GroupOptionStorageCallback = bool (*)(int64_t, const plg::string&, const plg::string&, const plg::any&);


  // Callback invoked when an option value is set for a group.
  using GroupOptionCallback = void (*)(int64_t, const plg::string&, const plg::string&, const plg::any&);


  // Callback invoked when a permission is added or removed from a group.
  using GroupPermissionStorageCallback = bool (*)(int64_t, Action, const plg::string&, const plg::string&, Status, Status);


  // Callback invoked when a permission is added or removed from a group.
  using GroupPermissionCallback = void (*)(int64_t, Action, const plg::string&, const plg::string&, Status, Status);


  // Called when the core requests loading of server groups.
  using LoadGroupsCallback = bool (*)(int64_t);


  // Callback invoked when a permission in user has been expired.
  using PermExpirationCallback = void (*)(uint64_t, const plg::string&, Status);


  // Callback invoked when a parent group is set for a child group.
  using SetParentStorageCallback = bool (*)(int64_t, const plg::string&, const plg::string&);


  // Callback invoked when a parent group is set for a child group.
  using SetParentCallback = void (*)(int64_t, const plg::string&, const plg::string&);


  // Callback invoked when a cookie is set for a user.
  using UserCookieStorageCallback = bool (*)(int64_t, uint64_t, const plg::string&, const plg::any&);


  // Callback invoked when a cookie is set for a user.
  using UserCookieCallback = void (*)(int64_t, uint64_t, const plg::string&, const plg::any&);


  // Callback invoked after a user is successfully created.
  using UserCreateStorageCallback = bool (*)(int64_t, uint64_t, int32_t, bool, const plg::vector<plg::string>&);


  // Callback invoked after a user is successfully created.
  using UserCreateCallback = void (*)(int64_t, uint64_t, int32_t, bool, const plg::vector<plg::string>&);


  // Callback invoked before a user is deleted.
  using UserDeleteStorageCallback = bool (*)(int64_t, uint64_t);


  // Callback invoked before a user is deleted.
  using UserDeleteCallback = void (*)(int64_t, uint64_t);


  // Callback invoked when a group is added or removed from a user.
  using UserGroupStorageCallback = bool (*)(int64_t, Action, uint64_t, const plg::string&, int64_t, int64_t);


  // Callback invoked when a group is added or removed from a user.
  using UserGroupCallback = void (*)(int64_t, Action, uint64_t, const plg::string&, int64_t, int64_t);


  // Callback invoked when immunity is set for a user.
  using UserImmunityStorageCallback = bool (*)(int64_t, uint64_t, int32_t);


  // Callback invoked when immunity is set for a user.
  using UserImmunityCallback = void (*)(int64_t, uint64_t, int32_t);


  // Called when a user's data has been fully loaded.
  using UserLoadedCallback = void (*)(int64_t, uint64_t, PlayerState);


  // Callback invoked when a permission is added, removed, or replaced for a user.
  using UserPermissionStorageCallback = bool (*)(int64_t, Action, uint64_t, const plg::string&, Status, Status, int64_t, int64_t);


  // Callback invoked when a permission is added, removed, or replaced for a user.
  using UserPermissionCallback = void (*)(int64_t, Action, uint64_t, const plg::string&, Status, Status, int64_t, int64_t);


  // Called when a user data load is requested.
  using UserRequestCallback = bool (*)(int64_t, uint64_t, const plg::string&, bool);


} // namespace permissions
