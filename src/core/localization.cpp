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

	std::string_view ClientLanguage(CPlayerSlot slot) {
		auto player = g_PlayerManager.ToPlayer(slot);
		if (player != nullptr && !player->GetLanguageCode().empty()) {
			return player->GetLanguageCode();
		}
		return ServerLanguage();
	}
}// namespace

std::string_view lang::GetCode(std::string_view name) {
	auto it = s_languageCodes.find(name);
	return it != s_languageCodes.end() ? it->second : std::string_view{};
}

Result<void> lang::Initialize(const plg::vector<plg::string>& paths) {
	if (!__translations_LoadTranslation) {
		return {};
	}

	if (auto error = translations::LoadTranslation(paths); !error.empty()) {
		return MakeError("Reading error: {}", error);
	}

	if (__translations_SetDefaultLanguage) {
		translations::SetDefaultLanguage(plg::string(ServerLanguage()));
	}
	return {};
}

std::optional<plg::string> lang::Get(CPlayerSlot slot, std::string_view key) {
	if (!__translations_Translate) {
		return std::nullopt;
	}

	return translations::Translate(plg::string(ClientLanguage(slot)), plg::string(key));
}

std::optional<plg::string> lang::Get(CPlayerSlot slot, std::string_view key, std::format_args args) {
	auto text = Get(slot, key);
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
