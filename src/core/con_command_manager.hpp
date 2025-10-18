#pragma once

#include "listener_manager.hpp"
#include "con_var_manager.hpp"
#include <core/sdk/utils.h>

#include <convar.h>

enum class CommandCallingContext  {
	Console = 0,
	Chat = 1,
};

using CommandListenerCallback = ResultType (*)(int caller, CommandCallingContext context, const plg::vector<plg::string>& arguments);

struct ConCommandInfo {
	~ConCommandInfo();

	ConCommandData* command{};
	ConCommandRef commandRef{};
	plg::enum_map<ListenerManager<CommandListenerCallback>, HookMode> callbacks;
	bool defaultCommand{};
};

class ConCommandManager {
public:
	ConCommandManager() = default;
	~ConCommandManager() = default;

	bool AddCommandListener(const plg::string& name, CommandListenerCallback callback, HookMode mode);
	bool RemoveCommandListener(const plg::string& name, CommandListenerCallback callback, HookMode mode);
	bool IsValidValveCommand(const plg::string& name) const;
	bool AddValveCommand(const plg::string& name, const plg::string& description, ConVarFlag flags = ConVarFlag::None, uint64 adminFlags = 0);
	bool RemoveValveCommand(const plg::string& name);

	ResultType DispatchConCommand(const CCommandContext* ctx, const CCommand* args, HookMode mode);
	ResultType ExecuteCommandCallbacks(const plg::string& name, const CCommandContext& ctx, const CCommand& args, HookMode mode, CommandCallingContext callingContext);

private:
 	plg::parallel_flat_hash_map<plg::string, std::shared_ptr<ConCommandInfo>, plg::case_insensitive_hash, plg::case_insensitive_equal> m_cmdLookup;
	plg::enum_map<ListenerManager<CommandListenerCallback>, HookMode> m_globalCallbacks;
};

extern ConCommandManager g_CommandManager;
