#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from permissions.pplugin (group: user)

namespace permissions {

  using _DumpPermissions = Status (*)(uint64_t, plg::vector<plg::string>&);
}
extern "C" PLUGIN_API permissions::_DumpPermissions __permissions_DumpPermissions;
namespace permissions {
  /**
   * @brief Get permissions of user
   * @param targetID (uint64): Player ID
   * @param perms (string[]&): Permissions
   * @return int32: Success, TargetUserNotFound
   */
  inline Status DumpPermissions(uint64_t targetID, plg::vector<plg::string>& perms) {
    return __permissions_DumpPermissions(targetID, perms);
  }

  using _CanAffectUser = Status (*)(uint64_t, uint64_t);
}
extern "C" PLUGIN_API permissions::_CanAffectUser __permissions_CanAffectUser;
namespace permissions {
  /**
   * @brief Check players immunity or groups priority
   * @param actorID (uint64): Player performing the action
   * @param targetID (uint64): Player receiving the action
   * @return int32: Allow, Disallow, ActorUserNotFound, or TargetUserNotFound
   */
  inline Status CanAffectUser(uint64_t actorID, uint64_t targetID) {
    return __permissions_CanAffectUser(actorID, targetID);
  }

  using _HasPermissionExtended = Status (*)(uint64_t, const plg::string&, bool, PermSource&, int64_t&);
}
extern "C" PLUGIN_API permissions::_HasPermissionExtended __permissions_HasPermissionExtended;
namespace permissions {
  /**
   * @brief Check if a user has a specific permission.
   * @param targetID (uint64): Player ID.
   * @param perm (string): Permission line.
   * @param exact (bool): Checking permission with ignoring wildcards (pass 'false' for default behavior).
   * @param permSource (uint32&): Permission source.
   * @param timestamp (int64&): Permission timestamp.
   * @return int32: Allow, Disallow, PermNotFound, TargetUserNotFound
   */
  inline Status HasPermissionExtended(uint64_t targetID, const plg::string& perm, bool exact, PermSource& permSource, int64_t& timestamp) {
    return __permissions_HasPermissionExtended(targetID, perm, exact, permSource, timestamp);
  }

  using _HasPermission = Status (*)(uint64_t, const plg::string&);
}
extern "C" PLUGIN_API permissions::_HasPermission __permissions_HasPermission;
namespace permissions {
  /**
   * @brief Check if a user has a specific permission.
   * @param targetID (uint64): Player ID.
   * @param perm (string): Permission line.
   * @return int32: Allow, Disallow, PermNotFound, TargetUserNotFound
   */
  inline Status HasPermission(uint64_t targetID, const plg::string& perm) {
    return __permissions_HasPermission(targetID, perm);
  }

  using _HasGroupExtended = Status (*)(uint64_t, const plg::string&, int64_t&);
}
extern "C" PLUGIN_API permissions::_HasGroupExtended __permissions_HasGroupExtended;
namespace permissions {
  /**
   * @brief Check if a user belongs to a specific group (directly or via parent groups).
   * @param targetID (uint64): Player ID.
   * @param groupName (string): Group name.
   * @param timestamp (int64&): Group timestamp.
   * @return int32: PermanentGroup, TemporalGroup, GroupNotDefined, TargetUserNotFound, GroupNotFound
   */
  inline Status HasGroupExtended(uint64_t targetID, const plg::string& groupName, int64_t& timestamp) {
    return __permissions_HasGroupExtended(targetID, groupName, timestamp);
  }

  using _HasGroup = Status (*)(uint64_t, const plg::string&);
}
extern "C" PLUGIN_API permissions::_HasGroup __permissions_HasGroup;
namespace permissions {
  /**
   * @brief Check if a user belongs to a specific group (directly or via parent groups).
   * @param targetID (uint64): Player ID.
   * @param groupName (string): Group name.
   * @return int32: PermanentGroup, TemporalGroup, GroupNotDefined, TargetUserNotFound, GroupNotFound
   */
  inline Status HasGroup(uint64_t targetID, const plg::string& groupName) {
    return __permissions_HasGroup(targetID, groupName);
  }

  using _GetUserGroups = Status (*)(uint64_t, plg::vector<plg::string>&);
}
extern "C" PLUGIN_API permissions::_GetUserGroups __permissions_GetUserGroups;
namespace permissions {
  /**
   * @brief Get user groups.
   * @param targetID (uint64): Player ID.
   * @param outGroups (string[]&): Groups
   * @return int32: Success, TargetUserNotFound
   */
  inline Status GetUserGroups(uint64_t targetID, plg::vector<plg::string>& outGroups) {
    return __permissions_GetUserGroups(targetID, outGroups);
  }

  using _GetImmunity = Status (*)(uint64_t, int32_t&);
}
extern "C" PLUGIN_API permissions::_GetImmunity __permissions_GetImmunity;
namespace permissions {
  /**
   * @brief Get the immunity level of a user.
   * @param targetID (uint64): Player ID.
   * @param immunity (int32&): Immunity
   * @return int32: Success, TargetUserNotFound
   */
  inline Status GetImmunity(uint64_t targetID, int32_t& immunity) {
    return __permissions_GetImmunity(targetID, immunity);
  }

  using _SetImmunity = Status (*)(int64_t, uint64_t, int32_t, bool);
}
extern "C" PLUGIN_API permissions::_SetImmunity __permissions_SetImmunity;
namespace permissions {
  /**
   * @brief Set the immunity level of a user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @param immunity (int32): Immunity.
   * @param dontBroadcast (bool)
   * @return int32: Success, TargetUserNotFound
   */
  inline Status SetImmunity(int64_t pluginID, uint64_t targetID, int32_t immunity, bool dontBroadcast) {
    return __permissions_SetImmunity(pluginID, targetID, immunity, dontBroadcast);
  }

  using _AddPermission = Status (*)(int64_t, uint64_t, const plg::string&, int64_t, bool);
}
extern "C" PLUGIN_API permissions::_AddPermission __permissions_AddPermission;
namespace permissions {
  /**
   * @brief Add a permission to a user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @param perm (string): Permission line.
   * @param timestamp (int64): Permission duration
   * @param dontBroadcast (bool): If set to `true`, suppresses dispatching of the permission change event to registered UserPermission listeners. The permission is still applied internally.
   * @return int32: Success, TargetUserNotFound, PermAlreadyGranted
   */
  inline Status AddPermission(int64_t pluginID, uint64_t targetID, const plg::string& perm, int64_t timestamp, bool dontBroadcast) {
    return __permissions_AddPermission(pluginID, targetID, perm, timestamp, dontBroadcast);
  }

  using _SetPermission = Status (*)(int64_t, uint64_t, const plg::string&, int64_t, bool);
}
extern "C" PLUGIN_API permissions::_SetPermission __permissions_SetPermission;
namespace permissions {
  /**
   * @brief Set a permission to a user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @param perm (string): Permission line.
   * @param timestamp (int64): Permission duration
   * @param dontBroadcast (bool): If set to `true`, suppresses dispatching of the permission change event to registered UserPermission listeners. The permission is still applied internally.
   * @return int32: Success, TargetUserNotFound, PermAlreadyGranted
   */
  inline Status SetPermission(int64_t pluginID, uint64_t targetID, const plg::string& perm, int64_t timestamp, bool dontBroadcast) {
    return __permissions_SetPermission(pluginID, targetID, perm, timestamp, dontBroadcast);
  }

  using _RemovePermission = Status (*)(int64_t, uint64_t, const plg::string&, bool, bool);
}
extern "C" PLUGIN_API permissions::_RemovePermission __permissions_RemovePermission;
namespace permissions {
  /**
   * @brief Remove a permission from a user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @param perm (string): Permission line.
   * @param recursiveDeletion (bool): Delete all nested perms.
   * @param dontBroadcast (bool)
   * @return int32: Success, TargetUserNotFound, PermNotFound
   */
  inline Status RemovePermission(int64_t pluginID, uint64_t targetID, const plg::string& perm, bool recursiveDeletion, bool dontBroadcast) {
    return __permissions_RemovePermission(pluginID, targetID, perm, recursiveDeletion, dontBroadcast);
  }

  using _AddGroup = Status (*)(int64_t, uint64_t, const plg::string&, int64_t, bool);
}
extern "C" PLUGIN_API permissions::_AddGroup __permissions_AddGroup;
namespace permissions {
  /**
   * @brief Add a group to a user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @param groupName (string): Group name.
   * @param timestamp (int64): Group duration.
   * @param dontBroadcast (bool): If set to `true`, suppresses dispatching of the group change event to registered UserGroup listeners. The group is still applied internally.
   * @return int32: Success, TargetUserNotFound, GroupNotFound, GroupAlreadyExist
   */
  inline Status AddGroup(int64_t pluginID, uint64_t targetID, const plg::string& groupName, int64_t timestamp, bool dontBroadcast) {
    return __permissions_AddGroup(pluginID, targetID, groupName, timestamp, dontBroadcast);
  }

  using _RemoveGroup = Status (*)(int64_t, uint64_t, const plg::string&, bool);
}
extern "C" PLUGIN_API permissions::_RemoveGroup __permissions_RemoveGroup;
namespace permissions {
  /**
   * @brief Remove a group from a user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @param groupName (string): Group name.
   * @param dontBroadcast (bool)
   * @return int32: Success, TargetUserNotFound, ChildGroupNotFound, ParentGroupNotFound
   */
  inline Status RemoveGroup(int64_t pluginID, uint64_t targetID, const plg::string& groupName, bool dontBroadcast) {
    return __permissions_RemoveGroup(pluginID, targetID, groupName, dontBroadcast);
  }

  using _GetCookie = Status (*)(uint64_t, const plg::string&, plg::any&);
}
extern "C" PLUGIN_API permissions::_GetCookie __permissions_GetCookie;
namespace permissions {
  /**
   * @brief Get a cookie value for a user.
   * @param targetID (uint64): Player ID.
   * @param name (string): Cookie name.
   * @param value (any&): Cookie value.
   * @return int32: Success, TargetUserNotFound, CookieNotFound
   */
  inline Status GetCookie(uint64_t targetID, const plg::string& name, plg::any& value) {
    return __permissions_GetCookie(targetID, name, value);
  }

  using _SetCookie = Status (*)(int64_t, uint64_t, const plg::string&, const plg::any&, bool);
}
extern "C" PLUGIN_API permissions::_SetCookie __permissions_SetCookie;
namespace permissions {
  /**
   * @brief Set a cookie value for a user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @param name (string): Cookie name.
   * @param cookie (any): Cookie value.
   * @param dontBroadcast (bool): If set to `true`, suppresses dispatching of the cookie change event to registered UserSetCookie listeners. The cookie is still applied internally.
   * @return int32: Success, TargetUserNotFound
   */
  inline Status SetCookie(int64_t pluginID, uint64_t targetID, const plg::string& name, const plg::any& cookie, bool dontBroadcast) {
    return __permissions_SetCookie(pluginID, targetID, name, cookie, dontBroadcast);
  }

  using _GetAllCookies = Status (*)(uint64_t, plg::vector<plg::string>&, plg::vector<plg::any>&);
}
extern "C" PLUGIN_API permissions::_GetAllCookies __permissions_GetAllCookies;
namespace permissions {
  /**
   * @brief Get all cookies from user.
   * @param targetID (uint64): Player ID.
   * @param names (string[]&): Array of cookie names
   * @param values (any[]&): Array of cookie values
   * @return int32: Success, TargetUserNotFound
   */
  inline Status GetAllCookies(uint64_t targetID, plg::vector<plg::string>& names, plg::vector<plg::any>& values) {
    return __permissions_GetAllCookies(targetID, names, values);
  }

  using _CreateUser = Status (*)(int64_t, uint64_t, int32_t, bool, const plg::vector<plg::string>&);
}
extern "C" PLUGIN_API permissions::_CreateUser __permissions_CreateUser;
namespace permissions {
  /**
   * @brief Create a new user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @param immunity (int32): User immunity (set -1 to return highest group priority).
   * @param offline (bool): Create as fake player.
   * @param groupsList (string[]): Array of groups to inherit ("group timestamp").
   * @return int32: Success, UserAlreadyExist, GroupNotFound, ChildGroupNotFound
   */
  inline Status CreateUser(int64_t pluginID, uint64_t targetID, int32_t immunity, bool offline, const plg::vector<plg::string>& groupsList) {
    return __permissions_CreateUser(pluginID, targetID, immunity, offline, groupsList);
  }

  using _DeleteUser = Status (*)(int64_t, uint64_t);
}
extern "C" PLUGIN_API permissions::_DeleteUser __permissions_DeleteUser;
namespace permissions {
  /**
   * @brief Delete a user.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param targetID (uint64): Player ID.
   * @return int32: Success, TargetUserNotFound
   */
  inline Status DeleteUser(int64_t pluginID, uint64_t targetID) {
    return __permissions_DeleteUser(pluginID, targetID);
  }

  using _UserExists = PlayerState (*)(uint64_t);
}
extern "C" PLUGIN_API permissions::_UserExists __permissions_UserExists;
namespace permissions {
  /**
   * @brief Check if a user exists.
   * @param targetID (uint64): Player ID.
   * @return uint32: PlayerState::NotFound, PlayerState::Online, PlayerState::Offline
   */
  inline PlayerState UserExists(uint64_t targetID) {
    return __permissions_UserExists(targetID);
  }

  using _DumpUsersList = plg::vector<uint64_t> (*)();
}
extern "C" PLUGIN_API permissions::_DumpUsersList __permissions_DumpUsersList;
namespace permissions {
  /**
   * @brief Returns a list of IDs for all players registered in the core.
   * @return uint64[]: A vector containing all registered player IDs.
   */
  inline plg::vector<uint64_t> DumpUsersList() {
    return __permissions_DumpUsersList();
  }

  using _LoadUser = Status (*)(int64_t, uint64_t, const plg::string&, bool, bool);
}
extern "C" PLUGIN_API permissions::_LoadUser __permissions_LoadUser;
namespace permissions {
  /**
   * @brief Dispatches a request to load user data.
   * @param pluginID (int64): Identifier of the calling plugin.
   * @param targetID (uint64): PlayerID of the user to be loaded.
   * @param username (string): The user's current username. Intended for synchronizing the username with external storage (e.g. updating an existing record or setting it during initial user creation).
   * @param offline (bool): Indicates whether the user's data was loaded without user presence on server.
   * @param dontBroadcast (bool)
   * @return int32
   */
  inline Status LoadUser(int64_t pluginID, uint64_t targetID, const plg::string& username, bool offline, bool dontBroadcast) {
    return __permissions_LoadUser(pluginID, targetID, username, offline, dontBroadcast);
  }

} // namespace permissions
