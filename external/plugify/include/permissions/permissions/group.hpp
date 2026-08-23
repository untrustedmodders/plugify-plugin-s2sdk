#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from permissions.pplugin (group: group)

namespace permissions {

  using _SetParent = Status (*)(int64_t, const plg::string&, const plg::string&, bool);
}
extern "C" PLUGIN_API permissions::_SetParent __permissions_SetParent;
namespace permissions {
  /**
   * @brief Set parent group for child group
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param childName (string): Child group name
   * @param parentName (string): Parent group name to set
   * @param dontBroadcast (bool)
   * @return int32: Success, ChildGroupNotFound, ParentGroupNotFound
   */
  inline Status SetParent(int64_t pluginID, const plg::string& childName, const plg::string& parentName, bool dontBroadcast) {
    return __permissions_SetParent(pluginID, childName, parentName, dontBroadcast);
  }

  using _GetParent = Status (*)(const plg::string&, plg::string&);
}
extern "C" PLUGIN_API permissions::_GetParent __permissions_GetParent;
namespace permissions {
  /**
   * @brief Get parent of requested group
   * @param groupName (string): Group name
   * @param parentName (string&): Parent name
   * @return int32: Success, ChildGroupNotFound, ParentGroupNotFound
   */
  inline Status GetParent(const plg::string& groupName, plg::string& parentName) {
    return __permissions_GetParent(groupName, parentName);
  }

  using _DumpPermissionsGroup = Status (*)(const plg::string&, plg::vector<plg::string>&);
}
extern "C" PLUGIN_API permissions::_DumpPermissionsGroup __permissions_DumpPermissionsGroup;
namespace permissions {
  /**
   * @brief Get permissions of group
   * @param name (string): Group name
   * @param perms (string[]&): Permissions
   * @return int32: Success, GroupNotFound
   */
  inline Status DumpPermissionsGroup(const plg::string& name, plg::vector<plg::string>& perms) {
    return __permissions_DumpPermissionsGroup(name, perms);
  }

  using _GetAllGroups = plg::vector<plg::string> (*)();
}
extern "C" PLUGIN_API permissions::_GetAllGroups __permissions_GetAllGroups;
namespace permissions {
  /**
   * @brief Get all created groups
   * @return string[]: Array of groups
   */
  inline plg::vector<plg::string> GetAllGroups() {
    return __permissions_GetAllGroups();
  }

  using _HasPermissionGroupExtended = Status (*)(const plg::string&, const plg::string&, bool);
}
extern "C" PLUGIN_API permissions::_HasPermissionGroupExtended __permissions_HasPermissionGroupExtended;
namespace permissions {
  /**
   * @brief Check if a group has a specific permission.
   * @param name (string): Group name.
   * @param perm (string): Permission line.
   * @param exact (bool): Checking permission with ignoring wildcards (pass 'false' for default behavior)
   * @return int32: Allow, Disallow, PermNotFound, GroupNotFound
   */
  inline Status HasPermissionGroupExtended(const plg::string& name, const plg::string& perm, bool exact) {
    return __permissions_HasPermissionGroupExtended(name, perm, exact);
  }

  using _HasPermissionGroup = Status (*)(const plg::string&, const plg::string&);
}
extern "C" PLUGIN_API permissions::_HasPermissionGroup __permissions_HasPermissionGroup;
namespace permissions {
  /**
   * @brief Check if a group has a specific permission.
   * @param name (string): Group name.
   * @param perm (string): Permission line.
   * @return int32: Allow, Disallow, PermNotFound, GroupNotFound
   */
  inline Status HasPermissionGroup(const plg::string& name, const plg::string& perm) {
    return __permissions_HasPermissionGroup(name, perm);
  }

  using _HasParentGroup = Status (*)(const plg::string&, const plg::string&);
}
extern "C" PLUGIN_API permissions::_HasParentGroup __permissions_HasParentGroup;
namespace permissions {
  /**
   * @brief Check if parent_name is a parent group for child_name.
   * @param childName (string): Child group name.
   * @param parentName (string): Parent group name to check.
   * @return int32: Allow, Disallow, ChildGroupNotFound, ParentGroupNotFound
   */
  inline Status HasParentGroup(const plg::string& childName, const plg::string& parentName) {
    return __permissions_HasParentGroup(childName, parentName);
  }

  using _GetPriorityGroup = Status (*)(const plg::string&, int32_t&);
}
extern "C" PLUGIN_API permissions::_GetPriorityGroup __permissions_GetPriorityGroup;
namespace permissions {
  /**
   * @brief Get the priority of a group.
   * @param groupName (string): Group name.
   * @param priority (int32&): Priority
   * @return int32: Success, GroupNotFound
   */
  inline Status GetPriorityGroup(const plg::string& groupName, int32_t& priority) {
    return __permissions_GetPriorityGroup(groupName, priority);
  }

  using _AddPermissionGroup = Status (*)(int64_t, const plg::string&, const plg::string&, bool);
}
extern "C" PLUGIN_API permissions::_AddPermissionGroup __permissions_AddPermissionGroup;
namespace permissions {
  /**
   * @brief Add a permission to a group.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param name (string): Group name.
   * @param perm (string): Permission line.
   * @param dontBroadcast (bool): If set to `true`, suppresses dispatching of the permission change event to registered GroupPermission listeners. The permission is still applied internally.
   * @return int32: Success, GroupNotFound, PermAlreadyGranted
   */
  inline Status AddPermissionGroup(int64_t pluginID, const plg::string& name, const plg::string& perm, bool dontBroadcast) {
    return __permissions_AddPermissionGroup(pluginID, name, perm, dontBroadcast);
  }

  using _SetPermissionGroup = Status (*)(int64_t, const plg::string&, const plg::string&, bool);
}
extern "C" PLUGIN_API permissions::_SetPermissionGroup __permissions_SetPermissionGroup;
namespace permissions {
  /**
   * @param pluginID (int64)
   * @param name (string)
   * @param perm (string)
   * @param dontBroadcast (bool)
   * @return int32
   */
  inline Status SetPermissionGroup(int64_t pluginID, const plg::string& name, const plg::string& perm, bool dontBroadcast) {
    return __permissions_SetPermissionGroup(pluginID, name, perm, dontBroadcast);
  }

  using _RemovePermissionGroup = Status (*)(int64_t, const plg::string&, const plg::string&, bool, bool);
}
extern "C" PLUGIN_API permissions::_RemovePermissionGroup __permissions_RemovePermissionGroup;
namespace permissions {
  /**
   * @brief Remove a permission from a group.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param name (string): Group name.
   * @param perm (string): Permission line.
   * @param recursiveDeletion (bool): Delete all nested perms.
   * @param dontBroadcast (bool)
   * @return int32: Success, GroupNotFound, PermNotFound
   */
  inline Status RemovePermissionGroup(int64_t pluginID, const plg::string& name, const plg::string& perm, bool recursiveDeletion, bool dontBroadcast) {
    return __permissions_RemovePermissionGroup(pluginID, name, perm, recursiveDeletion, dontBroadcast);
  }

  using _GetOptionGroup = Status (*)(const plg::string&, const plg::string&, plg::any&);
}
extern "C" PLUGIN_API permissions::_GetOptionGroup __permissions_GetOptionGroup;
namespace permissions {
  /**
   * @brief Get an option value for a group.
   * @param groupName (string): Group name
   * @param optionName (string): Option name
   * @param value (any&): Option value
   * @return int32: Success, OptionNotFound, GroupNotFound
   */
  inline Status GetOptionGroup(const plg::string& groupName, const plg::string& optionName, plg::any& value) {
    return __permissions_GetOptionGroup(groupName, optionName, value);
  }

  using _SetOptionGroup = Status (*)(int64_t, const plg::string&, const plg::string&, const plg::any&, bool);
}
extern "C" PLUGIN_API permissions::_SetOptionGroup __permissions_SetOptionGroup;
namespace permissions {
  /**
   * @brief Set an option value for a group.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param groupName (string): Group name
   * @param optionName (string): Option name
   * @param value (any): Option value.
   * @param dontBroadcast (bool)
   * @return int32: Success, GroupNotFound
   */
  inline Status SetOptionGroup(int64_t pluginID, const plg::string& groupName, const plg::string& optionName, const plg::any& value, bool dontBroadcast) {
    return __permissions_SetOptionGroup(pluginID, groupName, optionName, value, dontBroadcast);
  }

  using _GetAllOptionsGroup = Status (*)(const plg::string&, plg::vector<plg::string>&, plg::vector<plg::any>&);
}
extern "C" PLUGIN_API permissions::_GetAllOptionsGroup __permissions_GetAllOptionsGroup;
namespace permissions {
  /**
   * @brief Get all options from group.
   * @param groupName (string): Group name
   * @param optionNames (string[]&): Array of option names
   * @param values (any[]&): Array of option values
   * @return int32: Success, GroupNotFound
   */
  inline Status GetAllOptionsGroup(const plg::string& groupName, plg::vector<plg::string>& optionNames, plg::vector<plg::any>& values) {
    return __permissions_GetAllOptionsGroup(groupName, optionNames, values);
  }

  using _CreateGroup = Status (*)(int64_t, const plg::string&, const plg::vector<plg::string>&, int32_t, const plg::string&, bool);
}
extern "C" PLUGIN_API permissions::_CreateGroup __permissions_CreateGroup;
namespace permissions {
  /**
   * @brief Create a new group.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param name (string): Group name.
   * @param perms (string[]): Array of permission lines.
   * @param priority (int32): Group priority.
   * @param parent (string): Parent group name.
   * @param dontBroadcast (bool)
   * @return int32: Success, GroupAlreadyExist, ParentGroupNotFound
   */
  inline Status CreateGroup(int64_t pluginID, const plg::string& name, const plg::vector<plg::string>& perms, int32_t priority, const plg::string& parent, bool dontBroadcast) {
    return __permissions_CreateGroup(pluginID, name, perms, priority, parent, dontBroadcast);
  }

  using _DeleteGroup = Status (*)(int64_t, const plg::string&, bool);
}
extern "C" PLUGIN_API permissions::_DeleteGroup __permissions_DeleteGroup;
namespace permissions {
  /**
   * @brief Delete a group.
   * @param pluginID (int64): Identifier of the plugin that calls the method.
   * @param name (string): Group name.
   * @param dontBroadcast (bool)
   * @return int32: Success if deleted; GroupNotFound if group not found.
   */
  inline Status DeleteGroup(int64_t pluginID, const plg::string& name, bool dontBroadcast) {
    return __permissions_DeleteGroup(pluginID, name, dontBroadcast);
  }

  using _GroupExists = bool (*)(const plg::string&);
}
extern "C" PLUGIN_API permissions::_GroupExists __permissions_GroupExists;
namespace permissions {
  /**
   * @brief Check if a group exists.
   * @param name (string): Group name.
   * @return bool: True if group exists, false otherwise.
   */
  inline bool GroupExists(const plg::string& name) {
    return __permissions_GroupExists(name);
  }

  using _LoadGroups = Status (*)(int64_t, bool);
}
extern "C" PLUGIN_API permissions::_LoadGroups __permissions_LoadGroups;
namespace permissions {
  /**
   * @brief Dispatches a request to load server groups for a plugin.
   * @param pluginID (int64): Identifier of the calling plugin.
   * @param dontBroadcast (bool)
   * @return int32
   */
  inline Status LoadGroups(int64_t pluginID, bool dontBroadcast) {
    return __permissions_LoadGroups(pluginID, dontBroadcast);
  }

} // namespace permissions
