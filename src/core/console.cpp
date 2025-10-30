#include <tier0/logging.h>
#include <convar.h>

plg::hybrid_vector<plg::string, 32> g_ServerCommandBuffer;
std::atomic<bool> g_ShouldCatchSpew = false;

class LoggingListener final : public ILoggingListener {
public:
	void Log(const LoggingContext_t* context, const tchar* message) override {
		g_ServerCommandBuffer.emplace_back(message);
	}
} g_LoggingListener;

CON_COMMAND_F(s2_conhook_start, "", FCVAR_PROTECTED) {
	if (!g_ShouldCatchSpew)
		return;

	LoggingSystem_PushLoggingState(false, false);
	LoggingSystem_RegisterLoggingListener(&g_LoggingListener);
}

CON_COMMAND_F(s2_conhook_stop, "", FCVAR_PROTECTED) {
	if (!g_ShouldCatchSpew)
		return;

	LoggingSystem_PopLoggingState(false);
	g_ShouldCatchSpew = false;
}
