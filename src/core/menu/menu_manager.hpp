#pragma once

/**
 * Action passed to a menu's handler callback.
 */
enum class MenuAction {
	Start = 0,  // A display session has started for a client (param unused).
	Select = 1, // The client selected an item (param = absolute item index).
	Cancel = 2, // The display session ended without a final selection (param = MenuCancelReason).
	End = 3,    // The display session is fully closed; always sent last (param unused).
};

/**
 * Reason a menu display session was cancelled, passed as `param` on MenuAction::Cancel.
 */
enum class MenuCancelReason {
	Exit = 0,        // The client pressed the exit button.
	Timeout = 1,     // The display time expired.
	Disconnect = 2,  // The client disconnected while the menu was open.
	Interrupted = 3, // Another DisplayMenu call replaced this display for the client.
	Destroyed = 4,   // The menu handle was destroyed while being displayed.
	ExitBack = 5,    // The client pressed the back button (see SetMenuExitBackButton); handler is expected to redisplay the parent menu.
};

/**
 * Draw style for an individual menu item.
 */
enum class MenuItemStyle {
	Default = 0,  // Shown, numbered, selectable.
	Disabled = 1, // Shown, numbered, not selectable.
	Spacer = 2,   // Blank line, not numbered, not selectable.
};

using MenuId = uint32_t;

using MenuHandlerCallback = void (*)(MenuId id, MenuAction action, int playerSlot, int param);

// Renders the menu's current state (title/items/page) to the client. Backends read state via the getters below.
using MenuDisplayCallback = void (*)(MenuId id, int playerSlot);
// Hides/cleans up whatever UI the backend showed to the client.
using MenuCloseCallback = void (*)(MenuId id, int playerSlot);
// Optional: called every server frame while the client has a menu of this type open (e.g. for input polling).
using MenuFrameCallback = void (*)(MenuId id, int playerSlot);

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
	bool backButton{false}; // when set, takes over the exit slot: shown as "Back" and reports MenuCancelReason::ExitBack instead of Exit
	bool closeOnSelect{true};
};

struct MenuTypeCallbacks {
	MenuDisplayCallback display{};
	MenuCloseCallback close{};
	MenuFrameCallback frame{};
};

using TimerId = uint32_t;

struct ClientMenuState {
	MenuId id{};
	int currentOffset{};
	int page{};
	int cursor{}; // absolute item index highlighted by cursor-driven backends (e.g. ButtonMenu)
	double menuTime{}; // the `time` value passed to DisplayMenu; 0 = no timeout
	std::vector<int> prevOffsets;
	TimerId timerId{};
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
	MenuId CreateMenu(std::string_view title, MenuHandlerCallback handler, std::string_view menuType = {});
	bool DestroyMenu(MenuId id);
	bool IsValidMenu(MenuId id) const;

	// Properties -------------------------------------------------------------
	bool SetMenuTitle(MenuId id, std::string_view title);
	plg::string GetMenuTitle(MenuId id) const;
	bool SetMenuType(MenuId id, std::string_view typeName);
	plg::string GetMenuType(MenuId id) const;
	bool SetMenuPagination(MenuId id, int itemsPerPage); // 0 = no pagination
	int GetMenuPagination(MenuId id) const;
	bool SetMenuExitButton(MenuId id, bool enabled);
	bool GetMenuExitButton(MenuId id) const;
	bool SetMenuExitBackButton(MenuId id, bool enabled);
	bool GetMenuExitBackButton(MenuId id) const;
	bool SetMenuCloseOnSelect(MenuId id, bool enabled);
	bool GetMenuCloseOnSelect(MenuId id) const;

	// Items --------------------------------------------------------------
	int AddMenuItem(MenuId id, std::string_view info, std::string_view display, MenuItemStyle style = MenuItemStyle::Default);
	int InsertMenuItem(MenuId id, int index, std::string_view info, std::string_view display, MenuItemStyle style = MenuItemStyle::Default);
	bool RemoveMenuItem(MenuId id, int index);
	bool RemoveAllMenuItems(MenuId id);
	int GetMenuItemCount(MenuId id) const;
	plg::string GetMenuItemInfo(MenuId id, int index) const;
	plg::string GetMenuItemDisplay(MenuId id, int index) const;
	MenuItemStyle GetMenuItemStyle(MenuId id, int index) const;
	bool IsMenuItemSelectable(MenuId id, int index) const;
	bool SetMenuItemDisplay(MenuId id, int index, std::string_view display);
	bool SetMenuItemStyle(MenuId id, int index, MenuItemStyle style);

	// Display / navigation ------------------------------------------------
	bool DisplayMenu(MenuId id, int playerSlot, double time = 0.0);
	bool DisplayMenuAtItem(MenuId id, int playerSlot, int firstItem, double time = 0.0);
	bool CancelClientMenu(int playerSlot, MenuCancelReason reason = MenuCancelReason::Exit);
	MenuId GetClientMenu(int playerSlot) const;
	int GetClientMenuOffset(int playerSlot) const;
	double GetClientMenuTime(int playerSlot) const;
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
	std::shared_ptr<MenuData> FindMenu(MenuId id) const;
	std::shared_ptr<MenuTypeCallbacks> FindType(const MenuData& menu) const;
	void RedisplayClient(int playerSlot);
	void CloseClientDisplay(int playerSlot, bool notifyBackend);
	void EndClientMenu(int playerSlot, MenuCancelReason reason);
	static void OnMenuTimeout(TimerId timerId, const plg::vector<plg::any>& userData);

private:
	plg::flat_hash_map<MenuId, std::shared_ptr<MenuData>> m_menuInsts;
	plg::flat_hash_map<plg::string, std::shared_ptr<MenuTypeCallbacks>, plg::case_insensitive_hash, plg::case_insensitive_equal> m_menuTypes;
	plg::string m_defaultMenuType{"button"};
	std::array<ClientMenuState, MaxPlayers + 1> m_clientState{};
	MenuId m_nextId{};
	mutable std::recursive_mutex m_mutex;
};
inline MenuManager& g_MenuManager = MenuManager::Instance();
