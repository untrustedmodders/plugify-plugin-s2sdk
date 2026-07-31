#pragma once

class CoreConfig {
public:
	explicit CoreConfig(plg::vector<plg::string> paths);
	~CoreConfig();

	Result<bool> Initialize();

	const plg::vector<plg::string>& GetPaths() const;

	bool IsSilentChatTrigger(std::string_view message) const;
	bool IsPublicChatTrigger(std::string_view message) const;
	bool IsRegexMatch(std::string_view message) const;

private:
	static bool IsTriggerInternal(const std::vector<plg::string>& triggers, std::string_view message);

public:
	std::vector<plg::string> PublicChatTrigger{"!"};
	std::vector<plg::string> SilentChatTrigger{"/"};
	//std::vector<std::regex> FilterConsoleCleaner{};
	plg::string ServerLanguage{"en"};
	bool FollowCS2ServerGuidelines{true};
	bool FixFlashAlertMessage{true};
	bool FixServerListPlayer{true};
	bool FixLoadMotd{true};

	// Menu system settings
	bool MenuEnabled{true};								// Master toggle for the built-in menu implementation (chat/console/centerhtml/button types + their input commands).
															// Disable if you're bringing your own via the RegisterMenuType/CreateMenuHandle exports; the core menu system itself stays usable either way.
	std::vector<plg::string> MenuCommandPrefixes{"css_"};	// Command prefixes producing <prefix>0.."<prefix>9 digit-select commands (e.g. "sm_" for SourceMod-style binds)
	std::vector<plg::string> MenuSelectCommands{"menuselect"};	// Command names that take a single 0-based item index argument (SourceMod-style); empty to disable
	plg::string MenuDisabledColor{"#808080"};				// Hex color for disabled items in HTML-rendered menus (centerhtml, button)
	plg::string MenuHighlightColor{"#FFD700"};				// Hex color for the highlighted row in the WASD button menu
	int MenuCenterHtmlDuration{30};						// Seconds the centerhtml menu panel is shown before it needs a redraw
	int MenuButtonHtmlDuration{2};							// Seconds the WASD button menu's HTML panel is shown per redraw
	float MenuButtonRefreshInterval{1.5f};					// Seconds between keep-alive redraws of the WASD button menu when there's no input
	bool MenuButtonFreezePlayer{true};						// Freeze the player's movement while a WASD button menu is open
	uint64_t MenuButtonKeyUp{};							// Button (InputBitMask_t name, e.g. "IN_FORWARD") that moves the WASD button-menu cursor up
	uint64_t MenuButtonKeyDown{};							// Button that moves the WASD button-menu cursor down
	uint64_t MenuButtonKeySelect{};						// Button that selects the highlighted item
	uint64_t MenuButtonKeyExit{};							// Button that exits the WASD button menu

private:
	plg::vector<plg::string> m_paths;
};
