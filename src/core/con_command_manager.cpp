#include "con_command_manager.hpp"
#include "player_manager.hpp"
#include "core_config.hpp"

#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/utils.h>

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

bool ConCommandManager::AddCommandListener(const plg::string& name, CommandListenerCallback callback, HookMode mode) {
	if (name.empty()) {
		return m_globalCallbacks[mode].Register(callback);
	}

	if (auto commandInfo = plg::find(m_cmdLookup, name)) {
		return commandInfo->callbacks[mode].Register(callback);
	} else {
		ConCommandRef commandRef = g_pCVar->FindConCommand(name.c_str());
		if (!commandRef.IsValidRef()) {
			plg::print(LS_WARNING, "Command {} is not exists\n", name);
			return false;
		}

		commandInfo = m_cmdLookup.emplace(name, std::make_shared<ConCommandInfo>()).first->second;
		commandInfo->command = g_pCVar->GetConCommandData(commandRef);
		commandInfo->defaultCommand = true;
		return commandInfo->callbacks[mode].Register(callback);
	}
}

bool ConCommandManager::RemoveCommandListener(const plg::string& name, CommandListenerCallback callback, HookMode mode) {
	if (name.empty()) {
		return m_globalCallbacks[mode].Unregister(callback);
	}

	if (auto commandInfo = plg::find(m_cmdLookup, name)) {
		return commandInfo->callbacks[mode].Unregister(callback);
	}

	return false;
}

bool ConCommandManager::AddValveCommand(const plg::string& name, const plg::string& description, ConVarFlag flags, uint64 adminFlags) {
	if (name.empty()) {
		plg::print(LS_WARNING, "Command name empty\n", name);
		return false;
	}

	auto commandInfo = plg::find(m_cmdLookup, name);
	if (commandInfo || g_pCVar->FindConVar(name.c_str()).IsValidRef() || g_pCVar->FindConCommand(name.c_str()).IsValidRef()) {
		plg::print(LS_WARNING, "Command '{}' is already exists\n", name);
		return false;
	}

	commandInfo = m_cmdLookup.emplace(name, std::make_shared<ConCommandInfo>()).first->second;

	ConCommandCreation_t setup;
	setup.m_pszName = name.c_str();
	setup.m_pszHelpString = description.c_str();
	setup.m_nFlags = SanitiseConVarFlags(static_cast<uint64>(flags));
	setup.m_CBInfo = { &CommandCallback };
	setup.m_CompletionCBInfo = {};

	commandInfo->commandRef = g_pCVar->RegisterConCommand(setup, ConVar_GetDefaultFlags());
	commandInfo->command = commandInfo->commandRef.GetRawData();
	//commandInfo->adminFlags = adminFlags;

	return true;
}

bool ConCommandManager::RemoveValveCommand(const plg::string& name) {
	auto commandRef = g_pCVar->FindConCommand(name.c_str());
	if (!commandRef.IsValidRef()) {
		return false;
	}

	if (m_cmdLookup.erase(name) != 0) {
		return true;
	}

	g_pCVar->UnregisterConCommandCallbacks(commandRef);
	return true;
}

bool ConCommandManager::IsValidValveCommand(const plg::string& name) const {
	ConCommandRef commandRef = g_pCVar->FindConCommand(name.c_str());
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

ResultType ConCommandManager::ExecuteCommandCallbacks(const plg::string& name, const CCommandContext& ctx, const CCommand& args, HookMode mode, CommandCallingContext callingContext) {
	plg::print(LS_DETAILED, "[ConCommandManager::ExecuteCommandCallbacks][{}]: {}\n", mode == HookMode::Pre ? "Pre" : "Post", name);

	int size = args.ArgC();

	plg::vector<plg::string> arguments;
	arguments.reserve(static_cast<size_t>(size));
	for (int i = 0; i < size; ++i) {
		arguments.emplace_back(args.Arg(i));
	}

	CPlayerSlot caller = ctx.GetPlayerSlot();

	ResultType result = ResultType::Continue;

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

	if (auto commandInfo = plg::find(m_cmdLookup, name)) {
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

	const char* arg0 = args->Arg(0);

	plg::print(LS_DETAILED, "[ConCommandManager::Hook_DispatchConCommand]: {}\n", arg0);

	static const char sayCommand[] = "say";
	constexpr size_t sayNullTerminated = sizeof(sayCommand) - 1;
	if (!std::strncmp(arg0, sayCommand, sayNullTerminated)) {
		if (!arg0[sayNullTerminated] || !std::strcmp(&arg0[sayNullTerminated], "_team")) {
			const char* arg1 = args->Arg(1);
			while (*arg1 == ' ') {
				arg1++;
			}

			bool silent = g_pCoreConfig->IsSilentChatTrigger(arg1);
			if (silent || g_pCoreConfig->IsPublicChatTrigger(arg1)) {
				char* message = (char*) (args->ArgS() + 1);
				++message;

				// Trailing slashes are only removed if Host_Say has been called.
				bool hostSay = silent && mode == HookMode::Pre;
				if (hostSay) message[std::strlen(message) - 1] = 0;

				CCommand nargs;
				nargs.Tokenize(message);

				auto result = ExecuteCommandCallbacks(nargs[0], *ctx, nargs, mode, CommandCallingContext::Chat);
				if (result >= ResultType::Handled || hostSay) {
					return ResultType::Stop;
				}
			}
		}
	}

	return ExecuteCommandCallbacks(arg0, *ctx, *args, mode, CommandCallingContext::Console);
}

ConCommandManager g_CommandManager;