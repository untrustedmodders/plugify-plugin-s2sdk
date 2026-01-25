#include "polyhook.hpp"

polyhook::_HookDetour __polyhook_HookDetour = nullptr;

polyhook::_HookDetour2 __polyhook_HookDetour2 = nullptr;

polyhook::_HookVirtualTable __polyhook_HookVirtualTable = nullptr;

polyhook::_HookVirtualTable2 __polyhook_HookVirtualTable2 = nullptr;

polyhook::_HookVirtualFunc __polyhook_HookVirtualFunc = nullptr;

polyhook::_HookVirtualFunc2 __polyhook_HookVirtualFunc2 = nullptr;

polyhook::_UnhookDetour __polyhook_UnhookDetour = nullptr;

polyhook::_UnhookVirtualTable __polyhook_UnhookVirtualTable = nullptr;

polyhook::_UnhookVirtualTable2 __polyhook_UnhookVirtualTable2 = nullptr;

polyhook::_UnhookVirtualFunc __polyhook_UnhookVirtualFunc = nullptr;

polyhook::_UnhookVirtualFunc2 __polyhook_UnhookVirtualFunc2 = nullptr;

polyhook::_FindDetour __polyhook_FindDetour = nullptr;

polyhook::_FindVirtual __polyhook_FindVirtual = nullptr;

polyhook::_FindVirtual2 __polyhook_FindVirtual2 = nullptr;

polyhook::_GetVirtualIndex __polyhook_GetVirtualIndex = nullptr;

polyhook::_UnhookAll __polyhook_UnhookAll = nullptr;

polyhook::_UnhookAllVirtual __polyhook_UnhookAllVirtual = nullptr;

polyhook::_AddCallback __polyhook_AddCallback = nullptr;

polyhook::_AddCallback2 __polyhook_AddCallback2 = nullptr;

polyhook::_RemoveCallback __polyhook_RemoveCallback = nullptr;

polyhook::_IsCallbackRegistered __polyhook_IsCallbackRegistered = nullptr;

polyhook::_AreCallbacksRegistered __polyhook_AreCallbacksRegistered = nullptr;

polyhook::_GetFunctionAddr __polyhook_GetFunctionAddr = nullptr;

polyhook::_GetOriginalAddr __polyhook_GetOriginalAddr = nullptr;

polyhook::_GetArgumentBool __polyhook_GetArgumentBool = nullptr;

polyhook::_GetArgumentInt8 __polyhook_GetArgumentInt8 = nullptr;

polyhook::_GetArgumentUInt8 __polyhook_GetArgumentUInt8 = nullptr;

polyhook::_GetArgumentInt16 __polyhook_GetArgumentInt16 = nullptr;

polyhook::_GetArgumentUInt16 __polyhook_GetArgumentUInt16 = nullptr;

polyhook::_GetArgumentInt32 __polyhook_GetArgumentInt32 = nullptr;

polyhook::_GetArgumentUInt32 __polyhook_GetArgumentUInt32 = nullptr;

polyhook::_GetArgumentInt64 __polyhook_GetArgumentInt64 = nullptr;

polyhook::_GetArgumentUInt64 __polyhook_GetArgumentUInt64 = nullptr;

polyhook::_GetArgumentFloat __polyhook_GetArgumentFloat = nullptr;

polyhook::_GetArgumentDouble __polyhook_GetArgumentDouble = nullptr;

polyhook::_GetArgumentPointer __polyhook_GetArgumentPointer = nullptr;

polyhook::_GetArgumentString __polyhook_GetArgumentString = nullptr;

polyhook::_GetArgument __polyhook_GetArgument = nullptr;

polyhook::_SetArgumentBool __polyhook_SetArgumentBool = nullptr;

polyhook::_SetArgumentInt8 __polyhook_SetArgumentInt8 = nullptr;

polyhook::_SetArgumentUInt8 __polyhook_SetArgumentUInt8 = nullptr;

polyhook::_SetArgumentInt16 __polyhook_SetArgumentInt16 = nullptr;

polyhook::_SetArgumentUInt16 __polyhook_SetArgumentUInt16 = nullptr;

polyhook::_SetArgumentInt32 __polyhook_SetArgumentInt32 = nullptr;

polyhook::_SetArgumentUInt32 __polyhook_SetArgumentUInt32 = nullptr;

polyhook::_SetArgumentInt64 __polyhook_SetArgumentInt64 = nullptr;

polyhook::_SetArgumentUInt64 __polyhook_SetArgumentUInt64 = nullptr;

polyhook::_SetArgumentFloat __polyhook_SetArgumentFloat = nullptr;

polyhook::_SetArgumentDouble __polyhook_SetArgumentDouble = nullptr;

polyhook::_SetArgumentPointer __polyhook_SetArgumentPointer = nullptr;

polyhook::_SetArgumentString __polyhook_SetArgumentString = nullptr;

polyhook::_SetArgument __polyhook_SetArgument = nullptr;

polyhook::_GetReturnBool __polyhook_GetReturnBool = nullptr;

polyhook::_GetReturnInt8 __polyhook_GetReturnInt8 = nullptr;

polyhook::_GetReturnUInt8 __polyhook_GetReturnUInt8 = nullptr;

polyhook::_GetReturnInt16 __polyhook_GetReturnInt16 = nullptr;

polyhook::_GetReturnUInt16 __polyhook_GetReturnUInt16 = nullptr;

polyhook::_GetReturnInt32 __polyhook_GetReturnInt32 = nullptr;

polyhook::_GetReturnUInt32 __polyhook_GetReturnUInt32 = nullptr;

polyhook::_GetReturnInt64 __polyhook_GetReturnInt64 = nullptr;

polyhook::_GetReturnUInt64 __polyhook_GetReturnUInt64 = nullptr;

polyhook::_GetReturnFloat __polyhook_GetReturnFloat = nullptr;

polyhook::_GetReturnDouble __polyhook_GetReturnDouble = nullptr;

polyhook::_GetReturnPointer __polyhook_GetReturnPointer = nullptr;

polyhook::_GetReturnString __polyhook_GetReturnString = nullptr;

polyhook::_GetReturn __polyhook_GetReturn = nullptr;

polyhook::_SetReturnBool __polyhook_SetReturnBool = nullptr;

polyhook::_SetReturnInt8 __polyhook_SetReturnInt8 = nullptr;

polyhook::_SetReturnUInt8 __polyhook_SetReturnUInt8 = nullptr;

polyhook::_SetReturnInt16 __polyhook_SetReturnInt16 = nullptr;

polyhook::_SetReturnUInt16 __polyhook_SetReturnUInt16 = nullptr;

polyhook::_SetReturnInt32 __polyhook_SetReturnInt32 = nullptr;

polyhook::_SetReturnUInt32 __polyhook_SetReturnUInt32 = nullptr;

polyhook::_SetReturnInt64 __polyhook_SetReturnInt64 = nullptr;

polyhook::_SetReturnUInt64 __polyhook_SetReturnUInt64 = nullptr;

polyhook::_SetReturnFloat __polyhook_SetReturnFloat = nullptr;

polyhook::_SetReturnDouble __polyhook_SetReturnDouble = nullptr;

polyhook::_SetReturnPointer __polyhook_SetReturnPointer = nullptr;

polyhook::_SetReturnString __polyhook_SetReturnString = nullptr;

polyhook::_SetReturn __polyhook_SetReturn = nullptr;

polyhook::_GetRegisterBool __polyhook_GetRegisterBool = nullptr;

polyhook::_GetRegisterInt8 __polyhook_GetRegisterInt8 = nullptr;

polyhook::_GetRegisterUInt8 __polyhook_GetRegisterUInt8 = nullptr;

polyhook::_GetRegisterInt16 __polyhook_GetRegisterInt16 = nullptr;

polyhook::_GetRegisterUInt16 __polyhook_GetRegisterUInt16 = nullptr;

polyhook::_GetRegisterInt32 __polyhook_GetRegisterInt32 = nullptr;

polyhook::_GetRegisterUInt32 __polyhook_GetRegisterUInt32 = nullptr;

polyhook::_GetRegisterInt64 __polyhook_GetRegisterInt64 = nullptr;

polyhook::_GetRegisterUInt64 __polyhook_GetRegisterUInt64 = nullptr;

polyhook::_GetRegisterFloat __polyhook_GetRegisterFloat = nullptr;

polyhook::_GetRegisterDouble __polyhook_GetRegisterDouble = nullptr;

polyhook::_GetRegisterPointer __polyhook_GetRegisterPointer = nullptr;

polyhook::_GetRegisterString __polyhook_GetRegisterString = nullptr;

polyhook::_GetRegister __polyhook_GetRegister = nullptr;

polyhook::_SetRegisterBool __polyhook_SetRegisterBool = nullptr;

polyhook::_SetRegisterInt8 __polyhook_SetRegisterInt8 = nullptr;

polyhook::_SetRegisterUInt8 __polyhook_SetRegisterUInt8 = nullptr;

polyhook::_SetRegisterInt16 __polyhook_SetRegisterInt16 = nullptr;

polyhook::_SetRegisterUInt16 __polyhook_SetRegisterUInt16 = nullptr;

polyhook::_SetRegisterInt32 __polyhook_SetRegisterInt32 = nullptr;

polyhook::_SetRegisterUInt32 __polyhook_SetRegisterUInt32 = nullptr;

polyhook::_SetRegisterInt64 __polyhook_SetRegisterInt64 = nullptr;

polyhook::_SetRegisterUInt64 __polyhook_SetRegisterUInt64 = nullptr;

polyhook::_SetRegisterFloat __polyhook_SetRegisterFloat = nullptr;

polyhook::_SetRegisterDouble __polyhook_SetRegisterDouble = nullptr;

polyhook::_SetRegisterPointer __polyhook_SetRegisterPointer = nullptr;

polyhook::_SetRegisterString __polyhook_SetRegisterString = nullptr;

polyhook::_SetRegister __polyhook_SetRegister = nullptr;

