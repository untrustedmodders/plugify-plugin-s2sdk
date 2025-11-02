#include "con_command_manager.hpp"
#include "player_manager.hpp"
#include "core_config.hpp"

#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/utils.hpp>

#include <icvar.h>
#include <igameevents.h>

static void CommandCallback(const CCommandContext&, const CCommand&) {
}

ConCommandInfo::~ConCommandInfo() {
	if (!g_pCVar || defaultCommand) {
		return;
	}

	g_pCVar->UnregisterConCommandCallbacks(commandRef);
}

bool ConCommandManager::AddCommandListener(std::string_view name, CommandListenerCallback callback, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	if (name.empty()) {
		return m_globalCallbacks[mode].Register(callback);
	}

	std::shared_ptr<ConCommandInfo> commandInfo;
	{
		auto it = m_cmdLookup.find(name);
		if (it != m_cmdLookup.end()) {
			commandInfo = it->second;
		} else {
			ConCommandRef commandRef = g_pCVar->FindConCommand(name.data());
			if (!commandRef.IsValidRef()) {
				plg::print(LS_WARNING, "Command {} is not exists\n", name);
				return false;
			}
			commandInfo = std::make_shared<ConCommandInfo>();
			commandInfo->command = g_pCVar->GetConCommandData(commandRef);
			commandInfo->defaultCommand = true;
			m_cmdLookup.emplace(name, commandInfo);
		}
	}
	return commandInfo->callbacks[mode].Register(callback);
}

bool ConCommandManager::RemoveCommandListener(std::string_view name, CommandListenerCallback callback, HookMode mode) {
	std::scoped_lock lock(m_mutex);

	if (name.empty()) {
		return m_globalCallbacks[mode].Unregister(callback);
	}

	auto it = m_cmdLookup.find(name);
	if (it != m_cmdLookup.end()) {
		auto commandInfo = it->second;
		auto status = commandInfo->callbacks[mode].Unregister(callback);
		if (commandInfo->callbacks[HookMode::Pre].Empty() && commandInfo->callbacks[HookMode::Post].Empty()) {
			m_cmdLookup.erase(it);
		}
		return status;
	}

	return false;
}

bool ConCommandManager::AddValveCommand(std::string_view name, std::string_view description, ConVarFlag flags, uint64 adminFlags) {
	std::scoped_lock lock(m_mutex);

	if (name.empty()) {
		plg::print(LS_WARNING, "Command name empty\n", name);
		return false;
	}

	auto it = m_cmdLookup.find(name);
	if (it != m_cmdLookup.end() || g_pCVar->FindConVar(name.data()).IsValidRef() || g_pCVar->FindConCommand(name.data()).IsValidRef()) {
		plg::print(LS_WARNING, "Command '{}' is already exists\n", name);
		return false;
	}

	auto commandInfo = std::make_shared<ConCommandInfo>();

	ConCommandCreation_t setup;
	setup.m_pszName = name.data();
	setup.m_pszHelpString = description.data();
	setup.m_nFlags = SanitiseConVarFlags(static_cast<uint64>(flags));
	setup.m_CBInfo = { &CommandCallback };
	setup.m_CompletionCBInfo = {};

	commandInfo->commandRef = g_pCVar->RegisterConCommand(setup, ConVar_GetDefaultFlags());
	commandInfo->command = commandInfo->commandRef.GetRawData();
	//commandInfo->adminFlags = adminFlags;

	m_cmdLookup.emplace(name, commandInfo);
	return true;
}

bool ConCommandManager::RemoveValveCommand(std::string_view name) {
	std::scoped_lock lock(m_mutex);

	auto commandRef = g_pCVar->FindConCommand(name.data());
	if (!commandRef.IsValidRef()) {
		return false;
	}

	if (m_cmdLookup.erase(name) != 0) {
		return true;
	}

	g_pCVar->UnregisterConCommandCallbacks(commandRef);
	return true;
}

bool ConCommandManager::IsValidValveCommand(std::string_view name) {
	std::scoped_lock lock(m_mutex);

	ConCommandRef commandRef = g_pCVar->FindConCommand(name.data());
	return commandRef.IsValidRef();
}

static bool CheckCommandAccess(CPlayerSlot slot, uint64 flags) {
	if (!flags) {
		return true;
	}

	auto pPlayer = g_PlayerManager.ToPlayer(slot);
	if (pPlayer == nullptr) {
		return false;
	}

	/*if (!pPlayer->IsAdminFlagSet(flags)) {
		utils::PrintChat(slot, "You don't have access to this command.");
		return false;
	}*/

	return true;
}

ResultType ConCommandManager::ExecuteCommandCallbacks(std::string_view name, const CCommandContext& ctx, const CCommand& args, HookMode mode, CommandCallingContext callingContext) {
	plg::print(LS_DETAILED, "[ConCommandManager::ExecuteCommandCallbacks][{}]: {}\n", mode == HookMode::Pre ? "Pre" : "Post", name);

	int size = args.ArgC();

	plg::vector<plg::string> arguments;
	arguments.reserve(static_cast<size_t>(size));
	for (int i = 0; i < size; ++i) {
		arguments.emplace_back(args.Arg(i));
	}

	CPlayerSlot caller = ctx.GetPlayerSlot();

	ResultType result = ResultType::Continue;

	std::scoped_lock lock(m_mutex);

	{
		auto funcs = m_globalCallbacks[mode].Get();
		for (const auto& func : funcs) {
			auto thisResult = func(caller, callingContext, arguments);
			if (thisResult >= ResultType::Stop) {
				if (mode == HookMode::Pre) {
					return ResultType::Stop;
				}

				result = thisResult;
				break;
			}

			if (thisResult >= ResultType::Handled) {
				result = thisResult;
			}
		}
	}

	auto it = m_cmdLookup.find(name);
	if (it != m_cmdLookup.end()) {
		auto commandInfo = it->second;
		/*if (!CheckCommandAccess(caller, commandInfo->adminFlags)) {
			return result;
		}*/

		auto funcs = commandInfo->callbacks[mode].Get();
		for (const auto& func : funcs) {
			auto thisResult = func(caller, callingContext, arguments);
			if (thisResult >= ResultType::Handled) {
				return thisResult;
			} else if (thisResult > result) {
				result = thisResult;
			}
		}
	}

	return result;
}

ResultType ConCommandManager::DispatchConCommand(const CCommandContext* ctx, const CCommand* args, HookMode mode) {
	if (ctx == nullptr || args == nullptr) {
        return ResultType::Continue;
    }

    std::string_view arg0 = args->Arg(0);

    plg::print(LS_DETAILED, "[ConCommandManager::Hook_DispatchConCommand]: {}\n", arg0);

	auto trim_left = [](std::string_view& str, char ch) {
		while (!str.empty() && str.front() == ch) {
			str.remove_prefix(1);
		}
	};

	auto trim_right = [](std::string_view& str, char ch) {
		while (!str.empty() && str.back() == ch) {
			str.remove_suffix(1);
		}
	};

    if (arg0.starts_with("say") || arg0.starts_with("say_team")) {
    	std::string_view arg1 = args->Arg(1);
    	trim_left(arg1, ' ');
    	bool silentChat = g_pCoreConfig->IsSilentChatTrigger(arg1);
    	bool publicChat = g_pCoreConfig->IsPublicChatTrigger(arg1);
    	if (silentChat || publicChat) {
    		std::string_view message_sv = args->ArgS();
    		trim_left(message_sv, '"');
    		trim_right(message_sv, '"');
    		message_sv.remove_prefix(1);

    		CCommand nargs;
    		nargs.Tokenize(CUtlString(message_sv));

    		auto result = ExecuteCommandCallbacks(nargs[0], *ctx, nargs, mode, CommandCallingContext::Chat);
    		if (result >= ResultType::Handled || silentChat) {
    			return ResultType::Stop;
    		}
    	}
    }

    return ExecuteCommandCallbacks(arg0, *ctx, *args, mode, CommandCallingContext::Console);
}
