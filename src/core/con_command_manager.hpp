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

	bool AddCommandListener(std::string_view name, CommandListenerCallback callback, HookMode mode);
	bool RemoveCommandListener(std::string_view name, CommandListenerCallback callback, HookMode mode);
	bool IsValidValveCommand(std::string_view name) const;
	bool AddValveCommand(std::string_view name, std::string_view description, ConVarFlag flags = ConVarFlag::None, uint64 adminFlags = 0);
	bool RemoveValveCommand(std::string_view name);

	ResultType DispatchConCommand(const CCommandContext* ctx, const CCommand* args, HookMode mode);
	ResultType ExecuteCommandCallbacks(std::string_view name, const CCommandContext& ctx, const CCommand& args, HookMode mode, CommandCallingContext callingContext);

private:
 	plg::flat_hash_map<plg::string, std::shared_ptr<ConCommandInfo>, plg::case_insensitive_hash, plg::case_insensitive_equal> m_cmdLookup;
	std::recursive_mutex m_mutex;
	plg::enum_map<ListenerManager<CommandListenerCallback>, HookMode> m_globalCallbacks;
};

extern ConCommandManager g_CommandManager;
