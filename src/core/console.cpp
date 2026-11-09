#include "console.hpp"

#include <tier0/logging.h>
#include <convar.h>
#include <icvar.h>

static plg::string s_capturedOutput;
static int s_captureDepth = 0;

class LoggingListener final : public ILoggingListener {
public:
	void Log(const LoggingContext_t* context, const tchar* message) override {
		s_capturedOutput += message;
	}
} g_LoggingListener;

// Splits text into commands on ';' and new lines, like the engine command buffer.
static std::vector<std::string> SplitCommands(std::string_view text) {
	std::vector<std::string> commands;
	bool inQuotes = false;
	size_t start = 0;
	for (size_t i = 0; i <= text.size(); ++i) {
		if (i < text.size()) {
			char c = text[i];
			if (c == '"') {
				inQuotes = !inQuotes;
				continue;
			}
			if (c != '\n' && (inQuotes || c != ';'))
				continue;
			inQuotes = false;
		}
		auto command = text.substr(start, i - start);
		if (command.find_first_not_of(" \t\r") != std::string_view::npos)
			commands.emplace_back(command);
		start = i + 1;
	}
	return commands;
}

static void ExecuteCommand(const std::string& text) {
	CCommand args;
	if (!args.Tokenize(text.c_str()) || args.ArgC() == 0)
		return;

	auto command = g_pCVar->FindConCommand(args.Arg(0));
	if (command.IsValidRef()) {
		CCommandContext context(CommandTarget_t::CT_NO_TARGET, CPlayerSlot(-1));
		g_pCVar->DispatchConCommand(command, context, args);
		return;
	}

	ConVarRefAbstract conVar(g_pCVar->FindConVar(args.Arg(0)));
	if (conVar.IsValidRef() && conVar.IsConVarDataValid()) {
		if (args.ArgC() == 1) {
			Msg("%s = %s\n", conVar.GetName(), conVar.GetString().Get());
		} else {
			conVar.SetString(args.ArgC() == 2 ? args.Arg(1) : args.ArgS());
		}
		return;
	}

	Msg("Unknown command '%s'\n", args.Arg(0));
}

plg::string ExecuteServerCommandCapture(std::string_view commands) {
	if (s_captureDepth++ == 0) {
		s_capturedOutput.clear();
		LoggingSystem_PushLoggingState(false, false);
		LoggingSystem_RegisterLoggingListener(&g_LoggingListener);
	}

	// A nested call (a command run from here calls back into us) returns only its own output.
	size_t offset = s_capturedOutput.size();
	for (const auto& command : SplitCommands(commands)) {
		ExecuteCommand(command);
	}
	plg::string result = s_capturedOutput.substr(offset);

	if (--s_captureDepth == 0) {
		LoggingSystem_PopLoggingState(false);
		s_capturedOutput.clear();
	}
	return result;
}
