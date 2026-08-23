#include "menu_manager.hpp"

#include <core/core_config.hpp>
#include <core/localization.hpp>
#include <core/sdk/utils.hpp>
#include <iterator>

// Chat, console and center-html menus are all "digit driven": the client selects an
// item by triggering the shared css_0..css_9 commands registered by MenuManager::Init.
// They differ only in how they render the current page.

namespace {
	constexpr int kDigitKeysPerPage = 7; // keys 1-7 select items, 8 = back, 9 = next, 0 = exit

	int VisibleItemCount(MenuId id, int offset) {
		int count = g_MenuManager.GetMenuItemCount(id);
		int perPage = g_MenuManager.GetMenuPagination(id);
		int pageSize = perPage > 0 ? std::min(perPage, kDigitKeysPerPage) : std::min(count, kDigitKeysPerPage);
		return std::max(0, std::min(pageSize, count - offset));
	}

	// The actual command that selects an item, based on whatever digit-input path is configured
	// (MenuCommandPrefixes takes priority since it's the shared HandleDigitInput path; falls back
	// to the "menuselect" command). Empty if neither input path is configured at all.
	std::string DigitInputCommand() {
		if (!g_pCoreConfig->MenuCommandPrefixes.empty()) {
			return std::format("{}<number>", g_pCoreConfig->MenuCommandPrefixes.front());
		}
		if (!g_pCoreConfig->MenuSelectCommands.empty()) {
			return std::format("{} <number>", g_pCoreConfig->MenuSelectCommands.front());
		}
		return {};
	}

	// -- ChatMenu --------------------------------------------------------

	void ChatMenu_Display(MenuId id, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		utils::PrintChat(slot, ' ' + g_MenuManager.GetMenuTitle(id));
		utils::PrintChat(slot, " --------------------");

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int shown = VisibleItemCount(id, offset);

		for (int i = 0; i < shown; ++i) {
			int index = offset + i;
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(id, index);
			if (style == MenuItemStyle::Spacer) {
				utils::PrintChat(slot, " ");
				continue;
			}

			plg::string suffix = style == MenuItemStyle::Disabled ? ' ' + lang::Get(playerSlot, "S2SDK.Menu.Disabled").value_or("Disabled") : plg::string{};
			utils::PrintChat(slot, std::format(" !{} {}{}", i + 1, g_MenuManager.GetMenuItemDisplay(id, index), suffix));
		}

		if (g_MenuManager.ClientMenuHasPrevPage(playerSlot)) {
			utils::PrintChat(slot, " !8 " + lang::Get(playerSlot, "S2SDK.Menu.Prev").value_or("Prev"));
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			utils::PrintChat(slot, " !9 " + lang::Get(playerSlot, "S2SDK.Menu.Next").value_or("Next"));
		}
		if (g_MenuManager.GetMenuExitBackButton(id)) {
			utils::PrintChat(slot, " !0 " + lang::Get(playerSlot, "S2SDK.Menu.Back").value_or("Back"));
		} else if (g_MenuManager.GetMenuExitButton(id)) {
			utils::PrintChat(slot, " !0 " + lang::Get(playerSlot, "S2SDK.Menu.Exit").value_or("Exit"));
		}

		// The "!1" hints above only work as literal chat input if a bare digit-named command is
		// registered; by default only "<prefix><number>" is, so spell out what to actually type.
		std::string command = DigitInputCommand();
		if (!command.empty()) {
			std::string_view trigger = g_pCoreConfig->PublicChatTrigger.empty() ? std::string_view{} : std::string_view(g_pCoreConfig->PublicChatTrigger.front());
			utils::PrintChat(slot, ' ' + lang::Format(playerSlot, "S2SDK.Menu.SelectHintChat", trigger, command).value_or(std::format("Type '{0}{1}' in chat to make a selection.", trigger, command)));
		}
	}

	void ChatMenu_Close(MenuId, int) {
		// Chat history has no explicit "close" concept; nothing to do.
	}

	// -- ConsoleMenu -------------------------------------------------------

	void ConsoleMenu_Display(MenuId id, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		utils::PrintConsole(slot, g_MenuManager.GetMenuTitle(id));
		utils::PrintConsole(slot, "--------------------");

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int shown = VisibleItemCount(id, offset);

		for (int i = 0; i < shown; ++i) {
			int index = offset + i;
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(id, index);
			if (style == MenuItemStyle::Spacer) {
				utils::PrintConsole(slot, "");
				continue;
			}

			plg::string suffix = style == MenuItemStyle::Disabled ? ' ' + lang::Get(playerSlot, "S2SDK.Menu.Disabled").value_or("Disabled") : plg::string{};
			utils::PrintConsole(slot, std::format("{}. {}{}", i + 1, g_MenuManager.GetMenuItemDisplay(id, index), suffix));
		}

		if (g_MenuManager.ClientMenuHasPrevPage(playerSlot)) {
			utils::PrintConsole(slot, "8. " + lang::Get(playerSlot, "S2SDK.Menu.Prev").value_or("Prev"));
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			utils::PrintConsole(slot, "9. " + lang::Get(playerSlot, "S2SDK.Menu.Next").value_or("Next"));
		}
		if (g_MenuManager.GetMenuExitBackButton(id)) {
			utils::PrintConsole(slot, "0. " + lang::Get(playerSlot, "S2SDK.Menu.Back").value_or("Back"));
		} else if (g_MenuManager.GetMenuExitButton(id)) {
			utils::PrintConsole(slot, "0. " + lang::Get(playerSlot, "S2SDK.Menu.Exit").value_or("Exit"));
		}

		std::string command = DigitInputCommand();
		if (!command.empty()) {
			utils::PrintConsole(slot, lang::Format(playerSlot, "S2SDK.Menu.SelectHintConsole", command).value_or(std::format("Use '{0}' to make a selection.", command)));
		}
	}

	void ConsoleMenu_Close(MenuId, int) {
		// Console output has no explicit "close" concept; nothing to do.
	}

	// -- CenterHtmlMenu ------------------------------------------------------

	void CenterHtmlMenu_Display(MenuId id, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		std::string html;
		auto out = std::back_inserter(html);
		std::format_to(out, "<b>{}</b><br>", g_MenuManager.GetMenuTitle(id));

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int shown = VisibleItemCount(id, offset);

		for (int i = 0; i < shown; ++i) {
			int index = offset + i;
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(id, index);
			if (style == MenuItemStyle::Spacer) {
				html += "<br>";
				continue;
			}

			if (style == MenuItemStyle::Disabled) {
				std::format_to(out, "<font color='{}'>{}. {}</font><br>", g_pCoreConfig->MenuDisabledColor, i + 1, g_MenuManager.GetMenuItemDisplay(id, index));
			} else {
				std::format_to(out, "{}. {}<br>", i + 1, g_MenuManager.GetMenuItemDisplay(id, index));
			}
		}

		if (g_MenuManager.ClientMenuHasPrevPage(playerSlot)) {
			std::format_to(out, "8. {}<br>", lang::Get(playerSlot, "S2SDK.Menu.Prev").value_or("Prev"));
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			std::format_to(out, "9. {}<br>", lang::Get(playerSlot, "S2SDK.Menu.Next").value_or("Next"));
		}
		if (g_MenuManager.GetMenuExitBackButton(id)) {
			std::format_to(out, "0. {}<br>", lang::Get(playerSlot, "S2SDK.Menu.Back").value_or("Back"));
		} else if (g_MenuManager.GetMenuExitButton(id)) {
			std::format_to(out, "0. {}<br>", lang::Get(playerSlot, "S2SDK.Menu.Exit").value_or("Exit"));
		}

		std::string command = DigitInputCommand();
		if (!command.empty()) {
			std::format_to(out, "{}<br>", lang::Format(playerSlot, "S2SDK.Menu.SelectHintConsole", command).value_or(std::format("Use '{0}' to make a selection.", command)));
		}

		double time = g_MenuManager.GetClientMenuTime(playerSlot);
		utils::PrintHtmlCentre(slot, html, time > 0 ? static_cast<int>(std::round(time)) : g_pCoreConfig->MenuCenterHtmlDuration);
	}

	void CenterHtmlMenu_Close(MenuId, int playerSlot) {
		utils::PrintHtmlCentre(CPlayerSlot(playerSlot), " ", 1);
	}
}// namespace

void RegisterBuiltinChatMenuType() {
	g_MenuManager.RegisterMenuType("chat", &ChatMenu_Display, &ChatMenu_Close);
}

void RegisterBuiltinConsoleMenuType() {
	g_MenuManager.RegisterMenuType("console", &ConsoleMenu_Display, &ConsoleMenu_Close);
}

void RegisterBuiltinCenterHtmlMenuType() {
	g_MenuManager.RegisterMenuType("centerhtml", &CenterHtmlMenu_Display, &CenterHtmlMenu_Close);
}
