#include "menu_manager.hpp"

#include <core/con_command_manager.hpp>
#include <core/core_config.hpp>
#include <core/sdk/utils.hpp>
#include <core/timer_system.hpp>

void RegisterBuiltinChatMenuType();
void RegisterBuiltinConsoleMenuType();
void RegisterBuiltinCenterHtmlMenuType();
void RegisterBuiltinButtonMenuType();

MenuManager MenuManager::instance;

namespace {
	// Shared by every registered <prefix>0.."<prefix>9 command (e.g. "css_3", "sm_3").
	ResultType OnMenuDigitCommand(int caller, ConCommandContext, const plg::vector<plg::string>& arguments) {
		if (arguments.empty()) {
			return ResultType::Continue;
		}

		std::string_view command = arguments[0];
		for (const plg::string& prefix : g_pCoreConfig->MenuCommandPrefixes) {
			std::string_view prefixView = prefix;
			if (command.size() != prefixView.size() + 1 || !command.starts_with(prefixView)) {
				continue;
			}

			char digitChar = command.back();
			if (digitChar < '0' || digitChar > '9') {
				return ResultType::Continue;
			}

			if (g_MenuManager.HandleDigitInput(caller, digitChar - '0')) {
				return ResultType::Handled;
			}

			return ResultType::Continue;
		}

		return ResultType::Continue;
	}

	// SourceMod-style single command: "menuselect <0-9>".
	ResultType OnMenuSelectCommand(int caller, ConCommandContext, const plg::vector<plg::string>& arguments) {
		if (arguments.size() < 2) {
			return ResultType::Continue;
		}

		auto digit = plg::cast_to<int>(arguments[1]);
		if (!digit || *digit < 0 || *digit > 9) {
			return ResultType::Continue;
		}

		if (g_MenuManager.HandleDigitInput(caller, *digit)) {
			return ResultType::Handled;
		}

		return ResultType::Continue;
	}
#if 0
	// Sample "test" command: demonstrates MenuManager's public API with a menu
	// whose items include one that opens a nested sub-menu, and shows how the
	// sub-menu's "back" button (MenuCancelReason::ExitBack) returns to it.
	void OnTestMenuAction(MenuId id, MenuAction action, CPlayerSlot slot, int param);

	MenuId CreateTestMainMenu() {
		MenuId menu = g_MenuManager.CreateMenu("Test Menu", &OnTestMenuAction);
		g_MenuManager.AddMenuItem(menu, "hello", "Say Hello");
		g_MenuManager.AddMenuItem(menu, "disabled", "Disabled Item", MenuItemStyle::Disabled);
		g_MenuManager.AddMenuItem(menu, "submenu", "Open Sub-Menu");
		g_MenuManager.AddMenuItem(menu, "disabled2", "Disabled Item2", MenuItemStyle::Disabled);
		g_MenuManager.AddMenuItem(menu, "disabled3", "Disabled Item3", MenuItemStyle::Disabled);
		g_MenuManager.AddMenuItem(menu, "disabled4", "Disabled Item4", MenuItemStyle::Disabled);
		g_MenuManager.AddMenuItem(menu, "disabled5", "Disabled Item5", MenuItemStyle::Disabled);
		g_MenuManager.AddMenuItem(menu, "disabled6", "Disabled Item6", MenuItemStyle::Disabled);
		g_MenuManager.AddMenuItem(menu, "disabled7", "Disabled Item7", MenuItemStyle::Disabled);
		g_MenuManager.AddMenuItem(menu, "disabled8", "Disabled Item8", MenuItemStyle::Disabled);
		g_MenuManager.AddMenuItem(menu, "submenu2", "Say Hello2");
		return menu;
	}

	void OnTestSubMenuAction(MenuId id, MenuAction action, CPlayerSlot slot, int param) {
		switch (action) {
			case MenuAction::Select:
				utils::PrintChat(slot, std::format("You picked '{}' from the sub-menu.", g_MenuManager.GetMenuItemInfo(id, param)));
				break;
			case MenuAction::Cancel:
				if (static_cast<MenuCancelReason>(param) == MenuCancelReason::ExitBack) {
					g_MenuManager.DisplayMenu(CreateTestMainMenu(), slot);
				}
				break;
			case MenuAction::End:
				g_MenuManager.DestroyMenu(id);
				break;
			default:
				break;
		}
	}

	void OnTestMenuAction(MenuId id, MenuAction action, CPlayerSlot slot, int param) {
		switch (action) {
			case MenuAction::Select: {
				plg::string info = g_MenuManager.GetMenuItemInfo(id, param);
				if (info == "hello") {
					utils::PrintChat(slot, "Hello from the test menu!");
				} else if (info == "submenu") {
					MenuId sub = g_MenuManager.CreateMenu("Test Sub-Menu", &OnTestSubMenuAction);
					g_MenuManager.AddMenuItem(sub, "option_a", "Sub-option A");
					g_MenuManager.AddMenuItem(sub, "option_b", "Sub-option B");
					g_MenuManager.SetMenuExitBackButton(sub, true);
					g_MenuManager.DisplayMenu(sub, slot);
				}
				break;
			}
			case MenuAction::End:
				g_MenuManager.DestroyMenu(id);
				break;
			default:
				break;
		}
	}

	ResultType OnTestMenuCommand(int caller, ConCommandContext, const plg::vector<plg::string>&) {
		if (!utils::IsPlayerSlot(caller)) {
			plg::print(LS_WARNING, "The 'test' command can only be used by an in-game player\n");
			return ResultType::Handled;
		}

		g_MenuManager.DisplayMenu(CreateTestMainMenu(), caller);

		return ResultType::Handled;
	}
#endif
}// namespace

void MenuManager::Init() {
	if (!g_pCoreConfig->MenuEnabled) {
		return;
	}

	constexpr ConVarFlag kPlayerTriggerableFlags = ConVarFlag::LinkedConcommand | ConVarFlag::Release | ConVarFlag::ClientCanExecute;

	for (const plg::string& prefix : g_pCoreConfig->MenuCommandPrefixes) {
		for (int i = 0; i <= 9; ++i) {
			auto name = std::format("{}{}", prefix, i);
			g_ConCommandManager.AddValveCommand(name, "Menu key handler", kPlayerTriggerableFlags);
			g_ConCommandManager.AddCommandListener(name, &OnMenuDigitCommand, HookMode::Pre);
		}
	}

	for (const plg::string& name : g_pCoreConfig->MenuSelectCommands) {
		g_ConCommandManager.AddValveCommand(name, "Menu item select", kPlayerTriggerableFlags);
		g_ConCommandManager.AddCommandListener(name, &OnMenuSelectCommand, HookMode::Pre);
	}

	//g_ConCommandManager.AddValveCommand("test", "Opens a sample menu to check MenuManager", kPlayerTriggerableFlags);
	//g_ConCommandManager.AddCommandListener("test", &OnTestMenuCommand, HookMode::Pre);

	RegisterBuiltinChatMenuType();
	RegisterBuiltinConsoleMenuType();
	RegisterBuiltinCenterHtmlMenuType();
	RegisterBuiltinButtonMenuType();
}

void MenuManager::OnClientDisconnect(CPlayerSlot slot) {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(slot)) {
		return;
	}
	if (m_clientState[static_cast<size_t>(slot.Get())].id) {
		EndClientMenu(slot.Get(), MenuCancelReason::Disconnect);
	}
}

void MenuManager::OnGameFrame() {
	std::scoped_lock lock(m_mutex);
	for (int slot = 0; slot <= MaxPlayers; ++slot) {
		auto& state = m_clientState[static_cast<size_t>(slot)];
		if (!state.id) {
			continue;
		}

		auto menu = FindMenu(state.id);
		if (!menu) {
			continue;
		}

		if (auto type = FindType(*menu); type && type->frame) {
			type->frame(state.id, slot);
		}
	}
}

// -- Menu type registry -----------------------------------------------------

bool MenuManager::RegisterMenuType(std::string_view name, MenuDisplayCallback display, MenuCloseCallback close, MenuFrameCallback frame) {
	std::scoped_lock lock(m_mutex);

	if (name.empty() || !display || !close) {
		plg::print(LS_WARNING, "Cannot register menu type with empty name or null display/close callback\n");
		return false;
	}

	if (m_menuTypes.contains(name)) {
		plg::print(LS_WARNING, "Menu type '{}' is already registered\n", name);
		return false;
	}

	m_menuTypes.emplace(name, std::make_shared<MenuTypeCallbacks>(display, close, frame));
	return true;
}

bool MenuManager::UnregisterMenuType(std::string_view name) {
	std::scoped_lock lock(m_mutex);
	return m_menuTypes.erase(name) != 0;
}

bool MenuManager::IsMenuTypeRegistered(std::string_view name) const {
	std::scoped_lock lock(m_mutex);
	return m_menuTypes.contains(name);
}

plg::vector<plg::string> MenuManager::GetMenuTypes() const {
	std::scoped_lock lock(m_mutex);
	plg::vector<plg::string> names;
	names.reserve(m_menuTypes.size());
	for (const auto& [name, _] : m_menuTypes) {
		names.push_back(name);
	}
	return names;
}

bool MenuManager::SetDefaultMenuType(std::string_view name) {
	std::scoped_lock lock(m_mutex);
	if (!m_menuTypes.contains(name)) {
		plg::print(LS_WARNING, "Cannot set default menu type to unregistered type '{}'\n", name);
		return false;
	}
	m_defaultMenuType = name;
	return true;
}

plg::string MenuManager::GetDefaultMenuType() const {
	std::scoped_lock lock(m_mutex);
	return m_defaultMenuType;
}

// -- Lifecycle ----------------------------------------------------------------

MenuId MenuManager::CreateMenu(std::string_view title, MenuHandlerCallback handler, std::string_view menuType) {
	std::scoped_lock lock(m_mutex);

	auto menu = std::make_shared<MenuData>();
	menu->title = title;
	menu->typeName = menuType;
	menu->handler = handler;

	MenuId id = m_nextId++;
	m_menuInsts.emplace(id, std::move(menu));
	return id;
}

bool MenuManager::DestroyMenu(MenuId id) {
	std::scoped_lock lock(m_mutex);

	if (!m_menuInsts.contains(id)) {
		return false;
	}

	for (int slot = 0; slot <= MaxPlayers; ++slot) {
		if (m_clientState[static_cast<size_t>(slot)].id == id) {
			EndClientMenu(slot, MenuCancelReason::Destroyed);
		}
	}

	m_menuInsts.erase(id);
	return true;
}

bool MenuManager::IsValidMenu(MenuId id) const {
	std::scoped_lock lock(m_mutex);
	return m_menuInsts.contains(id);
}

// -- Properties -----------------------------------------------------------------

bool MenuManager::SetMenuTitle(MenuId id, std::string_view title) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu) {
		return false;
	}
	menu->title = title;
	return true;
}

plg::string MenuManager::GetMenuTitle(MenuId id) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	return menu ? menu->title : plg::string{};
}

bool MenuManager::SetMenuType(MenuId id, std::string_view typeName) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu) {
		return false;
	}
	menu->typeName = typeName;
	return true;
}

plg::string MenuManager::GetMenuType(MenuId id) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	return menu ? menu->typeName : plg::string{};
}

bool MenuManager::SetMenuPagination(MenuId id, int itemsPerPage) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu || itemsPerPage < 0) {
		return false;
	}
	menu->itemsPerPage = itemsPerPage;
	return true;
}

int MenuManager::GetMenuPagination(MenuId id) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	return menu ? menu->itemsPerPage : 0;
}

bool MenuManager::SetMenuExitButton(MenuId id, bool enabled) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu) {
		return false;
	}
	menu->exitButton = enabled;
	return true;
}

bool MenuManager::GetMenuExitButton(MenuId id) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	return menu && menu->exitButton;
}

bool MenuManager::SetMenuExitBackButton(MenuId id, bool enabled) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu) {
		return false;
	}
	menu->backButton = enabled;
	return true;
}

bool MenuManager::GetMenuExitBackButton(MenuId id) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	return menu && menu->backButton;
}

bool MenuManager::SetMenuCloseOnSelect(MenuId id, bool enabled) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu) {
		return false;
	}
	menu->closeOnSelect = enabled;
	return true;
}

bool MenuManager::GetMenuCloseOnSelect(MenuId id) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	return menu && menu->closeOnSelect;
}

// -- Items ------------------------------------------------------------------------

int MenuManager::AddMenuItem(MenuId id, std::string_view info, std::string_view display, MenuItemStyle style) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu) {
		return -1;
	}
	menu->items.push_back(MenuItemData{info, display, style});
	return static_cast<int>(menu->items.size()) - 1;
}

int MenuManager::InsertMenuItem(MenuId id, int index, std::string_view info, std::string_view display, MenuItemStyle style) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu || index < 0 || index > static_cast<int>(menu->items.size())) {
		return -1;
	}
	menu->items.insert(menu->items.begin() + index, MenuItemData{info, display, style});
	return index;
}

bool MenuManager::RemoveMenuItem(MenuId id, int index) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return false;
	}
	menu->items.erase(menu->items.begin() + index);
	return true;
}

bool MenuManager::RemoveAllMenuItems(MenuId id) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu) {
		return false;
	}
	menu->items.clear();
	return true;
}

int MenuManager::GetMenuItemCount(MenuId id) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	return menu ? static_cast<int>(menu->items.size()) : 0;
}

plg::string MenuManager::GetMenuItemInfo(MenuId id, int index) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return {};
	}
	return menu->items[static_cast<size_t>(index)].info;
}

plg::string MenuManager::GetMenuItemDisplay(MenuId id, int index) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return {};
	}
	return menu->items[static_cast<size_t>(index)].display;
}

MenuItemStyle MenuManager::GetMenuItemStyle(MenuId id, int index) const {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return MenuItemStyle::Disabled;
	}
	return menu->items[static_cast<size_t>(index)].style;
}

bool MenuManager::IsMenuItemSelectable(MenuId id, int index) const {
	return GetMenuItemStyle(id, index) == MenuItemStyle::Default;
}

bool MenuManager::SetMenuItemDisplay(MenuId id, int index, std::string_view display) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return false;
	}
	menu->items[static_cast<size_t>(index)].display = display;
	return true;
}

bool MenuManager::SetMenuItemStyle(MenuId id, int index, MenuItemStyle style) {
	std::scoped_lock lock(m_mutex);
	auto menu = FindMenu(id);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return false;
	}
	menu->items[static_cast<size_t>(index)].style = style;
	return true;
}

// -- Display / navigation --------------------------------------------------------

bool MenuManager::DisplayMenu(MenuId id, CPlayerSlot slot, double time) {
	return DisplayMenuAtItem(id, slot, 0, time);
}

bool MenuManager::DisplayMenuAtItem(MenuId id, CPlayerSlot slot, int firstItem, double time) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(slot)) {
		return false;
	}

	auto menu = FindMenu(id);
	if (!menu) {
		return false;
	}

	auto type = FindType(*menu);
	if (!type) {
		plg::print(LS_WARNING, "Unknown menu type '{}'\n", menu->typeName.empty() ? m_defaultMenuType : menu->typeName);
		return false;
	}

	if (m_clientState[static_cast<size_t>(slot)].id) {
		EndClientMenu(slot, MenuCancelReason::Interrupted);
	}

	auto& state = m_clientState[static_cast<size_t>(slot)];
	state = ClientMenuState{};
	state.id = id;
	state.currentOffset = menu->items.empty() ? 0 : std::clamp(firstItem, 0, static_cast<int>(menu->items.size()) - 1);
	state.cursor = state.currentOffset;
	state.menuTime = time > 0 ? time : 0;

	if (time > 0) {
		state.timerId = g_TimerSystem.CreateTimer(time, &MenuManager::OnMenuTimeout, TimerFlag::Default,
			{slot, id});
	}

	if (menu->handler) {
		menu->handler(id, MenuAction::Start, slot, 0);
	}

	type->display(id, slot);
	return true;
}

bool MenuManager::CancelClientMenu(CPlayerSlot slot, MenuCancelReason reason) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(slot)) {
		return false;
	}

	if (!m_clientState[static_cast<size_t>(slot)].id) {
		return false;
	}

	EndClientMenu(slot, reason);
	return true;
}

MenuId MenuManager::GetClientMenu(CPlayerSlot slot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(slot)) {
		return 0;
	}
	return m_clientState[static_cast<size_t>(slot)].id;
}

int MenuManager::GetClientMenuOffset(CPlayerSlot slot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(slot)) {
		return 0;
	}
	return m_clientState[static_cast<size_t>(slot)].currentOffset;
}

double MenuManager::GetClientMenuTime(CPlayerSlot slot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(slot)) {
		return 0;
	}
	return m_clientState[static_cast<size_t>(slot)].menuTime;
}

int MenuManager::GetClientMenuCursor(CPlayerSlot slot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(slot)) {
		return 0;
	}
	return m_clientState[static_cast<size_t>(slot)].cursor;
}

void MenuManager::SetClientMenuCursor(CPlayerSlot slot, int index) {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(slot)) {
		return;
	}
	m_clientState[static_cast<size_t>(slot)].cursor = index;
}

bool MenuManager::ClientMenuHasPrevPage(CPlayerSlot slot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(slot)) {
		return false;
	}
	return !m_clientState[static_cast<size_t>(slot)].prevOffsets.empty();
}

bool MenuManager::ClientMenuHasNextPage(CPlayerSlot slot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(slot)) {
		return false;
	}
	const auto& state = m_clientState[static_cast<size_t>(slot)];
	auto menu = FindMenu(state.id);
	if (!menu || menu->itemsPerPage <= 0) {
		return false;
	}
	return state.currentOffset + menu->itemsPerPage < static_cast<int>(menu->items.size());
}

bool MenuManager::MenuNextPage(CPlayerSlot slot) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(slot)) {
		return false;
	}

	auto& state = m_clientState[static_cast<size_t>(slot)];
	auto menu = FindMenu(state.id);
	if (!menu || menu->itemsPerPage <= 0) {
		return false;
	}

	int next = state.currentOffset + menu->itemsPerPage;
	if (next >= static_cast<int>(menu->items.size())) {
		return false;
	}

	state.prevOffsets.push_back(state.currentOffset);
	state.currentOffset = next;
	state.page++;
	RedisplayClient(slot);
	return true;
}

bool MenuManager::MenuPrevPage(CPlayerSlot slot) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(slot)) {
		return false;
	}

	auto& state = m_clientState[static_cast<size_t>(slot)];
	if (!state.id || state.prevOffsets.empty()) {
		return false;
	}

	state.currentOffset = state.prevOffsets.back();
	state.prevOffsets.pop_back();
	state.page--;
	RedisplayClient(slot);
	return true;
}

bool MenuManager::SelectMenuItem(CPlayerSlot slot, int itemIndex) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(slot)) {
		return false;
	}

	auto& state = m_clientState[static_cast<size_t>(slot)];
	MenuId id = state.id;
	auto menu = FindMenu(id);
	if (!menu) {
		return false;
	}

	if (itemIndex < 0 || itemIndex >= static_cast<int>(menu->items.size())) {
		return false;
	}

	if (menu->items[static_cast<size_t>(itemIndex)].style != MenuItemStyle::Default) {
		return false;
	}

	bool shouldClose = menu->closeOnSelect;
	if (shouldClose) {
		// Tear down the display first so a handler that opens a new menu from within
		// the Select callback starts from a clean slate.
		CloseClientDisplay(slot, true);
	}

	if (menu->handler) {
		menu->handler(id, MenuAction::Select, slot, itemIndex);
	}

	if (shouldClose && menu->handler) {
		menu->handler(id, MenuAction::End, slot, 0);
	}

	return true;
}

bool MenuManager::HandleDigitInput(CPlayerSlot slot, int digit) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(slot)) {
		return false;
	}

	const auto& state = m_clientState[static_cast<size_t>(slot)];
	if (!state.id) {
		return false;
	}

	auto menu = FindMenu(state.id);
	if (!menu) {
		return false;
	}

	switch (digit) {
		case 0:
			if (menu->backButton) {
				return CancelClientMenu(slot, MenuCancelReason::ExitBack);
			}
			if (!menu->exitButton) {
				return false;
			}
			return CancelClientMenu(slot, MenuCancelReason::Exit);
		case 8:
			return MenuPrevPage(slot);
		case 9:
			return MenuNextPage(slot);
		default:
			return SelectMenuItem(slot, state.currentOffset + digit - 1);
	}
}

// -- Internals ----------------------------------------------------------------------

std::shared_ptr<MenuData> MenuManager::FindMenu(MenuId id) const {
	auto it = m_menuInsts.find(id);
	return it != m_menuInsts.end() ? it->second : nullptr;
}

std::shared_ptr<MenuTypeCallbacks> MenuManager::FindType(const MenuData& menu) const {
	std::string_view typeName = menu.typeName.empty() ? m_defaultMenuType : menu.typeName;
	auto it = m_menuTypes.find(typeName);
	return it != m_menuTypes.end() ? it->second : nullptr;
}

void MenuManager::RedisplayClient(CPlayerSlot slot) {
	const auto& state = m_clientState[static_cast<size_t>(slot)];
	auto menu = FindMenu(state.id);
	if (!menu) {
		return;
	}
	if (auto type = FindType(*menu)) {
		type->display(state.id, slot);
	}
}

void MenuManager::CloseClientDisplay(CPlayerSlot slot, bool notifyBackend) {
	auto& state = m_clientState[static_cast<size_t>(slot)];

	if (state.timerId) {
		g_TimerSystem.KillTimer(state.timerId);
	}

	if (notifyBackend && state.id) {
		if (auto menu = FindMenu(state.id)) {
			if (auto type = FindType(*menu)) {
				type->close(state.id, slot);
			}
		}
	}

	state = ClientMenuState{};
}

void MenuManager::EndClientMenu(CPlayerSlot slot, MenuCancelReason reason) {
	auto& state = m_clientState[static_cast<size_t>(slot)];
	if (!state.id) {
		return;
	}

	MenuId id = state.id;
	auto menu = FindMenu(id);

	CloseClientDisplay(slot, reason != MenuCancelReason::Disconnect);

	if (menu && menu->handler) {
		menu->handler(id, MenuAction::Cancel, slot, static_cast<int>(reason));
		menu->handler(id, MenuAction::End, slot, 0);
	}
}

void MenuManager::OnMenuTimeout(uint32_t timerId, const plg::vector<plg::any>& userData) {
	CPlayerSlot slot = plg::get<int32_t>(userData[0]);
	MenuId id = plg::get<uint64_t>(userData[1]);

	std::scoped_lock lock(g_MenuManager.m_mutex);

	auto& state = g_MenuManager.m_clientState[static_cast<size_t>(slot)];
	if (state.id == id && state.timerId == timerId) {
		state.timerId = 0; // the timer already fired; nothing left to kill
		g_MenuManager.EndClientMenu(slot, MenuCancelReason::Timeout);
	}
}
