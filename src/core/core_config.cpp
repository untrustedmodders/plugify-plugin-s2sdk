#include "core_config.hpp"
#include <core/sdk/entity/globaltypes.h>
#include <core/sdk/utils.hpp>
#include <configs/configs.hpp>

using namespace std::string_view_literals;

namespace {
	InputBitMask_t ParseButtonName(std::string_view name, InputBitMask_t defaultValue) {
		static const plg::flat_hash_map<std::string_view, InputBitMask_t> buttons = {
			{"IN_ATTACK", IN_ATTACK},
			{"IN_JUMP", IN_JUMP},
			{"IN_DUCK", IN_DUCK},
			{"IN_FORWARD", IN_FORWARD},
			{"IN_BACK", IN_BACK},
			{"IN_USE", IN_USE},
			{"IN_TURNLEFT", IN_TURNLEFT},
			{"IN_TURNRIGHT", IN_TURNRIGHT},
			{"IN_MOVELEFT", IN_MOVELEFT},
			{"IN_MOVERIGHT", IN_MOVERIGHT},
			{"IN_ATTACK2", IN_ATTACK2},
			{"IN_RELOAD", IN_RELOAD},
			{"IN_SPEED", IN_SPEED},
			{"IN_JOYAUTOSPRINT", IN_JOYAUTOSPRINT},
			{"IN_USEORRELOAD", IN_USEORRELOAD},
			{"IN_SCORE", IN_SCORE},
			{"IN_ZOOM", IN_ZOOM},
			{"IN_LOOK_AT_WEAPON", IN_LOOK_AT_WEAPON},
		};

		if (name.empty()) {
			return defaultValue;
		}

		auto it = buttons.find(name);
		if (it == buttons.end()) {
			plg::print(LS_WARNING, "Unknown menu button name '{}'\n", name);
			return defaultValue;
		}
		return it->second;
	}
}// namespace

CoreConfig::CoreConfig(plg::vector<plg::string> paths) : m_paths(std::move(paths)) {
}

CoreConfig::~CoreConfig() {
	/*for (auto& regex : FilterConsoleCleaner) {
		delete regex;
	}*/
}

Result<bool> CoreConfig::Initialize() {
	auto config = configs::Config(m_paths);
	if (auto error = config.GetError(); !error.empty()) {
		return MakeError("Reading error: {}", error);
	}

	PublicChatTrigger.clear();
	if (config.JumpKey("PublicChatTrigger")) {
		if (config.IsArray() && config.JumpFirst()) {
			do {
				PublicChatTrigger.emplace_back(config.GetString());
			} while (config.JumpNext());
			config.JumpBack();
		}
		config.JumpBack();
	}

	SilentChatTrigger.clear();
	if (config.JumpKey("SilentChatTrigger")) {
		if (config.IsArray() && config.JumpFirst()) {
			do {
				SilentChatTrigger.emplace_back(config.GetString());
			} while (config.JumpNext());
			config.JumpBack();
		}
		config.JumpBack();
	}

	/*FilterConsoleCleaner.clear();
	if (config.JumpKey("FilterConsoleCleaner")) {
		if (config.IsArray() && config.JumpFirst()) {
			do {
				plg::string pattern = config.GetString();
				FilterConsoleCleaner.emplace_back(pattern.begin(), pattern.end());
			} while (config.JumpNext());
			config.JumpBack();
		}
		config.JumpBack();
	}*/

	ServerLanguage = config.GetString("ServerLanguage", "en");
	FollowCS2ServerGuidelines = config.GetBool("FollowCS2ServerGuidelines", true);
	FixFlashAlertMessage = config.GetBool("FixFlashAlertMessage", true);
	FixServerListPlayer = config.GetBool("FixServerListPlayer", true);
	FixLoadMotd = config.GetBool("FixLoadMotd", true);

	MenuEnabled = config.GetBool("MenuEnabled", true);

	MenuCommandPrefixes.clear();
	if (config.JumpKey("MenuCommandPrefixes")) {
		if (config.IsArray() && config.JumpFirst()) {
			do {
				MenuCommandPrefixes.emplace_back(config.GetString());
			} while (config.JumpNext());
			config.JumpBack();
		}
		config.JumpBack();
	}

	MenuSelectCommands.clear();
	if (config.JumpKey("MenuSelectCommands")) {
		if (config.IsArray() && config.JumpFirst()) {
			do {
				MenuSelectCommands.emplace_back(config.GetString());
			} while (config.JumpNext());
			config.JumpBack();
		}
		config.JumpBack();
	}

	MenuDisabledColor = config.GetString("MenuDisabledColor", "#808080");
	MenuHighlightColor = config.GetString("MenuHighlightColor", "#FFD700");
	MenuCenterHtmlDuration = static_cast<int>(config.GetInt("MenuCenterHtmlDuration", 30));
	MenuButtonHtmlDuration = static_cast<int>(config.GetInt("MenuButtonHtmlDuration", 2));
	MenuButtonRefreshInterval = static_cast<float>(config.GetFloat("MenuButtonRefreshInterval", 1.5));
	MenuButtonFreezePlayer = config.GetBool("MenuButtonFreezePlayer", true);
	MenuButtonMaxItems = static_cast<int>(config.GetInt("MenuButtonMaxItems", 6));
	MenuButtonKeyUp = ParseButtonName(config.GetString("MenuButtonKeyUp", "IN_FORWARD"), IN_FORWARD);
	MenuButtonKeyDown = ParseButtonName(config.GetString("MenuButtonKeyDown", "IN_BACK"), IN_BACK);
	MenuButtonKeyLeft = ParseButtonName(config.GetString("MenuButtonKeyLeft", "IN_MOVELEFT"), IN_MOVELEFT);
	MenuButtonKeyRight = ParseButtonName(config.GetString("MenuButtonKeyRight", "IN_MOVERIGHT"), IN_MOVERIGHT);
	MenuButtonKeySelect = ParseButtonName(config.GetString("MenuButtonKeySelect", "IN_USE"), IN_USE);
	MenuButtonKeyExit = ParseButtonName(config.GetString("MenuButtonKeyExit", "IN_RELOAD"), IN_RELOAD);
	MenuButtonImagePath = config.GetString("MenuButtonImagePath", "resource/menus");
	MenuButtonImageExtension = config.GetString("MenuButtonImageExtension", "vsvg");
	MenuButtonImageWidth = static_cast<int>(config.GetInt("MenuButtonImageWidth", 52));
	MenuButtonImageHeight = static_cast<int>(config.GetInt("MenuButtonImageHeight", 35));
	MenuButtonImageUp = config.GetString("MenuButtonImageUp", "w");
	MenuButtonImageDown = config.GetString("MenuButtonImageDown", "s");
	MenuButtonImageLeft = config.GetString("MenuButtonImageLeft", "a");
	MenuButtonImageRight = config.GetString("MenuButtonImageRight", "d");
	MenuButtonImageSelect = config.GetString("MenuButtonImageSelect", "e");
	MenuButtonImageExit = config.GetString("MenuButtonImageExit", "f");
	MenuButtonImageEmpty = config.GetString("MenuButtonImageEmpty", "empty");
	MenuButtonImageEmptyHalf = config.GetString("MenuButtonImageEmptyHalf", "empty_half");
	MenuSoundScroll = config.GetString("MenuSoundScroll", "");
	MenuSoundClick = config.GetString("MenuSoundClick", "");
	MenuSoundBack = config.GetString("MenuSoundBack", "");
	MenuSoundExit = config.GetString("MenuSoundExit", "");
	MenuSoundDisabled = config.GetString("MenuSoundDisabled", "");

	return {};
}

const plg::vector<plg::string>& CoreConfig::GetPaths() const {
	return m_paths;
}

bool CoreConfig::IsTriggerInternal(const std::vector<plg::string>& triggers, std::string_view message) {
	for (const plg::string& trigger : triggers) {
		if (message.starts_with(trigger)) {
			return true;
		}
	}

	return false;
}

bool CoreConfig::IsSilentChatTrigger(std::string_view message) const {
	return IsTriggerInternal(SilentChatTrigger, message);
}

bool CoreConfig::IsPublicChatTrigger(std::string_view message) const {
	return IsTriggerInternal(PublicChatTrigger, message);
}

bool CoreConfig::IsRegexMatch(std::string_view message) const {
	/*for (const auto& regex : FilterConsoleCleaner) {
		if (std::regex_match(message.begin(), message.end(), regex))
			return true;
	}*/
	return false;
}