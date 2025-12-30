#include <core/con_command_manager.hpp>
#include <core/console.hpp>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Creates a console command as an administrative command.
 * If the command does not exist, it is created. When this command is invoked,
 * the access rights of the player are automatically checked before allowing it to continue.
 *
 * @param name The name of the console command.
 * @param adminFlags The admin flags that indicate which admin level can use this command.
 * @param description A brief description of what the command does.
 * @param flags Command flags that define the behavior of the command.
 * @param callback A callback function that is invoked when the command is executed.
 * @param mode Whether the hook was in post mode (after processing) or pre mode (before processing).
 * @return A boolean indicating whether the command was successfully added.
 */
extern "C" PLUGIN_API bool AddAdminCommand(const plg::string& name, int64_t adminFlags, const plg::string& description, ConVarFlag flags, CommandListenerCallback callback, HookMode mode) {
	auto result = g_ConCommandManager.AddValveCommand(name, description, flags, adminFlags);
	g_ConCommandManager.AddCommandListener(name, callback, mode);
	return result;
}

/**
 * @brief Creates a console command or hooks an already existing one.
 *
 * @param name The name of the console command.
 * @param description A brief description of what the command does.
 * @param flags Command flags that define the behavior of the command.
 * @param callback A callback function that is invoked when the command is executed.
 * @param mode Whether the hook was in post mode (after processing) or pre mode (before processing).
 * @return A boolean indicating whether the command was successfully added.
 */
extern "C" PLUGIN_API bool AddConsoleCommand(const plg::string& name, const plg::string& description, ConVarFlag flags, CommandListenerCallback callback, HookMode mode) {
	auto result = g_ConCommandManager.AddValveCommand(name, description, flags);
	g_ConCommandManager.AddCommandListener(name, callback, mode);
	return result;
}

/**
 * @brief Removes a console command from the system.
 *
 * @param name The name of the command to be removed.
 * @param callback The callback function associated with the command to be removed.
 */
extern "C" PLUGIN_API bool RemoveCommand(const plg::string& name, CommandListenerCallback callback) {
	g_ConCommandManager.RemoveCommandListener(name, callback, HookMode::Pre);
	g_ConCommandManager.RemoveCommandListener(name, callback, HookMode::Post);
	return g_ConCommandManager.RemoveValveCommand(name);
}

/**
 * @brief Adds a callback that will fire when a command is sent to the server.
 *
 * @param name The name of the command.
 * @param callback The callback function that will be invoked when the command is executed.
 * @param mode Whether the hook was in post mode (after processing) or pre mode (before processing).
 * @return A boolean indicating whether the callback was successfully added.
 */
extern "C" PLUGIN_API bool AddCommandListener(const plg::string& name, CommandListenerCallback callback, HookMode mode) {
	return g_ConCommandManager.AddCommandListener(name, callback, mode);
}

/**
 * @brief Removes a callback that fires when a command is sent to the server.
 *
 * @param name The name of the command.
 * @param callback The callback function to be removed.
 * @param mode Whether the hook was in post mode (after processing) or pre mode (before processing).
 * @return A boolean indicating whether the callback was successfully removed.
 */
extern "C" PLUGIN_API bool RemoveCommandListener(const plg::string& name, CommandListenerCallback callback, HookMode mode) {
	return g_ConCommandManager.RemoveCommandListener(name, callback, mode);
}

/**
 * @brief Executes a server command as if it were run on the server console or through RCON.
 *
 * @param command The command to execute on the server.
 */
extern "C" PLUGIN_API void ServerCommand(const plg::string& command) {
	auto cleanCommand = std::format("{}\n", command);
	g_pEngineServer->ServerCommand(cleanCommand.c_str());
}
/**
 * @brief Executes a server command as if it were on the server console (or RCON) and stores the printed text into buffer.
 *
 * @param command The command to execute on the server.
 * @return String to store command result into.
 */
extern "C" PLUGIN_API plg::string ServerCommandEx(const plg::string& command) {
	auto cleanCommand = std::format("{}\n", command);

	if (!g_ShouldCatchSpew) {
		g_ShouldCatchSpew = true;
		g_pEngineServer->ServerCommand("s2_conhook_start\n");
		g_pEngineServer->ServerCommand(cleanCommand.c_str());
		g_pEngineServer->ServerCommand("s2_conhook_stop\n");
	} else {
		g_pEngineServer->ServerCommand(cleanCommand.c_str());
	}

	switch (g_ServerCommandBuffer.size()) {
		case 0:
			return "";

		case 1:
			return g_ServerCommandBuffer[0];

		default:
			plg::string result = plg::join(g_ServerCommandBuffer, "");
			g_ServerCommandBuffer.clear();
			return result;
	}
}

/**
 * @brief Executes a client command.
 * This will only work if clients have `cl_restrict_server_commands` set to 0.
 *
 * @param playerSlot The index of the player's slot executing the command.
 * @param command The command to execute on the client.
 */
extern "C" PLUGIN_API void ClientCommand(int playerSlot, const plg::string& command) {
	auto cleanCommand = std::format("{}\n", command);
	g_pEngineServer->ClientCommand(playerSlot, "%s", cleanCommand.c_str());
}

/**
 * @brief Executes a client command on the server without network communication.
 *
 * @param playerSlot The index of the player's slot.
 * @param command The command to be executed by the client.
 */
extern "C" PLUGIN_API void FakeClientCommand(int playerSlot, const plg::string& command) {
	CCommand args;
	CUtlString str(command);
	args.Tokenize(str);
	auto handle = g_pCVar->FindConCommand(args.Arg(0));
	if (!handle.IsValidRef())
		return;

	CCommandContext context(CommandTarget_t::CT_NO_TARGET, playerSlot);

	g_pCVar->DispatchConCommand(handle, context, args);
}

/**
 * @brief Returns the names of all registered console commands and cvars.
 *
 * @return The vector of command/cvar names.
 */
extern "C" PLUGIN_API plg::vector<plg::string> GetAllConCommands(ConVarFlag flags) {
	plg::vector<plg::string> commands;
	ConCommandRef cmd = g_pCVar->FindFirstConCommand();
	for (; cmd.IsValidRef(); cmd = g_pCVar->FindNextConCommand(cmd)) {
		if (flags == ConVarFlag::None || cmd.IsFlagSet(static_cast<uint64>(flags))) {
			commands.push_back(cmd.GetName());
		}
	}
	return commands;
}

/**
 * @brief Returns all console commands registered by this plugin.
 *
 * Collects only commands explicitly registered through the S2SDK plugin
 * command manager. Engine-native commands and commands from other plugins
 * are not included.
 *
 * @return The vector of ConCommand names.
 */
extern "C" PLUGIN_API plg::vector<plg::string> GetAllCommands() {
	return g_ConCommandManager.GetAllCommands();
}
