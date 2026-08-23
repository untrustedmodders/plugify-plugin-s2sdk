#include "permissions.hpp"

permissions::_OnGroupCreateStorage_Register __permissions_OnGroupCreateStorage_Register = nullptr;

permissions::_OnGroupCreateStorage_Unregister __permissions_OnGroupCreateStorage_Unregister = nullptr;

permissions::_OnGroupCreate_Register __permissions_OnGroupCreate_Register = nullptr;

permissions::_OnGroupCreate_Unregister __permissions_OnGroupCreate_Unregister = nullptr;

permissions::_OnGroupDeleteStorage_Register __permissions_OnGroupDeleteStorage_Register = nullptr;

permissions::_OnGroupDeleteStorage_Unregister __permissions_OnGroupDeleteStorage_Unregister = nullptr;

permissions::_OnGroupDelete_Register __permissions_OnGroupDelete_Register = nullptr;

permissions::_OnGroupDelete_Unregister __permissions_OnGroupDelete_Unregister = nullptr;

permissions::_OnGroupExpiration_Register __permissions_OnGroupExpiration_Register = nullptr;

permissions::_OnGroupExpiration_Unregister __permissions_OnGroupExpiration_Unregister = nullptr;

permissions::_OnGroupOptionChangeStorage_Register __permissions_OnGroupOptionChangeStorage_Register = nullptr;

permissions::_OnGroupOptionChangeStorage_Unregister __permissions_OnGroupOptionChangeStorage_Unregister = nullptr;

permissions::_OnGroupOptionChange_Register __permissions_OnGroupOptionChange_Register = nullptr;

permissions::_OnGroupOptionChange_Unregister __permissions_OnGroupOptionChange_Unregister = nullptr;

permissions::_OnGroupPermissionChangeStorage_Register __permissions_OnGroupPermissionChangeStorage_Register = nullptr;

permissions::_OnGroupPermissionChangeStorage_Unregister __permissions_OnGroupPermissionChangeStorage_Unregister = nullptr;

permissions::_OnGroupPermissionChange_Register __permissions_OnGroupPermissionChange_Register = nullptr;

permissions::_OnGroupPermissionChange_Unregister __permissions_OnGroupPermissionChange_Unregister = nullptr;

permissions::_OnGroupsLoad_Register __permissions_OnGroupsLoad_Register = nullptr;

permissions::_OnGroupsLoad_Unregister __permissions_OnGroupsLoad_Unregister = nullptr;

permissions::_OnPermissionExpiration_Register __permissions_OnPermissionExpiration_Register = nullptr;

permissions::_OnPermissionExpiration_Unregister __permissions_OnPermissionExpiration_Unregister = nullptr;

permissions::_OnSetParentStorage_Register __permissions_OnSetParentStorage_Register = nullptr;

permissions::_OnSetParentStorage_Unregister __permissions_OnSetParentStorage_Unregister = nullptr;

permissions::_OnSetParent_Register __permissions_OnSetParent_Register = nullptr;

permissions::_OnSetParent_Unregister __permissions_OnSetParent_Unregister = nullptr;

permissions::_OnUserCookieChangeStorage_Register __permissions_OnUserCookieChangeStorage_Register = nullptr;

permissions::_OnUserCookieChangeStorage_Unregister __permissions_OnUserCookieChangeStorage_Unregister = nullptr;

permissions::_OnUserCookieChange_Register __permissions_OnUserCookieChange_Register = nullptr;

permissions::_OnUserCookieChange_Unregister __permissions_OnUserCookieChange_Unregister = nullptr;

permissions::_OnUserCreateStorage_Register __permissions_OnUserCreateStorage_Register = nullptr;

permissions::_OnUserCreateStorage_Unregister __permissions_OnUserCreateStorage_Unregister = nullptr;

permissions::_OnUserCreate_Register __permissions_OnUserCreate_Register = nullptr;

permissions::_OnUserCreate_Unregister __permissions_OnUserCreate_Unregister = nullptr;

permissions::_OnUserDeleteStorage_Register __permissions_OnUserDeleteStorage_Register = nullptr;

permissions::_OnUserDeleteStorage_Unregister __permissions_OnUserDeleteStorage_Unregister = nullptr;

permissions::_OnUserDelete_Register __permissions_OnUserDelete_Register = nullptr;

permissions::_OnUserDelete_Unregister __permissions_OnUserDelete_Unregister = nullptr;

permissions::_OnUserGroupChangeStorage_Register __permissions_OnUserGroupChangeStorage_Register = nullptr;

permissions::_OnUserGroupChangeStorage_Unregister __permissions_OnUserGroupChangeStorage_Unregister = nullptr;

permissions::_OnUserGroupChange_Register __permissions_OnUserGroupChange_Register = nullptr;

permissions::_OnUserGroupChange_Unregister __permissions_OnUserGroupChange_Unregister = nullptr;

permissions::_OnUserImmunityChangeStorage_Register __permissions_OnUserImmunityChangeStorage_Register = nullptr;

permissions::_OnUserImmunityChangeStorage_Unregister __permissions_OnUserImmunityChangeStorage_Unregister = nullptr;

permissions::_OnUserImmunityChange_Register __permissions_OnUserImmunityChange_Register = nullptr;

permissions::_OnUserImmunityChange_Unregister __permissions_OnUserImmunityChange_Unregister = nullptr;

permissions::_OnUserLoaded_Register __permissions_OnUserLoaded_Register = nullptr;

permissions::_OnUserLoaded_Unregister __permissions_OnUserLoaded_Unregister = nullptr;

permissions::_OnUserPermissionChangeStorage_Register __permissions_OnUserPermissionChangeStorage_Register = nullptr;

permissions::_OnUserPermissionChangeStorage_Unregister __permissions_OnUserPermissionChangeStorage_Unregister = nullptr;

permissions::_OnUserPermissionChange_Register __permissions_OnUserPermissionChange_Register = nullptr;

permissions::_OnUserPermissionChange_Unregister __permissions_OnUserPermissionChange_Unregister = nullptr;

permissions::_OnUserRequest_Register __permissions_OnUserRequest_Register = nullptr;

permissions::_OnUserRequest_Unregister __permissions_OnUserRequest_Unregister = nullptr;

permissions::_SetParent __permissions_SetParent = nullptr;

permissions::_DumpPermissions __permissions_DumpPermissions = nullptr;

permissions::_GetParent __permissions_GetParent = nullptr;

permissions::_CanAffectUser __permissions_CanAffectUser = nullptr;

permissions::_DumpPermissionsGroup __permissions_DumpPermissionsGroup = nullptr;

permissions::_GetAllGroups __permissions_GetAllGroups = nullptr;

permissions::_HasPermissionExtended __permissions_HasPermissionExtended = nullptr;

permissions::_HasPermissionGroupExtended __permissions_HasPermissionGroupExtended = nullptr;

permissions::_HasPermission __permissions_HasPermission = nullptr;

permissions::_HasPermissionGroup __permissions_HasPermissionGroup = nullptr;

permissions::_HasGroupExtended __permissions_HasGroupExtended = nullptr;

permissions::_HasParentGroup __permissions_HasParentGroup = nullptr;

permissions::_HasGroup __permissions_HasGroup = nullptr;

permissions::_GetPriorityGroup __permissions_GetPriorityGroup = nullptr;

permissions::_GetUserGroups __permissions_GetUserGroups = nullptr;

permissions::_AddPermissionGroup __permissions_AddPermissionGroup = nullptr;

permissions::_GetImmunity __permissions_GetImmunity = nullptr;

permissions::_SetImmunity __permissions_SetImmunity = nullptr;

permissions::_SetPermissionGroup __permissions_SetPermissionGroup = nullptr;

permissions::_AddPermission __permissions_AddPermission = nullptr;

permissions::_RemovePermissionGroup __permissions_RemovePermissionGroup = nullptr;

permissions::_SetPermission __permissions_SetPermission = nullptr;

permissions::_GetOptionGroup __permissions_GetOptionGroup = nullptr;

permissions::_SetOptionGroup __permissions_SetOptionGroup = nullptr;

permissions::_GetAllOptionsGroup __permissions_GetAllOptionsGroup = nullptr;

permissions::_CreateGroup __permissions_CreateGroup = nullptr;

permissions::_RemovePermission __permissions_RemovePermission = nullptr;

permissions::_DeleteGroup __permissions_DeleteGroup = nullptr;

permissions::_AddGroup __permissions_AddGroup = nullptr;

permissions::_GroupExists __permissions_GroupExists = nullptr;

permissions::_LoadGroups __permissions_LoadGroups = nullptr;

permissions::_RemoveGroup __permissions_RemoveGroup = nullptr;

permissions::_GetCookie __permissions_GetCookie = nullptr;

permissions::_SetCookie __permissions_SetCookie = nullptr;

permissions::_GetAllCookies __permissions_GetAllCookies = nullptr;

permissions::_CreateUser __permissions_CreateUser = nullptr;

permissions::_DeleteUser __permissions_DeleteUser = nullptr;

permissions::_UserExists __permissions_UserExists = nullptr;

permissions::_DumpUsersList __permissions_DumpUsersList = nullptr;

permissions::_LoadUser __permissions_LoadUser = nullptr;

