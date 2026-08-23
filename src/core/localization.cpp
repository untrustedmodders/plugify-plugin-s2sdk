#include "localization.hpp"

#include <core/core_config.hpp>
#include <core/player_manager.hpp>

#include <translations/translations.hpp>

namespace {
	std::string_view ServerLanguage() {
		return g_pCoreConfig != nullptr ? g_pCoreConfig->ServerLanguage : "en";
	}

	std::string_view ClientLanguage(int playerSlot) {
		auto player = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
		if (player != nullptr) {
			return player->GetLanguageCode();
		}
		return ServerLanguage();
	}
}// namespace

Result<void> lang::Initialize(const plg::vector<plg::string>& paths) {
	if (!__translations_LoadTranslation) {
		return {};
	}

	if (auto error = translations::LoadTranslation(paths); !error.empty()) {
		return MakeError("Reading error: {}", error);
	}
	return {};
}

std::optional<plg::string> lang::Get(int playerSlot, std::string_view key) {
	if (!__translations_Translate) {
		return std::nullopt;
	}

	plg::string name(key);
	plg::string client = ClientLanguage(playerSlot);
	plg::string server = ServerLanguage();

	plg::string text = translations::Translate(client, name);
	if (text.empty() && client != server) {
		text = translations::Translate(server, name);
	}
	return text;
}

std::optional<plg::string> lang::Get(int playerSlot, std::string_view key, std::format_args args) {
	auto text = Get(playerSlot, key);
	if (!text || text->empty()) {
		return text;
	}

	try {
		return std::vformat(*text, args);
	} catch (const std::format_error& e) {
		plg::print(LS_WARNING, "Built-in text for '{}' does not match its arguments: {}\n", key, e.what());
		return text;
	}
}
