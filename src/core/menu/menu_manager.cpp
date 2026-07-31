#include "menu_manager.hpp"

#include <core/con_command_manager.hpp>
#include <core/sdk/utils.hpp>
#include <core/timer_system.hpp>

void RegisterBuiltinChatMenuType();
void RegisterBuiltinConsoleMenuType();
void RegisterBuiltinCenterHtmlMenuType();
void RegisterBuiltinButtonMenuType();

MenuManager MenuManager::instance;

namespace {
	ResultType OnMenuDigitCommand(int caller, ConCommandContext, const plg::vector<plg::string>& arguments) {
		if (arguments.empty() || arguments[0].size() != 5 || !arguments[0].starts_with("css_")) {
			return ResultType::Continue;
		}

		char digitChar = arguments[0][4];
		if (digitChar < '0' || digitChar > '9') {
			return ResultType::Continue;
		}

		if (g_MenuManager.HandleDigitInput(caller, digitChar - '0')) {
			return ResultType::Handled;
		}

		return ResultType::Continue;
	}
}// namespace

void MenuManager::Init() {
	for (int i = 0; i <= 9; ++i) {
		auto name = std::format("css_{}", i);
		g_ConCommandManager.AddValveCommand(name, "Menu key handler", ConVarFlag::None);
		g_ConCommandManager.AddCommandListener(name, &OnMenuDigitCommand, HookMode::Pre);
	}

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
	if (m_clientState[static_cast<size_t>(slot.Get())].menuHandle) {
		EndClientMenu(slot.Get(), MenuCancelReason::Disconnect);
	}
}

void MenuManager::OnGameFrame() {
	std::scoped_lock lock(m_mutex);
	for (int slot = 0; slot <= MaxPlayers; ++slot) {
		auto& state = m_clientState[static_cast<size_t>(slot)];
		if (!state.menuHandle) {
			continue;
		}

		auto menu = Find(state.menuHandle);
		if (!menu) {
			continue;
		}

		if (auto* type = FindType(*menu); type && type->frame) {
			type->frame(state.menuHandle, slot);
		}
	}
}

// -- Menu type registry -----------------------------------------------------

bool MenuManager::RegisterMenuType(std::string_view name, MenuDisplayCallback display, MenuCloseCallback close, MenuFrameCallback frame) {
	std::scoped_lock lock(m_mutex);

	if (name.empty() || !display || !close) {
		plg::print(LS_WARNING, "MenuManager: cannot register menu type with empty name or null display/close callback\n");
		return false;
	}

	if (m_menuTypes.contains(name)) {
		plg::print(LS_WARNING, "MenuManager: menu type '{}' is already registered\n", name);
		return false;
	}

	m_menuTypes.emplace(name, MenuTypeCallbacks{display, close, frame});
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
		plg::print(LS_WARNING, "MenuManager: cannot set default menu type to unregistered type '{}'\n", name);
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

uint64 MenuManager::CreateMenu(std::string_view title, MenuHandlerCallback handler, std::string_view menuType) {
	std::scoped_lock lock(m_mutex);

	auto menu = std::make_shared<MenuData>();
	menu->title = title;
	menu->typeName = menuType;
	menu->handler = handler;

	uint64 handle = m_nextHandle++;
	m_menus.emplace(handle, std::move(menu));
	return handle;
}

bool MenuManager::DestroyMenu(uint64 menuHandle) {
	std::scoped_lock lock(m_mutex);

	if (!m_menus.contains(menuHandle)) {
		return false;
	}

	for (int slot = 0; slot <= MaxPlayers; ++slot) {
		if (m_clientState[static_cast<size_t>(slot)].menuHandle == menuHandle) {
			EndClientMenu(slot, MenuCancelReason::Destroyed);
		}
	}

	m_menus.erase(menuHandle);
	return true;
}

bool MenuManager::IsValidMenu(uint64 menuHandle) const {
	std::scoped_lock lock(m_mutex);
	return m_menus.contains(menuHandle);
}

// -- Properties -----------------------------------------------------------------

bool MenuManager::SetMenuTitle(uint64 menuHandle, std::string_view title) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu) {
		return false;
	}
	menu->title = title;
	return true;
}

plg::string MenuManager::GetMenuTitle(uint64 menuHandle) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	return menu ? menu->title : plg::string{};
}

bool MenuManager::SetMenuType(uint64 menuHandle, std::string_view typeName) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu) {
		return false;
	}
	menu->typeName = typeName;
	return true;
}

plg::string MenuManager::GetMenuType(uint64 menuHandle) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	return menu ? menu->typeName : plg::string{};
}

bool MenuManager::SetMenuPagination(uint64 menuHandle, int itemsPerPage) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu || itemsPerPage < 0) {
		return false;
	}
	menu->itemsPerPage = itemsPerPage;
	return true;
}

int MenuManager::GetMenuPagination(uint64 menuHandle) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	return menu ? menu->itemsPerPage : 0;
}

bool MenuManager::SetMenuExitButton(uint64 menuHandle, bool enabled) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu) {
		return false;
	}
	menu->exitButton = enabled;
	return true;
}

bool MenuManager::GetMenuExitButton(uint64 menuHandle) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	return menu && menu->exitButton;
}

bool MenuManager::SetMenuCloseOnSelect(uint64 menuHandle, bool enabled) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu) {
		return false;
	}
	menu->closeOnSelect = enabled;
	return true;
}

bool MenuManager::GetMenuCloseOnSelect(uint64 menuHandle) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	return menu && menu->closeOnSelect;
}

// -- Items ------------------------------------------------------------------------

int MenuManager::AddMenuItem(uint64 menuHandle, std::string_view info, std::string_view display, MenuItemStyle style) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu) {
		return -1;
	}
	menu->items.push_back(MenuItemData{info, display, style});
	return static_cast<int>(menu->items.size()) - 1;
}

int MenuManager::InsertMenuItem(uint64 menuHandle, int index, std::string_view info, std::string_view display, MenuItemStyle style) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu || index < 0 || index > static_cast<int>(menu->items.size())) {
		return -1;
	}
	menu->items.insert(menu->items.begin() + index, MenuItemData{info, display, style});
	return index;
}

bool MenuManager::RemoveMenuItem(uint64 menuHandle, int index) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return false;
	}
	menu->items.erase(menu->items.begin() + index);
	return true;
}

bool MenuManager::RemoveAllMenuItems(uint64 menuHandle) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu) {
		return false;
	}
	menu->items.clear();
	return true;
}

int MenuManager::GetMenuItemCount(uint64 menuHandle) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	return menu ? static_cast<int>(menu->items.size()) : 0;
}

plg::string MenuManager::GetMenuItemInfo(uint64 menuHandle, int index) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return {};
	}
	return menu->items[static_cast<size_t>(index)].info;
}

plg::string MenuManager::GetMenuItemDisplay(uint64 menuHandle, int index) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return {};
	}
	return menu->items[static_cast<size_t>(index)].display;
}

MenuItemStyle MenuManager::GetMenuItemStyle(uint64 menuHandle, int index) const {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return MenuItemStyle::Disabled;
	}
	return menu->items[static_cast<size_t>(index)].style;
}

bool MenuManager::IsMenuItemSelectable(uint64 menuHandle, int index) const {
	return GetMenuItemStyle(menuHandle, index) == MenuItemStyle::Default;
}

bool MenuManager::SetMenuItemDisplay(uint64 menuHandle, int index, std::string_view display) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return false;
	}
	menu->items[static_cast<size_t>(index)].display = display;
	return true;
}

bool MenuManager::SetMenuItemStyle(uint64 menuHandle, int index, MenuItemStyle style) {
	std::scoped_lock lock(m_mutex);
	auto menu = Find(menuHandle);
	if (!menu || index < 0 || index >= static_cast<int>(menu->items.size())) {
		return false;
	}
	menu->items[static_cast<size_t>(index)].style = style;
	return true;
}

// -- Display / navigation --------------------------------------------------------

bool MenuManager::DisplayMenu(uint64 menuHandle, int playerSlot, int time) {
	return DisplayMenuAtItem(menuHandle, playerSlot, 0, time);
}

bool MenuManager::DisplayMenuAtItem(uint64 menuHandle, int playerSlot, int firstItem, int time) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(playerSlot)) {
		return false;
	}

	auto menu = Find(menuHandle);
	if (!menu) {
		return false;
	}

	const auto* type = FindType(*menu);
	if (!type) {
		plg::print(LS_WARNING, "MenuManager: unknown menu type '{}'\n", menu->typeName.empty() ? m_defaultMenuType : menu->typeName);
		return false;
	}

	if (m_clientState[static_cast<size_t>(playerSlot)].menuHandle) {
		EndClientMenu(playerSlot, MenuCancelReason::Interrupted);
	}

	auto& state = m_clientState[static_cast<size_t>(playerSlot)];
	state = ClientMenuState{};
	state.menuHandle = menuHandle;
	state.currentOffset = menu->items.empty() ? 0 : std::clamp(firstItem, 0, static_cast<int>(menu->items.size()) - 1);
	state.cursor = state.currentOffset;
	state.menuTime = time > 0 ? time : 0;

	if (time > 0) {
		state.timerId = g_TimerSystem.CreateTimer(static_cast<double>(time), &MenuManager::OnMenuTimeout, TimerFlag::Default,
			{static_cast<int32_t>(playerSlot), menuHandle});
	}

	if (menu->handler) {
		menu->handler(menuHandle, MenuAction::Start, playerSlot, 0);
	}

	type->display(menuHandle, playerSlot);
	return true;
}

bool MenuManager::CancelClientMenu(int playerSlot, MenuCancelReason reason) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(playerSlot)) {
		return false;
	}

	if (!m_clientState[static_cast<size_t>(playerSlot)].menuHandle) {
		return false;
	}

	EndClientMenu(playerSlot, reason);
	return true;
}

uint64 MenuManager::GetClientMenu(int playerSlot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(playerSlot)) {
		return 0;
	}
	return m_clientState[static_cast<size_t>(playerSlot)].menuHandle;
}

int MenuManager::GetClientMenuOffset(int playerSlot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(playerSlot)) {
		return 0;
	}
	return m_clientState[static_cast<size_t>(playerSlot)].currentOffset;
}

int MenuManager::GetClientMenuTime(int playerSlot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(playerSlot)) {
		return 0;
	}
	return m_clientState[static_cast<size_t>(playerSlot)].menuTime;
}

int MenuManager::GetClientMenuCursor(int playerSlot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(playerSlot)) {
		return 0;
	}
	return m_clientState[static_cast<size_t>(playerSlot)].cursor;
}

void MenuManager::SetClientMenuCursor(int playerSlot, int index) {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(playerSlot)) {
		return;
	}
	m_clientState[static_cast<size_t>(playerSlot)].cursor = index;
}

bool MenuManager::ClientMenuHasPrevPage(int playerSlot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(playerSlot)) {
		return false;
	}
	return !m_clientState[static_cast<size_t>(playerSlot)].prevOffsets.empty();
}

bool MenuManager::ClientMenuHasNextPage(int playerSlot) const {
	std::scoped_lock lock(m_mutex);
	if (!utils::IsPlayerSlot(playerSlot)) {
		return false;
	}
	const auto& state = m_clientState[static_cast<size_t>(playerSlot)];
	auto menu = Find(state.menuHandle);
	if (!menu || menu->itemsPerPage <= 0) {
		return false;
	}
	return state.currentOffset + menu->itemsPerPage < static_cast<int>(menu->items.size());
}

bool MenuManager::MenuNextPage(int playerSlot) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(playerSlot)) {
		return false;
	}

	auto& state = m_clientState[static_cast<size_t>(playerSlot)];
	auto menu = Find(state.menuHandle);
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
	RedisplayClient(playerSlot);
	return true;
}

bool MenuManager::MenuPrevPage(int playerSlot) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(playerSlot)) {
		return false;
	}

	auto& state = m_clientState[static_cast<size_t>(playerSlot)];
	if (!state.menuHandle || state.prevOffsets.empty()) {
		return false;
	}

	state.currentOffset = state.prevOffsets.back();
	state.prevOffsets.pop_back();
	state.page--;
	RedisplayClient(playerSlot);
	return true;
}

bool MenuManager::SelectMenuItem(int playerSlot, int itemIndex) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(playerSlot)) {
		return false;
	}

	auto& state = m_clientState[static_cast<size_t>(playerSlot)];
	uint64 handle = state.menuHandle;
	auto menu = Find(handle);
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
		CloseClientDisplay(playerSlot, true);
	}

	if (menu->handler) {
		menu->handler(handle, MenuAction::Select, playerSlot, itemIndex);
	}

	if (shouldClose && menu->handler) {
		menu->handler(handle, MenuAction::End, playerSlot, 0);
	}

	return true;
}

bool MenuManager::HandleDigitInput(int playerSlot, int digit) {
	std::scoped_lock lock(m_mutex);

	if (!utils::IsPlayerSlot(playerSlot)) {
		return false;
	}

	const auto& state = m_clientState[static_cast<size_t>(playerSlot)];
	if (!state.menuHandle) {
		return false;
	}

	auto menu = Find(state.menuHandle);
	if (!menu) {
		return false;
	}

	switch (digit) {
		case 0:
			if (!menu->exitButton) {
				return false;
			}
			return CancelClientMenu(playerSlot, MenuCancelReason::Exit);
		case 8:
			return MenuPrevPage(playerSlot);
		case 9:
			return MenuNextPage(playerSlot);
		default:
			return SelectMenuItem(playerSlot, state.currentOffset + digit - 1);
	}
}

// -- Internals ----------------------------------------------------------------------

std::shared_ptr<MenuData> MenuManager::Find(uint64 menuHandle) const {
	auto it = m_menus.find(menuHandle);
	return it != m_menus.end() ? it->second : nullptr;
}

const MenuTypeCallbacks* MenuManager::FindType(const MenuData& menu) const {
	std::string_view typeName = menu.typeName.empty() ? m_defaultMenuType : menu.typeName;
	auto it = m_menuTypes.find(typeName);
	return it != m_menuTypes.end() ? &it->second : nullptr;
}

void MenuManager::RedisplayClient(int playerSlot) {
	const auto& state = m_clientState[static_cast<size_t>(playerSlot)];
	auto menu = Find(state.menuHandle);
	if (!menu) {
		return;
	}
	if (const auto* type = FindType(*menu)) {
		type->display(state.menuHandle, playerSlot);
	}
}

void MenuManager::CloseClientDisplay(int playerSlot, bool notifyBackend) {
	auto& state = m_clientState[static_cast<size_t>(playerSlot)];

	if (state.timerId) {
		g_TimerSystem.KillTimer(state.timerId);
	}

	if (notifyBackend && state.menuHandle) {
		if (auto menu = Find(state.menuHandle)) {
			if (const auto* type = FindType(*menu)) {
				type->close(state.menuHandle, playerSlot);
			}
		}
	}

	state = ClientMenuState{};
}

void MenuManager::EndClientMenu(int playerSlot, MenuCancelReason reason) {
	auto& state = m_clientState[static_cast<size_t>(playerSlot)];
	if (!state.menuHandle) {
		return;
	}

	uint64 handle = state.menuHandle;
	auto menu = Find(handle);

	CloseClientDisplay(playerSlot, reason != MenuCancelReason::Disconnect);

	if (menu && menu->handler) {
		menu->handler(handle, MenuAction::Cancel, playerSlot, static_cast<int>(reason));
		menu->handler(handle, MenuAction::End, playerSlot, 0);
	}
}

void MenuManager::OnMenuTimeout(uint32_t timerId, const plg::vector<plg::any>& userData) {
	int slot = plg::get<int32_t>(userData[0]);
	uint64 handle = plg::get<uint64_t>(userData[1]);

	std::scoped_lock lock(g_MenuManager.m_mutex);

	auto& state = g_MenuManager.m_clientState[static_cast<size_t>(slot)];
	if (state.menuHandle == handle && state.timerId == timerId) {
		state.timerId = 0; // the timer already fired; nothing left to kill
		g_MenuManager.EndClientMenu(slot, MenuCancelReason::Timeout);
	}
}
