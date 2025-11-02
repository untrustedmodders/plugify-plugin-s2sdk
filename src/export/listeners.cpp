#include "core/listeners.hpp"

extern "C" {
#undef DECLARE_ACCESSOR
#define DECLARE_ACCESSOR(name, ret, ...) \
	using name##Callback = ret (*)(__VA_ARGS__); \
	PLUGIN_API void name##_Register(name##Callback callback) { g_##name##ListenerManager.Register(callback); } \
	PLUGIN_API void name##_Register2(name##Callback callback, int priority) { g_##name##ListenerManager.Register(callback, priority); } \
	PLUGIN_API void name##_Unregister(name##Callback callback) { g_##name##ListenerManager.Unregister(callback); }

	LISTENER_LIST(DECLARE_ACCESSOR)
}
