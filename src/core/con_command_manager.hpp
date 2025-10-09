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
	ConCommandInfo() = delete;
	explicit ConCommandInfo(plg::string name, plg::string description = {});
	~ConCommandInfo();

	plg::string name;
	plg::string description;
	uint64 adminFlags{};
	ConCommandData* command{};
	ConCommandRef commandRef{};
	std::array<ListenerManager<CommandListenerCallback>, 2> callbacks;
	bool defaultCommand{};
};

using CommandInfoPtr = std::unique_ptr<ConCommandInfo>;

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
	//std::vector<ConCommandInfo*> m_cmdList;
	plg::map<plg::string, CommandInfoPtr, plg::case_insensitive_equal> m_cmdLookup;
	std::array<ListenerManager<CommandListenerCallback>, 2> m_globalCallbacks;
};

extern ConCommandManager g_CommandManager;
