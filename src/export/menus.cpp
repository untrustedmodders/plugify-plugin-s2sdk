#include <core/menu/menu_manager.hpp>

PLUGIFY_WARN_PUSH()

PLUGIFY_LINKAGE()

/**
 * @brief Registers a new menu type backend (e.g. a custom rendering style), so it can be
 * selected via CreateMenu/SetMenuType. Built-in types "chat", "console", "centerhtml" and
 * "button" are registered automatically at startup.
 *
 * @param name The unique, case-insensitive name of the menu type.
 * @param display Callback invoked to render the menu's current state to a client.
 * @param close Callback invoked to hide/clean up whatever UI was shown to a client.
 * @param frame Optional callback invoked every server frame while a client has this menu type open (e.g. for input polling). Pass null if not needed.
 * @return True if the type was registered; false if the name is empty, a callback is null, or the name is already taken.
 */
extern "C" PLUGIN_API bool RegisterMenuType(const plg::string& name, MenuDisplayCallback display, MenuCloseCallback close, MenuFrameCallback frame) {
	return g_MenuManager.RegisterMenuType(name, display, close, frame);
}

/**
 * @brief Unregisters a previously registered menu type.
 *
 * @param name The name of the menu type to remove.
 * @return True if a type with that name was found and removed.
 */
extern "C" PLUGIN_API bool UnregisterMenuType(const plg::string& name) {
	return g_MenuManager.UnregisterMenuType(name);
}

/**
 * @brief Checks whether a menu type with the given name is currently registered.
 *
 * @param name The name of the menu type.
 * @return True if the menu type is registered.
 */
extern "C" PLUGIN_API bool IsMenuTypeRegistered(const plg::string& name) {
	return g_MenuManager.IsMenuTypeRegistered(name);
}

/**
 * @brief Returns the names of all currently registered menu types.
 *
 * @return The vector of menu type names.
 */
extern "C" PLUGIN_API plg::vector<plg::string> GetMenuTypes() {
	return g_MenuManager.GetMenuTypes();
}

/**
 * @brief Sets the menu type used by menus that don't specify one explicitly.
 *
 * @param name The name of an already-registered menu type.
 * @return True if the type exists and was set as default.
 */
extern "C" PLUGIN_API bool SetDefaultMenuType(const plg::string& name) {
	return g_MenuManager.SetDefaultMenuType(name);
}

/**
 * @brief Returns the current default menu type name.
 *
 * @return The default menu type name.
 */
extern "C" PLUGIN_API plg::string GetDefaultMenuType() {
	return g_MenuManager.GetDefaultMenuType();
}

/**
 * @brief Creates a new menu.
 *
 * @param title The title shown at the top of the menu.
 * @param handler Callback invoked with Start/Select/Cancel/End actions as the menu is displayed and interacted with.
 * @param menuType The name of the menu type backend to render with. Empty uses the current default menu type.
 * @return A handle to the created menu.
 */
extern "C" PLUGIN_API MenuId CreateMenu(const plg::string& title, MenuHandlerCallback handler, const plg::string& menuType) {
	return g_MenuManager.CreateMenu(title, handler, menuType);
}

/**
 * @brief Destroys a menu. Any client currently viewing it is cancelled first (MenuCancelReason::Destroyed).
 *
 * @param id The handle to the menu.
 * @return True if the menu existed and was destroyed.
 */
extern "C" PLUGIN_API bool DestroyMenu(MenuId id) {
	return g_MenuManager.DestroyMenu(id);
}

/**
 * @brief Checks whether a menu handle refers to an existing menu.
 *
 * @param id The handle to the menu.
 * @return True if the handle is valid.
 */
extern "C" PLUGIN_API bool IsValidMenu(MenuId id) {
	return g_MenuManager.IsValidMenu(id);
}

/**
 * @brief Sets a menu's title.
 *
 * @param id The handle to the menu.
 * @param title The new title.
 * @return True if the menu exists.
 */
extern "C" PLUGIN_API bool SetMenuTitle(MenuId id, const plg::string& title) {
	return g_MenuManager.SetMenuTitle(id, title);
}

/**
 * @brief Gets a menu's title.
 *
 * @param id The handle to the menu.
 * @return The menu's title, or an empty string if the handle is invalid.
 */
extern "C" PLUGIN_API plg::string GetMenuTitle(MenuId id) {
	return g_MenuManager.GetMenuTitle(id);
}

/**
 * @brief Sets which registered menu type backend renders this menu.
 *
 * @param id The handle to the menu.
 * @param typeName The name of a registered menu type, or empty to use the default menu type.
 * @return True if the menu exists.
 */
extern "C" PLUGIN_API bool SetMenuType(MenuId id, const plg::string& typeName) {
	return g_MenuManager.SetMenuType(id, typeName);
}

/**
 * @brief Gets the menu type backend name assigned to this menu.
 *
 * @param id The handle to the menu.
 * @return The menu type name (may be empty, meaning "use the default").
 */
extern "C" PLUGIN_API plg::string GetMenuType(MenuId id) {
	return g_MenuManager.GetMenuType(id);
}

/**
 * @brief Sets how many items are shown per page.
 *
 * @param id The handle to the menu.
 * @param itemsPerPage The number of items per page, or 0 to disable pagination (show every item on one page).
 * @return True if the menu exists and itemsPerPage is not negative.
 */
extern "C" PLUGIN_API bool SetMenuPagination(MenuId id, int itemsPerPage) {
	return g_MenuManager.SetMenuPagination(id, itemsPerPage);
}

/**
 * @brief Gets how many items are shown per page.
 *
 * @param id The handle to the menu.
 * @return The items-per-page value, 0 meaning pagination is disabled.
 */
extern "C" PLUGIN_API int GetMenuPagination(MenuId id) {
	return g_MenuManager.GetMenuPagination(id);
}

/**
 * @brief Sets whether the menu shows an exit option.
 *
 * @param id The handle to the menu.
 * @param enabled True to show an exit option.
 * @return True if the menu exists.
 */
extern "C" PLUGIN_API bool SetMenuExitButton(MenuId id, bool enabled) {
	return g_MenuManager.SetMenuExitButton(id, enabled);
}

/**
 * @brief Gets whether the menu shows an exit option.
 *
 * @param id The handle to the menu.
 * @return True if the exit option is enabled.
 */
extern "C" PLUGIN_API bool GetMenuExitButton(MenuId id) {
	return g_MenuManager.GetMenuExitButton(id);
}

/**
 * @brief Sets whether the menu shows a "back" option in place of the exit option. Selecting
 * it cancels the display with MenuCancelReason::ExitBack instead of MenuCancelReason::Exit,
 * which a handler can use to redisplay a parent menu (SourceMod-style ExitBack).
 *
 * @param id The handle to the menu.
 * @param enabled True to show a back option instead of the exit option.
 * @return True if the menu exists.
 */
extern "C" PLUGIN_API bool SetMenuExitBackButton(MenuId id, bool enabled) {
	return g_MenuManager.SetMenuExitBackButton(id, enabled);
}

/**
 * @brief Gets whether the menu shows a "back" option in place of the exit option.
 *
 * @param id The handle to the menu.
 * @return True if the back option is enabled.
 */
extern "C" PLUGIN_API bool GetMenuExitBackButton(MenuId id) {
	return g_MenuManager.GetMenuExitBackButton(id);
}

/**
 * @brief Sets whether selecting an item automatically closes the menu display for that client.
 * When disabled, the display stays open after MenuAction::Select and the handler is responsible
 * for closing/redisplaying it if desired.
 *
 * @param id The handle to the menu.
 * @param enabled True to auto-close on selection (the default).
 * @return True if the menu exists.
 */
extern "C" PLUGIN_API bool SetMenuCloseOnSelect(MenuId id, bool enabled) {
	return g_MenuManager.SetMenuCloseOnSelect(id, enabled);
}

/**
 * @brief Gets whether selecting an item automatically closes the menu display for that client.
 *
 * @param id The handle to the menu.
 * @return True if close-on-select is enabled.
 */
extern "C" PLUGIN_API bool GetMenuCloseOnSelect(MenuId id) {
	return g_MenuManager.GetMenuCloseOnSelect(id);
}

/**
 * @brief Appends an item to the end of a menu.
 *
 * @param id The handle to the menu.
 * @param info An internal identifier for the item, not shown to the client; retrieve it with GetMenuItemInfo from within the handler callback.
 * @param display The text shown to the client.
 * @param style The item's draw style (Default/Disabled/Spacer).
 * @return The index of the newly added item, or -1 if the menu handle is invalid.
 */
extern "C" PLUGIN_API int AddMenuItem(MenuId id, const plg::string& info, const plg::string& display, MenuItemStyle style) {
	return g_MenuManager.AddMenuItem(id, info, display, style);
}

/**
 * @brief Inserts an item into a menu at a specific index.
 *
 * @param id The handle to the menu.
 * @param index The index to insert at; must be within [0, item count].
 * @param info An internal identifier for the item, not shown to the client.
 * @param display The text shown to the client.
 * @param style The item's draw style (Default/Disabled/Spacer).
 * @return The index the item was inserted at, or -1 on failure.
 */
extern "C" PLUGIN_API int InsertMenuItemAt(MenuId id, int index, const plg::string& info, const plg::string& display, MenuItemStyle style) {
	return g_MenuManager.InsertMenuItem(id, index, info, display, style);
}

/**
 * @brief Removes an item from a menu.
 *
 * @param id The handle to the menu.
 * @param index The index of the item to remove.
 * @return True if the item existed and was removed.
 */
extern "C" PLUGIN_API bool RemoveMenuItem(MenuId id, int index) {
	return g_MenuManager.RemoveMenuItem(id, index);
}

/**
 * @brief Removes every item from a menu.
 *
 * @param id The handle to the menu.
 * @return True if the menu exists.
 */
extern "C" PLUGIN_API bool RemoveAllMenuItems(MenuId id) {
	return g_MenuManager.RemoveAllMenuItems(id);
}

/**
 * @brief Gets the number of items in a menu.
 *
 * @param id The handle to the menu.
 * @return The item count, or 0 if the handle is invalid.
 */
extern "C" PLUGIN_API int GetMenuItemsCount(MenuId id) {
	return g_MenuManager.GetMenuItemCount(id);
}

/**
 * @brief Gets an item's internal info string.
 *
 * @param id The handle to the menu.
 * @param index The index of the item.
 * @return The item's info string, or empty if out of range.
 */
extern "C" PLUGIN_API plg::string GetMenuItemInfoText(MenuId id, int index) {
	return g_MenuManager.GetMenuItemInfo(id, index);
}

/**
 * @brief Gets an item's display text.
 *
 * @param id The handle to the menu.
 * @param index The index of the item.
 * @return The item's display text, or empty if out of range.
 */
extern "C" PLUGIN_API plg::string GetMenuItemDisplay(MenuId id, int index) {
	return g_MenuManager.GetMenuItemDisplay(id, index);
}

/**
 * @brief Gets an item's draw style.
 *
 * @param id The handle to the menu.
 * @param index The index of the item.
 * @return The item's style; MenuItemStyle::Disabled if out of range.
 */
extern "C" PLUGIN_API MenuItemStyle GetMenuItemStyle(MenuId id, int index) {
	return g_MenuManager.GetMenuItemStyle(id, index);
}

/**
 * @brief Checks whether an item can currently be selected (style is Default).
 *
 * @param id The handle to the menu.
 * @param index The index of the item.
 * @return True if the item is selectable.
 */
extern "C" PLUGIN_API bool IsMenuItemSelectable(MenuId id, int index) {
	return g_MenuManager.IsMenuItemSelectable(id, index);
}

/**
 * @brief Changes an item's display text.
 *
 * @param id The handle to the menu.
 * @param index The index of the item.
 * @param display The new display text.
 * @return True if the item exists.
 */
extern "C" PLUGIN_API bool SetMenuItemDisplay(MenuId id, int index, const plg::string& display) {
	return g_MenuManager.SetMenuItemDisplay(id, index, display);
}

/**
 * @brief Changes an item's draw style.
 *
 * @param id The handle to the menu.
 * @param index The index of the item.
 * @param style The new style.
 * @return True if the item exists.
 */
extern "C" PLUGIN_API bool SetMenuItemStyle(MenuId id, int index, MenuItemStyle style) {
	return g_MenuManager.SetMenuItemStyle(id, index, style);
}

/**
 * @brief Displays a menu to a client, starting at the first item. Replaces whatever menu the client currently has open, if any.
 *
 * @param id The handle to the menu.
 * @param playerSlot The client's player slot.
 * @param time How long, in seconds, before the menu auto-closes (MenuCancelReason::Timeout). 0 or negative means no timeout.
 * @return True if the menu was displayed.
 */
extern "C" PLUGIN_API bool DisplayMenu(MenuId id, int playerSlot, int time) {
	return g_MenuManager.DisplayMenu(id, playerSlot, time);
}

/**
 * @brief Displays a menu to a client, starting at a specific item.
 *
 * @param id The handle to the menu.
 * @param playerSlot The client's player slot.
 * @param firstItem The index of the first item to show.
 * @param time How long, in seconds, before the menu auto-closes. 0 or negative means no timeout.
 * @return True if the menu was displayed.
 */
extern "C" PLUGIN_API bool DisplayMenuAtItem(MenuId id, int playerSlot, int firstItem, int time) {
	return g_MenuManager.DisplayMenuAtItem(id, playerSlot, firstItem, time);
}

/**
 * @brief Cancels whatever menu a client currently has open.
 *
 * @param playerSlot The client's player slot.
 * @param reason The reason reported to the menu's handler via MenuAction::Cancel.
 * @return True if the client had a menu open and it was cancelled.
 */
extern "C" PLUGIN_API bool CancelClientMenu(int playerSlot, MenuCancelReason reason) {
	return g_MenuManager.CancelClientMenu(playerSlot, reason);
}

/**
 * @brief Gets the menu a client currently has open.
 *
 * @param playerSlot The client's player slot.
 * @return The open menu's handle, or 0 if none.
 */
extern "C" PLUGIN_API MenuId GetClientMenu(int playerSlot) {
	return g_MenuManager.GetClientMenu(playerSlot);
}

/**
 * @brief Gets the index of the first item shown on the client's current page.
 *
 * @param playerSlot The client's player slot.
 * @return The current page offset.
 */
extern "C" PLUGIN_API int GetClientMenuOffset(int playerSlot) {
	return g_MenuManager.GetClientMenuOffset(playerSlot);
}

/**
 * @brief Gets the `time` value the client's current menu was displayed with.
 *
 * @param playerSlot The client's player slot.
 * @return The display time in seconds, 0 meaning no timeout.
 */
extern "C" PLUGIN_API double GetClientMenuTime(int playerSlot) {
	return g_MenuManager.GetClientMenuTime(playerSlot);
}

/**
 * @brief Gets the absolute item index highlighted by cursor-driven menu types (e.g. the button menu).
 *
 * @param playerSlot The client's player slot.
 * @return The cursor's item index.
 */
extern "C" PLUGIN_API int GetClientMenuCursor(int playerSlot) {
	return g_MenuManager.GetClientMenuCursor(playerSlot);
}

/**
 * @brief Sets the absolute item index highlighted by cursor-driven menu types.
 *
 * @param playerSlot The client's player slot.
 * @param index The item index to highlight.
 */
extern "C" PLUGIN_API void SetClientMenuCursor(int playerSlot, int index) {
	g_MenuManager.SetClientMenuCursor(playerSlot, index);
}

/**
 * @brief Checks whether the client's current menu display has a previous page to go back to.
 *
 * @param playerSlot The client's player slot.
 * @return True if a previous page exists.
 */
extern "C" PLUGIN_API bool ClientMenuHasPrevPage(int playerSlot) {
	return g_MenuManager.ClientMenuHasPrevPage(playerSlot);
}

/**
 * @brief Checks whether the client's current menu display has a next page.
 *
 * @param playerSlot The client's player slot.
 * @return True if a next page exists.
 */
extern "C" PLUGIN_API bool ClientMenuHasNextPage(int playerSlot) {
	return g_MenuManager.ClientMenuHasNextPage(playerSlot);
}

/**
 * @brief Advances the client's current menu display to the next page and redraws it.
 *
 * @param playerSlot The client's player slot.
 * @return True if there was a next page to move to.
 */
extern "C" PLUGIN_API bool MenuNextPage(int playerSlot) {
	return g_MenuManager.MenuNextPage(playerSlot);
}

/**
 * @brief Moves the client's current menu display back to the previous page and redraws it.
 *
 * @param playerSlot The client's player slot.
 * @return True if there was a previous page to move to.
 */
extern "C" PLUGIN_API bool MenuPrevPage(int playerSlot) {
	return g_MenuManager.MenuPrevPage(playerSlot);
}

/**
 * @brief Selects an item on the client's current menu display by its absolute index.
 * Intended to be called by menu type backends once they've resolved raw input into an item index.
 *
 * @param playerSlot The client's player slot.
 * @param itemIndex The absolute index of the item to select.
 * @return True if the item existed and was selectable.
 */
extern "C" PLUGIN_API bool SelectMenuItem(int playerSlot, int itemIndex) {
	return g_MenuManager.SelectMenuItem(playerSlot, itemIndex);
}

/**
 * @brief Shared input path for digit-driven menu types (chat/console/centerhtml): 1-7 select an item
 * on the current page, 8 goes to the previous page, 9 to the next page, 0 exits.
 *
 * @param playerSlot The client's player slot.
 * @param digit The digit (0-9) that was pressed.
 * @return True if the digit resulted in an action.
 */
extern "C" PLUGIN_API bool HandleDigitInput(int playerSlot, int digit) {
	return g_MenuManager.HandleDigitInput(playerSlot, digit);
}

PLUGIFY_WARN_POP()
