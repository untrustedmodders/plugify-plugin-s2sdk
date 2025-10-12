#include "con_command_manager.hpp"
#include "player_manager.hpp"
#include "core_config.hpp"

#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/utils.h>

#include <icvar.h>
#include <igameevents.h>

static void CommandCallback(const CCommandContext&, const CCommand&) {
}

ConCommandInfo::ConCommandInfo(plg::string name, plg::string description) : name(std::move(name)), description(std::move(description)) {
}

ConCommandInfo::~ConCommandInfo() {
	if (!g_pCVar || defaultCommand) {
		return;
	}

	g_pCVar->UnregisterConCommandCallbacks(commandRef);
}

bool ConCommandManager::AddCommandListener(const plg::string& name, CommandListenerCallback callback, HookMode mode) {
	if (name.empty()) {
		return m_globalCallbacks[static_cast<size_t>(mode)].Register(callback);
	}

	auto it = m_cmdLookup.find(name);
	if (it == m_cmdLookup.end()) {
		ConCommandRef commandRef = g_pCVar->FindConCommand(name.c_str());
		if (!commandRef.IsValidRef()) {
			return false;
		}

		auto& commandInfo = *m_cmdLookup.emplace(name, std::make_unique<ConCommandInfo>(name)).first->second;
		commandInfo.command = g_pCVar->GetConCommandData(commandRef);
		commandInfo.defaultCommand = true;
		return commandInfo.callbacks[static_cast<size_t>(mode)].Register(callback);
	} else {
		auto& commandInfo = *std::get<CommandInfoPtr>(*it);
		return commandInfo.callbacks[static_cast<size_t>(mode)].Register(callback);
	}
}

bool ConCommandManager::RemoveCommandListener(const plg::string& name, CommandListenerCallback callback, HookMode mode) {
	if (name.empty()) {
		return m_globalCallbacks[static_cast<size_t>(mode)].Unregister(callback);
	}

	auto it = m_cmdLookup.find(name);
	if (it == m_cmdLookup.end()) {
		return false;
	}

	auto& commandInfo = *std::get<CommandInfoPtr>(*it);
	return commandInfo.callbacks[static_cast<size_t>(mode)].Unregister(callback);
}

bool ConCommandManager::AddValveCommand(const plg::string& name, const plg::string& description, ConVarFlag flags, uint64 adminFlags) {
	if (name.empty() || g_pCVar->FindConVar(name.c_str()).IsValidRef()) {
		S2_LOGF(LS_DEBUG, "[ConCommandManager::AddValveCommand]: Command '{}' is empty or already exists\n", name);
		return false;
	}

	if (g_pCVar->FindConCommand(name.c_str()).IsValidRef()) {
		S2_LOGF(LS_DEBUG, "[ConCommandManager::AddValveCommand]: Command '{}' already exists\n", name);
		return false;
	}

	auto it = m_cmdLookup.find(name);
	if (it != m_cmdLookup.end()) {
		S2_LOGF(LS_DEBUG, "[ConCommandManager::AddValveCommand]: Command '{}' already exists\n", name);
		return false;
	}

	auto& commandInfo = *m_cmdLookup.emplace(name, std::make_unique<ConCommandInfo>(name, description)).first->second;

	ConCommandCreation_t setup;
	setup.m_pszName = commandInfo.name.c_str();
	setup.m_pszHelpString = commandInfo.description.c_str();
	setup.m_nFlags = SanitiseConVarFlags(static_cast<uint64>(flags));
	setup.m_CBInfo = { &CommandCallback };
	setup.m_CompletionCBInfo = {};

	commandInfo.commandRef = g_pCVar->RegisterConCommand(setup, ConVar_GetDefaultFlags());
	commandInfo.command = commandInfo.commandRef.GetRawData();
	commandInfo.adminFlags = adminFlags;

	return true;
}

bool ConCommandManager::RemoveValveCommand(const plg::string& name) {
	auto commandRef = g_pCVar->FindConCommand(name.c_str());
	if (!commandRef.IsValidRef()) {
		return false;
	}

	auto it = m_cmdLookup.find(name);
	if (it != m_cmdLookup.end()) {
		m_cmdLookup.erase(it);
		return true;
	} else {
		g_pCVar->UnregisterConCommandCallbacks(commandRef);
	}

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
	//S2_LOGF(LS_DEBUG, "[ConCommandManager::ExecuteCommandCallbacks][{}]: {}\n", mode == HookMode::Pre ? "Pre" : "Post", name);

	int size = args.ArgC();

	plg::vector<plg::string> arguments;
	arguments.reserve(static_cast<size_t>(size));
	for (int i = 0; i < size; ++i) {
		arguments.emplace_back(args.Arg(i));
	}

	CPlayerSlot caller = ctx.GetPlayerSlot();

	ResultType result = ResultType::Continue;

	const auto& globalCallback = m_globalCallbacks[static_cast<size_t>(mode)];

	for (size_t i = 0; i < globalCallback.GetCount(); ++i) {
		auto thisResult = globalCallback.Notify(i, caller, callingContext, arguments);
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

	auto it = m_cmdLookup.find(name);
	if (it != m_cmdLookup.end()) {
		const auto& commandInfo = *std::get<CommandInfoPtr>(*it);

		if (!CheckCommandAccess(caller, commandInfo.adminFlags)) {
			return result;
		}

		const auto& callback = commandInfo.callbacks[static_cast<size_t>(mode)];

		for (size_t i = 0; i < callback.GetCount(); ++i) {
			auto thisResult = callback.Notify(i, caller, callingContext, arguments);
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

	//S2_LOGF(LS_DEBUG, "[ConCommandManager::Hook_DispatchConCommand]: {}\n", name);

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