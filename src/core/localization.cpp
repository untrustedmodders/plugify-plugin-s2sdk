#include "localization.hpp"

#include <core/core_config.hpp>
#include <core/player_manager.hpp>

#include <translations/translations.hpp>

namespace {
	// `cl_language` reports the Steam language name; translation files are keyed by ISO code.
	const plg::flat_hash_map<std::string_view, std::string_view, plg::case_insensitive_hash, plg::case_insensitive_equal> s_languageCodes = {
		{"english", "en"},
		{"german", "de"},
		{"french", "fr"},
		{"italian", "it"},
		{"koreana", "ko"},
		{"spanish", "es"},
		{"schinese", "zh-CN"},
		{"tchinese", "zh-TW"},
		{"russian", "ru"},
		{"thai", "th"},
		{"japanese", "ja"},
		{"portuguese", "pt"},
		{"polish", "pl"},
		{"danish", "da"},
		{"dutch", "nl"},
		{"finnish", "fi"},
		{"norwegian", "no"},
		{"swedish", "sv"},
		{"hungarian", "hu"},
		{"czech", "cs"},
		{"romanian", "ro"},
		{"turkish", "tr"},
		{"brazilian", "pt-BR"},
		{"bulgarian", "bg"},
		{"greek", "el"},
		{"ukrainian", "uk"},
		{"latam", "es-419"},
		{"vietnamese", "vi"},
		{"indonesian", "id"},
	};

	std::string_view ServerLanguage() {
		return g_pCoreConfig != nullptr ? g_pCoreConfig->ServerLanguage : "en";
	}
}// namespace

Result<void> lang::Initialize(const plg::vector<plg::string>& paths) {
	if (auto error = translations::LoadTranslation(paths); !error.empty()) {
		return MakeError("Reading error: {}", error);
	}
	return {};
}

std::string_view lang::GetClientLanguageCode(int playerSlot) {
	auto player = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
	if (player != nullptr) {
		auto it = s_languageCodes.find(player->GetLanguage());
		if (it != s_languageCodes.end()) {
			return it->second;
		}
	}
	return ServerLanguage();
}

plg::string lang::Get(int playerSlot, std::string_view key) {
	plg::string name(key);
	plg::string client = GetClientLanguageCode(playerSlot);
	plg::string server = ServerLanguage();

	plg::string text = translations::Translate(client, name);
	if (text.empty() && client != server) {
		text = translations::Translate(server, name);
	}
	return text;
}

plg::string lang::Get(int playerSlot, std::string_view key, std::format_args args) {
	plg::string text = Get(playerSlot, key);
	if (text.empty()) {
		return {};
	}

	try {
		return std::vformat(text, args);
	} catch (const std::format_error& e) {
		plg::print(LS_WARNING, "Built-in text for '{}' does not match its arguments: {}\n", key, e.what());
		return text;
	}
}
