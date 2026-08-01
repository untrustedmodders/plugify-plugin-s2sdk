#pragma once

#include <core/sdk/entity/globaltypes.h>

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
	bool MenuEnabled{true};											// Master toggle for the built-in menu implementation (chat/console/centerhtml/button types + their input commands).
																	// Disable if you're bringing your own via the RegisterMenuType/CreateMenuHandle exports; the core menu system itself stays usable either way.
	std::vector<plg::string> MenuCommandPrefixes{"css_"};		// Command prefixes producing <prefix>0.."<prefix>9 digit-select commands (e.g. "sm_" for SourceMod-style binds)
	std::vector<plg::string> MenuSelectCommands{"menuselect"};	// Command names that take a single 0-based item index argument (SourceMod-style); empty to disable
	plg::string MenuDisabledColor{"#808080"};				// Hex color for disabled items in HTML-rendered menus (centerhtml, button)
	plg::string MenuHighlightColor{"#FFD700"};			// Hex color for the highlighted row in the WASD button menu
	int MenuCenterHtmlDuration{30};							// Seconds the centerhtml menu panel is shown before it needs a redraw
	int MenuButtonHtmlDuration{2};							// Seconds the WASD button menu's HTML panel is shown per redraw
	float MenuButtonRefreshInterval{0.1f};					// Seconds between keep-alive redraws of the WASD button menu when there's no input
	bool MenuButtonFreezePlayer{true};						// Freeze the player's movement while a WASD button menu is open
	plg::string MenuButtonFontClass;						// Panorama CSS class (e.g. "fontSize-sm", "fontSize-s") wrapping the WASD button menu's whole
															// body, to shrink/grow its text; empty keeps the default (unscaled) text size
	plg::string MenuButtonTitleFontClass;					// Same, but wrapping just the title line; empty keeps the default (unscaled) title size
	int MenuButtonMaxItems{6};								// Max items shown per page in the WASD button menu; menus asking for a smaller page size are left
															// alone, only larger/unlimited ones get clamped down to this. Keeps the item area + footer within
															// a fixed, always-visible height regardless of how many items a menu actually has.
	InputBitMask_t MenuButtonKeyUp{};						// Button (InputBitMask_t name, e.g. "IN_FORWARD") that moves the WASD button-menu cursor up
	InputBitMask_t MenuButtonKeyDown{};						// Button that moves the WASD button-menu cursor down
	InputBitMask_t MenuButtonKeyLeft{};						// Button that jumps a full page back
	InputBitMask_t MenuButtonKeyRight{};					// Button that jumps a full page forward
	InputBitMask_t MenuButtonKeySelect{};					// Button that selects the highlighted item
	InputBitMask_t MenuButtonKeyExit{};						// Button that exits the WASD button menu
	// Button-menu items and controls are always rendered as images from a mounted workshop addon (e.g. workshop
	// id 3763619947); requires that addon to be mounted server-side. Each button's image name gets a "-p" suffix
	// appended while the corresponding key (MenuButtonKeyUp/Down/etc above) is held.
	plg::string MenuButtonImagePath{"resource/menus"};		// s2r:// resource path (relative to the addon content root) the button images live under
	plg::string MenuButtonImageExtension{"vsvg"};			// File extension (without the dot) of the button images
	int MenuButtonImageWidth{52};							// Width, in pixels, each button image is rendered at
	int MenuButtonImageHeight{35};							// Height, in pixels, each button image is rendered at
	plg::string MenuButtonImageUp{"w"};						// Image name (without extension) for the up button
	plg::string MenuButtonImageDown{"s"};					// Same, for the down button
	plg::string MenuButtonImageLeft{"a"};					// Same, for the page-back button; only shown when a previous page exists
	plg::string MenuButtonImageRight{"d"};					// Same, for the page-forward button; only shown when a next page exists
	plg::string MenuButtonImageSelect{"e"};					// Same, for the select button
	plg::string MenuButtonImageExit{"f"};					// Same, for the exit/back button
	plg::string MenuButtonImageEmpty{"empty"};				// Filler image spanning a full prev+next slot pair, shown when neither applies but the exit image is
	plg::string MenuButtonImageEmptyHalf{"empty_half"};		// Filler image spanning a single prev/next slot, shown when only one of the two applies
	plg::string MenuSoundScroll;							// Sound event played to the client when the WASD button-menu cursor moves; empty plays nothing
	plg::string MenuSoundClick;								// Sound event played when an item is successfully selected
	plg::string MenuSoundBack;								// Sound event played when the "back" option (ExitBack) is chosen
	plg::string MenuSoundExit;								// Sound event played when the "exit" option is chosen
	plg::string MenuSoundDisabled;							// Sound event played when the select button is pressed on a non-selectable item

private:
	plg::vector<plg::string> m_paths;
};
