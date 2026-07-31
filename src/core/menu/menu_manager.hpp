#pragma once

/**
 * Action passed to a menu's handler callback.
 */
enum class MenuAction : uint8_t {
	Start = 0,  // A display session has started for a client (param unused).
	Select = 1, // The client selected an item (param = absolute item index).
	Cancel = 2, // The display session ended without a final selection (param = MenuCancelReason).
	End = 3,    // The display session is fully closed; always sent last (param unused).
};

/**
 * Reason a menu display session was cancelled, passed as `param` on MenuAction::Cancel.
 */
enum class MenuCancelReason : uint8_t {
	Exit = 0,        // The client pressed the exit button.
	Timeout = 1,     // The display time expired.
	Disconnect = 2,  // The client disconnected while the menu was open.
	Interrupted = 3, // Another DisplayMenu call replaced this display for the client.
	Destroyed = 4,   // The menu handle was destroyed while being displayed.
};

/**
 * Draw style for an individual menu item.
 */
enum class MenuItemStyle : uint8_t {
	Default = 0,  // Shown, numbered, selectable.
	Disabled = 1, // Shown, numbered, not selectable.
	Spacer = 2,   // Blank line, not numbered, not selectable.
};

using MenuHandlerCallback = void (*)(uint64 menuHandle, MenuAction action, int playerSlot, int param);

// Renders the menu's current state (title/items/page) to the client. Backends read state via the getters below.
using MenuDisplayCallback = void (*)(uint64 menuHandle, int playerSlot);
// Hides/cleans up whatever UI the backend showed to the client.
using MenuCloseCallback = void (*)(uint64 menuHandle, int playerSlot);
// Optional: called every server frame while the client has a menu of this type open (e.g. for input polling).
using MenuFrameCallback = void (*)(uint64 menuHandle, int playerSlot);

struct MenuItemData {
	plg::string info;
	plg::string display;
	MenuItemStyle style{MenuItemStyle::Default};
};

struct MenuData {
	plg::string title;
	plg::string typeName;
	std::vector<MenuItemData> items;
	MenuHandlerCallback handler{};
	int itemsPerPage{7};
	bool exitButton{true};
	bool closeOnSelect{true};
};

struct MenuTypeCallbacks {
	MenuDisplayCallback display{};
	MenuCloseCallback close{};
	MenuFrameCallback frame{};
};

struct ClientMenuState {
	uint64 menuHandle{};
	int currentOffset{};
	int page{};
	int cursor{}; // absolute item index highlighted by cursor-driven backends (e.g. ButtonMenu)
	int menuTime{}; // the `time` value passed to DisplayMenu; 0 = no timeout
	std::vector<int> prevOffsets;
	uint32_t timerId{};
};

class MenuManager {
	MenuManager() = default;
	~MenuManager() = default;
	NONCOPYABLE(MenuManager)

	static MenuManager instance;
public:
	static auto& Instance() noexcept {
		return instance;
	}
	static void Init();

	void OnClientDisconnect(CPlayerSlot slot);
	void OnGameFrame();

	// Menu type registry -------------------------------------------------
	bool RegisterMenuType(std::string_view name, MenuDisplayCallback display, MenuCloseCallback close, MenuFrameCallback frame = nullptr);
	bool UnregisterMenuType(std::string_view name);
	bool IsMenuTypeRegistered(std::string_view name) const;
	plg::vector<plg::string> GetMenuTypes() const;
	bool SetDefaultMenuType(std::string_view name);
	plg::string GetDefaultMenuType() const;

	// Lifecycle ------------------------------------------------------------
	uint64 CreateMenu(std::string_view title, MenuHandlerCallback handler, std::string_view menuType = {});
	bool DestroyMenu(uint64 menuHandle);
	bool IsValidMenu(uint64 menuHandle) const;

	// Properties -------------------------------------------------------------
	bool SetMenuTitle(uint64 menuHandle, std::string_view title);
	plg::string GetMenuTitle(uint64 menuHandle) const;
	bool SetMenuType(uint64 menuHandle, std::string_view typeName);
	plg::string GetMenuType(uint64 menuHandle) const;
	bool SetMenuPagination(uint64 menuHandle, int itemsPerPage); // 0 = no pagination
	int GetMenuPagination(uint64 menuHandle) const;
	bool SetMenuExitButton(uint64 menuHandle, bool enabled);
	bool GetMenuExitButton(uint64 menuHandle) const;
	bool SetMenuCloseOnSelect(uint64 menuHandle, bool enabled);
	bool GetMenuCloseOnSelect(uint64 menuHandle) const;

	// Items --------------------------------------------------------------
	int AddMenuItem(uint64 menuHandle, std::string_view info, std::string_view display, MenuItemStyle style = MenuItemStyle::Default);
	int InsertMenuItem(uint64 menuHandle, int index, std::string_view info, std::string_view display, MenuItemStyle style = MenuItemStyle::Default);
	bool RemoveMenuItem(uint64 menuHandle, int index);
	bool RemoveAllMenuItems(uint64 menuHandle);
	int GetMenuItemCount(uint64 menuHandle) const;
	plg::string GetMenuItemInfo(uint64 menuHandle, int index) const;
	plg::string GetMenuItemDisplay(uint64 menuHandle, int index) const;
	MenuItemStyle GetMenuItemStyle(uint64 menuHandle, int index) const;
	bool IsMenuItemSelectable(uint64 menuHandle, int index) const;
	bool SetMenuItemDisplay(uint64 menuHandle, int index, std::string_view display);
	bool SetMenuItemStyle(uint64 menuHandle, int index, MenuItemStyle style);

	// Display / navigation ------------------------------------------------
	bool DisplayMenu(uint64 menuHandle, int playerSlot, int time = 0);
	bool DisplayMenuAtItem(uint64 menuHandle, int playerSlot, int firstItem, int time = 0);
	bool CancelClientMenu(int playerSlot, MenuCancelReason reason = MenuCancelReason::Exit);
	uint64 GetClientMenu(int playerSlot) const;
	int GetClientMenuOffset(int playerSlot) const;
	int GetClientMenuTime(int playerSlot) const;
	int GetClientMenuCursor(int playerSlot) const;
	void SetClientMenuCursor(int playerSlot, int index);
	bool ClientMenuHasPrevPage(int playerSlot) const;
	bool ClientMenuHasNextPage(int playerSlot) const;

	bool MenuNextPage(int playerSlot);
	bool MenuPrevPage(int playerSlot);
	// Called by menu-type backends once they've resolved raw input into an absolute item index.
	bool SelectMenuItem(int playerSlot, int itemIndex);
	// Shared input path for digit-driven backends (chat/console/centerhtml): 1-7 = item, 8 = prev, 9 = next, 0 = exit.
	bool HandleDigitInput(int playerSlot, int digit);

private:
	std::shared_ptr<MenuData> Find(uint64 menuHandle) const;
	const MenuTypeCallbacks* FindType(const MenuData& menu) const;
	void RedisplayClient(int playerSlot);
	void CloseClientDisplay(int playerSlot, bool notifyBackend);
	void EndClientMenu(int playerSlot, MenuCancelReason reason);
	static void OnMenuTimeout(uint32_t timerId, const plg::vector<plg::any>& userData);

private:
	plg::flat_hash_map<uint64, std::shared_ptr<MenuData>> m_menus;
	plg::flat_hash_map<plg::string, MenuTypeCallbacks, plg::case_insensitive_hash, plg::case_insensitive_equal> m_menuTypes;
	plg::string m_defaultMenuType{"button"};
	std::array<ClientMenuState, MaxPlayers + 1> m_clientState{};
	uint64 m_nextHandle{1};
	mutable std::recursive_mutex m_mutex;
};
inline MenuManager& g_MenuManager = MenuManager::Instance();
