#pragma once

#include "enums.hpp"
#include "aliases.hpp"
#include "delegates.hpp"
#include <plugin_export.h>

// Generated from permissions.pplugin (group: listeners)

namespace permissions {

  using _OnGroupCreateStorage_Register = Status (*)(GroupCreateStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupCreateStorage_Register __permissions_OnGroupCreateStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupCreateStorage event. Callback invoked after a group is successfully created.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupCreateStorage_Register(GroupCreateStorageCallback callback) {
    return __permissions_OnGroupCreateStorage_Register(callback);
  }

  using _OnGroupCreateStorage_Unregister = Status (*)(GroupCreateStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupCreateStorage_Unregister __permissions_OnGroupCreateStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupCreateStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupCreateStorage_Unregister(GroupCreateStorageCallback callback) {
    return __permissions_OnGroupCreateStorage_Unregister(callback);
  }

  using _OnGroupCreate_Register = Status (*)(GroupCreateCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupCreate_Register __permissions_OnGroupCreate_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupCreate event. Callback invoked after a group is successfully created.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupCreate_Register(GroupCreateCallback callback) {
    return __permissions_OnGroupCreate_Register(callback);
  }

  using _OnGroupCreate_Unregister = Status (*)(GroupCreateCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupCreate_Unregister __permissions_OnGroupCreate_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupCreate event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupCreate_Unregister(GroupCreateCallback callback) {
    return __permissions_OnGroupCreate_Unregister(callback);
  }

  using _OnGroupDeleteStorage_Register = Status (*)(GroupDeleteStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupDeleteStorage_Register __permissions_OnGroupDeleteStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupDeleteStorage event. Callback invoked before a group is deleted.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupDeleteStorage_Register(GroupDeleteStorageCallback callback) {
    return __permissions_OnGroupDeleteStorage_Register(callback);
  }

  using _OnGroupDeleteStorage_Unregister = Status (*)(GroupDeleteStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupDeleteStorage_Unregister __permissions_OnGroupDeleteStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupDeleteStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupDeleteStorage_Unregister(GroupDeleteStorageCallback callback) {
    return __permissions_OnGroupDeleteStorage_Unregister(callback);
  }

  using _OnGroupDelete_Register = Status (*)(GroupDeleteCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupDelete_Register __permissions_OnGroupDelete_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupDelete event. Callback invoked before a group is deleted.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupDelete_Register(GroupDeleteCallback callback) {
    return __permissions_OnGroupDelete_Register(callback);
  }

  using _OnGroupDelete_Unregister = Status (*)(GroupDeleteCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupDelete_Unregister __permissions_OnGroupDelete_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupDelete event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupDelete_Unregister(GroupDeleteCallback callback) {
    return __permissions_OnGroupDelete_Unregister(callback);
  }

  using _OnGroupExpiration_Register = Status (*)(GroupExpirationCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupExpiration_Register __permissions_OnGroupExpiration_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupExpiration event. Callback invoked when a group in user has been expired.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupExpiration_Register(GroupExpirationCallback callback) {
    return __permissions_OnGroupExpiration_Register(callback);
  }

  using _OnGroupExpiration_Unregister = Status (*)(GroupExpirationCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupExpiration_Unregister __permissions_OnGroupExpiration_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupExpiration event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupExpiration_Unregister(GroupExpirationCallback callback) {
    return __permissions_OnGroupExpiration_Unregister(callback);
  }

  using _OnGroupOptionChangeStorage_Register = Status (*)(GroupOptionStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupOptionChangeStorage_Register __permissions_OnGroupOptionChangeStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupOptionChangeStorage event. Callback invoked when an option value is set for a group.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupOptionChangeStorage_Register(GroupOptionStorageCallback callback) {
    return __permissions_OnGroupOptionChangeStorage_Register(callback);
  }

  using _OnGroupOptionChangeStorage_Unregister = Status (*)(GroupOptionStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupOptionChangeStorage_Unregister __permissions_OnGroupOptionChangeStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupOptionChangeStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupOptionChangeStorage_Unregister(GroupOptionStorageCallback callback) {
    return __permissions_OnGroupOptionChangeStorage_Unregister(callback);
  }

  using _OnGroupOptionChange_Register = Status (*)(GroupOptionCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupOptionChange_Register __permissions_OnGroupOptionChange_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupOptionChange event. Callback invoked when an option value is set for a group.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupOptionChange_Register(GroupOptionCallback callback) {
    return __permissions_OnGroupOptionChange_Register(callback);
  }

  using _OnGroupOptionChange_Unregister = Status (*)(GroupOptionCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupOptionChange_Unregister __permissions_OnGroupOptionChange_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupOptionChange event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupOptionChange_Unregister(GroupOptionCallback callback) {
    return __permissions_OnGroupOptionChange_Unregister(callback);
  }

  using _OnGroupPermissionChangeStorage_Register = Status (*)(GroupPermissionStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupPermissionChangeStorage_Register __permissions_OnGroupPermissionChangeStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupPermissionChangeStorage event. Callback invoked when a permission is added or removed from a group.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupPermissionChangeStorage_Register(GroupPermissionStorageCallback callback) {
    return __permissions_OnGroupPermissionChangeStorage_Register(callback);
  }

  using _OnGroupPermissionChangeStorage_Unregister = Status (*)(GroupPermissionStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupPermissionChangeStorage_Unregister __permissions_OnGroupPermissionChangeStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupPermissionChangeStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupPermissionChangeStorage_Unregister(GroupPermissionStorageCallback callback) {
    return __permissions_OnGroupPermissionChangeStorage_Unregister(callback);
  }

  using _OnGroupPermissionChange_Register = Status (*)(GroupPermissionCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupPermissionChange_Register __permissions_OnGroupPermissionChange_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupPermissionChange event. Callback invoked when a permission is added or removed from a group.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupPermissionChange_Register(GroupPermissionCallback callback) {
    return __permissions_OnGroupPermissionChange_Register(callback);
  }

  using _OnGroupPermissionChange_Unregister = Status (*)(GroupPermissionCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupPermissionChange_Unregister __permissions_OnGroupPermissionChange_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupPermissionChange event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupPermissionChange_Unregister(GroupPermissionCallback callback) {
    return __permissions_OnGroupPermissionChange_Unregister(callback);
  }

  using _OnGroupsLoad_Register = Status (*)(LoadGroupsCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupsLoad_Register __permissions_OnGroupsLoad_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnGroupsLoad event. Called when the core requests loading of server groups.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnGroupsLoad_Register(LoadGroupsCallback callback) {
    return __permissions_OnGroupsLoad_Register(callback);
  }

  using _OnGroupsLoad_Unregister = Status (*)(LoadGroupsCallback);
}
extern "C" PLUGIN_API permissions::_OnGroupsLoad_Unregister __permissions_OnGroupsLoad_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnGroupsLoad event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnGroupsLoad_Unregister(LoadGroupsCallback callback) {
    return __permissions_OnGroupsLoad_Unregister(callback);
  }

  using _OnPermissionExpiration_Register = Status (*)(PermExpirationCallback);
}
extern "C" PLUGIN_API permissions::_OnPermissionExpiration_Register __permissions_OnPermissionExpiration_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnPermissionExpiration event. Callback invoked when a permission in user has been expired.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnPermissionExpiration_Register(PermExpirationCallback callback) {
    return __permissions_OnPermissionExpiration_Register(callback);
  }

  using _OnPermissionExpiration_Unregister = Status (*)(PermExpirationCallback);
}
extern "C" PLUGIN_API permissions::_OnPermissionExpiration_Unregister __permissions_OnPermissionExpiration_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnPermissionExpiration event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnPermissionExpiration_Unregister(PermExpirationCallback callback) {
    return __permissions_OnPermissionExpiration_Unregister(callback);
  }

  using _OnSetParentStorage_Register = Status (*)(SetParentStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnSetParentStorage_Register __permissions_OnSetParentStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnSetParentStorage event. Callback invoked when a parent group is set for a child group.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnSetParentStorage_Register(SetParentStorageCallback callback) {
    return __permissions_OnSetParentStorage_Register(callback);
  }

  using _OnSetParentStorage_Unregister = Status (*)(SetParentStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnSetParentStorage_Unregister __permissions_OnSetParentStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnSetParentStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnSetParentStorage_Unregister(SetParentStorageCallback callback) {
    return __permissions_OnSetParentStorage_Unregister(callback);
  }

  using _OnSetParent_Register = Status (*)(SetParentCallback);
}
extern "C" PLUGIN_API permissions::_OnSetParent_Register __permissions_OnSetParent_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnSetParent event. Callback invoked when a parent group is set for a child group.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnSetParent_Register(SetParentCallback callback) {
    return __permissions_OnSetParent_Register(callback);
  }

  using _OnSetParent_Unregister = Status (*)(SetParentCallback);
}
extern "C" PLUGIN_API permissions::_OnSetParent_Unregister __permissions_OnSetParent_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnSetParent event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnSetParent_Unregister(SetParentCallback callback) {
    return __permissions_OnSetParent_Unregister(callback);
  }

  using _OnUserCookieChangeStorage_Register = Status (*)(UserCookieStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserCookieChangeStorage_Register __permissions_OnUserCookieChangeStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserCookieChangeStorage event. Callback invoked when a cookie is set for a user.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserCookieChangeStorage_Register(UserCookieStorageCallback callback) {
    return __permissions_OnUserCookieChangeStorage_Register(callback);
  }

  using _OnUserCookieChangeStorage_Unregister = Status (*)(UserCookieStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserCookieChangeStorage_Unregister __permissions_OnUserCookieChangeStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserCookieChangeStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserCookieChangeStorage_Unregister(UserCookieStorageCallback callback) {
    return __permissions_OnUserCookieChangeStorage_Unregister(callback);
  }

  using _OnUserCookieChange_Register = Status (*)(UserCookieCallback);
}
extern "C" PLUGIN_API permissions::_OnUserCookieChange_Register __permissions_OnUserCookieChange_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserCookieChange event. Callback invoked when a cookie is set for a user.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserCookieChange_Register(UserCookieCallback callback) {
    return __permissions_OnUserCookieChange_Register(callback);
  }

  using _OnUserCookieChange_Unregister = Status (*)(UserCookieCallback);
}
extern "C" PLUGIN_API permissions::_OnUserCookieChange_Unregister __permissions_OnUserCookieChange_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserCookieChange event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserCookieChange_Unregister(UserCookieCallback callback) {
    return __permissions_OnUserCookieChange_Unregister(callback);
  }

  using _OnUserCreateStorage_Register = Status (*)(UserCreateStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserCreateStorage_Register __permissions_OnUserCreateStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserCreateStorage event. Callback invoked after a user is successfully created.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserCreateStorage_Register(UserCreateStorageCallback callback) {
    return __permissions_OnUserCreateStorage_Register(callback);
  }

  using _OnUserCreateStorage_Unregister = Status (*)(UserCreateStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserCreateStorage_Unregister __permissions_OnUserCreateStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserCreateStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserCreateStorage_Unregister(UserCreateStorageCallback callback) {
    return __permissions_OnUserCreateStorage_Unregister(callback);
  }

  using _OnUserCreate_Register = Status (*)(UserCreateCallback);
}
extern "C" PLUGIN_API permissions::_OnUserCreate_Register __permissions_OnUserCreate_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserCreate event. Callback invoked after a user is successfully created.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserCreate_Register(UserCreateCallback callback) {
    return __permissions_OnUserCreate_Register(callback);
  }

  using _OnUserCreate_Unregister = Status (*)(UserCreateCallback);
}
extern "C" PLUGIN_API permissions::_OnUserCreate_Unregister __permissions_OnUserCreate_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserCreate event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserCreate_Unregister(UserCreateCallback callback) {
    return __permissions_OnUserCreate_Unregister(callback);
  }

  using _OnUserDeleteStorage_Register = Status (*)(UserDeleteStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserDeleteStorage_Register __permissions_OnUserDeleteStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserDeleteStorage event. Callback invoked before a user is deleted.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserDeleteStorage_Register(UserDeleteStorageCallback callback) {
    return __permissions_OnUserDeleteStorage_Register(callback);
  }

  using _OnUserDeleteStorage_Unregister = Status (*)(UserDeleteStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserDeleteStorage_Unregister __permissions_OnUserDeleteStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserDeleteStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserDeleteStorage_Unregister(UserDeleteStorageCallback callback) {
    return __permissions_OnUserDeleteStorage_Unregister(callback);
  }

  using _OnUserDelete_Register = Status (*)(UserDeleteCallback);
}
extern "C" PLUGIN_API permissions::_OnUserDelete_Register __permissions_OnUserDelete_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserDelete event. Callback invoked before a user is deleted.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserDelete_Register(UserDeleteCallback callback) {
    return __permissions_OnUserDelete_Register(callback);
  }

  using _OnUserDelete_Unregister = Status (*)(UserDeleteCallback);
}
extern "C" PLUGIN_API permissions::_OnUserDelete_Unregister __permissions_OnUserDelete_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserDelete event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserDelete_Unregister(UserDeleteCallback callback) {
    return __permissions_OnUserDelete_Unregister(callback);
  }

  using _OnUserGroupChangeStorage_Register = Status (*)(UserGroupStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserGroupChangeStorage_Register __permissions_OnUserGroupChangeStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserGroupChangeStorage event. Callback invoked when a group is added or removed from a user.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserGroupChangeStorage_Register(UserGroupStorageCallback callback) {
    return __permissions_OnUserGroupChangeStorage_Register(callback);
  }

  using _OnUserGroupChangeStorage_Unregister = Status (*)(UserGroupStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserGroupChangeStorage_Unregister __permissions_OnUserGroupChangeStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserGroupChangeStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserGroupChangeStorage_Unregister(UserGroupStorageCallback callback) {
    return __permissions_OnUserGroupChangeStorage_Unregister(callback);
  }

  using _OnUserGroupChange_Register = Status (*)(UserGroupCallback);
}
extern "C" PLUGIN_API permissions::_OnUserGroupChange_Register __permissions_OnUserGroupChange_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserGroupChange event. Callback invoked when a group is added or removed from a user.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserGroupChange_Register(UserGroupCallback callback) {
    return __permissions_OnUserGroupChange_Register(callback);
  }

  using _OnUserGroupChange_Unregister = Status (*)(UserGroupCallback);
}
extern "C" PLUGIN_API permissions::_OnUserGroupChange_Unregister __permissions_OnUserGroupChange_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserGroupChange event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserGroupChange_Unregister(UserGroupCallback callback) {
    return __permissions_OnUserGroupChange_Unregister(callback);
  }

  using _OnUserImmunityChangeStorage_Register = Status (*)(UserImmunityStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserImmunityChangeStorage_Register __permissions_OnUserImmunityChangeStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserImmunityChangeStorage event. Callback invoked when immunity is set for a user.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserImmunityChangeStorage_Register(UserImmunityStorageCallback callback) {
    return __permissions_OnUserImmunityChangeStorage_Register(callback);
  }

  using _OnUserImmunityChangeStorage_Unregister = Status (*)(UserImmunityStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserImmunityChangeStorage_Unregister __permissions_OnUserImmunityChangeStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserImmunityChangeStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserImmunityChangeStorage_Unregister(UserImmunityStorageCallback callback) {
    return __permissions_OnUserImmunityChangeStorage_Unregister(callback);
  }

  using _OnUserImmunityChange_Register = Status (*)(UserImmunityCallback);
}
extern "C" PLUGIN_API permissions::_OnUserImmunityChange_Register __permissions_OnUserImmunityChange_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserImmunityChange event. Callback invoked when immunity is set for a user.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserImmunityChange_Register(UserImmunityCallback callback) {
    return __permissions_OnUserImmunityChange_Register(callback);
  }

  using _OnUserImmunityChange_Unregister = Status (*)(UserImmunityCallback);
}
extern "C" PLUGIN_API permissions::_OnUserImmunityChange_Unregister __permissions_OnUserImmunityChange_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserImmunityChange event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserImmunityChange_Unregister(UserImmunityCallback callback) {
    return __permissions_OnUserImmunityChange_Unregister(callback);
  }

  using _OnUserLoaded_Register = Status (*)(UserLoadedCallback);
}
extern "C" PLUGIN_API permissions::_OnUserLoaded_Register __permissions_OnUserLoaded_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserLoaded event. Called when a user's data has been fully loaded.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserLoaded_Register(UserLoadedCallback callback) {
    return __permissions_OnUserLoaded_Register(callback);
  }

  using _OnUserLoaded_Unregister = Status (*)(UserLoadedCallback);
}
extern "C" PLUGIN_API permissions::_OnUserLoaded_Unregister __permissions_OnUserLoaded_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserLoaded event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserLoaded_Unregister(UserLoadedCallback callback) {
    return __permissions_OnUserLoaded_Unregister(callback);
  }

  using _OnUserPermissionChangeStorage_Register = Status (*)(UserPermissionStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserPermissionChangeStorage_Register __permissions_OnUserPermissionChangeStorage_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserPermissionChangeStorage event. Callback invoked when a permission is added, removed, or replaced for a user.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserPermissionChangeStorage_Register(UserPermissionStorageCallback callback) {
    return __permissions_OnUserPermissionChangeStorage_Register(callback);
  }

  using _OnUserPermissionChangeStorage_Unregister = Status (*)(UserPermissionStorageCallback);
}
extern "C" PLUGIN_API permissions::_OnUserPermissionChangeStorage_Unregister __permissions_OnUserPermissionChangeStorage_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserPermissionChangeStorage event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserPermissionChangeStorage_Unregister(UserPermissionStorageCallback callback) {
    return __permissions_OnUserPermissionChangeStorage_Unregister(callback);
  }

  using _OnUserPermissionChange_Register = Status (*)(UserPermissionCallback);
}
extern "C" PLUGIN_API permissions::_OnUserPermissionChange_Register __permissions_OnUserPermissionChange_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserPermissionChange event. Callback invoked when a permission is added, removed, or replaced for a user.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserPermissionChange_Register(UserPermissionCallback callback) {
    return __permissions_OnUserPermissionChange_Register(callback);
  }

  using _OnUserPermissionChange_Unregister = Status (*)(UserPermissionCallback);
}
extern "C" PLUGIN_API permissions::_OnUserPermissionChange_Unregister __permissions_OnUserPermissionChange_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserPermissionChange event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserPermissionChange_Unregister(UserPermissionCallback callback) {
    return __permissions_OnUserPermissionChange_Unregister(callback);
  }

  using _OnUserRequest_Register = Status (*)(UserRequestCallback);
}
extern "C" PLUGIN_API permissions::_OnUserRequest_Register __permissions_OnUserRequest_Register;
namespace permissions {
  /**
   * @brief Registers a listener for the OnUserRequest event. Called when a user data load is requested.
   * @param callback (function): The callback to register.
   * @return int32
   */
  inline Status OnUserRequest_Register(UserRequestCallback callback) {
    return __permissions_OnUserRequest_Register(callback);
  }

  using _OnUserRequest_Unregister = Status (*)(UserRequestCallback);
}
extern "C" PLUGIN_API permissions::_OnUserRequest_Unregister __permissions_OnUserRequest_Unregister;
namespace permissions {
  /**
   * @brief Unregisters a previously registered listener for the OnUserRequest event.
   * @param callback (function): The callback to unregister.
   * @return int32
   */
  inline Status OnUserRequest_Unregister(UserRequestCallback callback) {
    return __permissions_OnUserRequest_Unregister(callback);
  }

} // namespace permissions
