#include "menu_manager.hpp"

#include <core/core_config.hpp>
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

			std::string_view suffix = style == MenuItemStyle::Disabled ? " (disabled)" : "";
			utils::PrintChat(slot, std::format(" !{} {}{}", i + 1, g_MenuManager.GetMenuItemDisplay(id, index), suffix));
		}

		if (g_MenuManager.ClientMenuHasPrevPage(playerSlot)) {
			utils::PrintChat(slot, " !8 Prev");
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			utils::PrintChat(slot, " !9 Next");
		}
		if (g_MenuManager.GetMenuExitBackButton(id)) {
			utils::PrintChat(slot, " !0 Back");
		} else if (g_MenuManager.GetMenuExitButton(id)) {
			utils::PrintChat(slot, " !0 Exit");
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

			std::string_view suffix = style == MenuItemStyle::Disabled ? " (disabled)" : "";
			utils::PrintConsole(slot, std::format("{}. {}{}", i + 1, g_MenuManager.GetMenuItemDisplay(id, index), suffix));
		}

		if (g_MenuManager.ClientMenuHasPrevPage(playerSlot)) {
			utils::PrintConsole(slot, "8. Prev");
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			utils::PrintConsole(slot, "9. Next");
		}
		if (g_MenuManager.GetMenuExitBackButton(id)) {
			utils::PrintConsole(slot, "0. Back");
		} else if (g_MenuManager.GetMenuExitButton(id)) {
			utils::PrintConsole(slot, "0. Exit");
		}

		utils::PrintConsole(slot, "Use 'css_<number>' to make a selection.");
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
			html += "8. Prev<br>";
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			html += "9. Next<br>";
		}
		if (g_MenuManager.GetMenuExitBackButton(id)) {
			html += "0. Back<br>";
		} else if (g_MenuManager.GetMenuExitButton(id)) {
			html += "0. Exit<br>";
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
