#include "menu_manager.hpp"

#include <core/sdk/utils.hpp>

// Chat, console and center-html menus are all "digit driven": the client selects an
// item by triggering the shared css_0..css_9 commands registered by MenuManager::Init.
// They differ only in how they render the current page.

namespace {
	constexpr int kDigitKeysPerPage = 7; // keys 1-7 select items, 8 = back, 9 = next, 0 = exit

	int VisibleItemCount(uint64 handle, int offset) {
		int count = g_MenuManager.GetMenuItemCount(handle);
		int perPage = g_MenuManager.GetMenuPagination(handle);
		int pageSize = perPage > 0 ? std::min(perPage, kDigitKeysPerPage) : std::min(count, kDigitKeysPerPage);
		return std::max(0, std::min(pageSize, count - offset));
	}

	// -- ChatMenu --------------------------------------------------------

	void ChatMenu_Display(uint64 handle, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		utils::PrintChat(slot, std::format(" {}", g_MenuManager.GetMenuTitle(handle)));
		utils::PrintChat(slot, " --------------------");

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int shown = VisibleItemCount(handle, offset);

		for (int i = 0; i < shown; ++i) {
			int index = offset + i;
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(handle, index);
			if (style == MenuItemStyle::Spacer) {
				utils::PrintChat(slot, " ");
				continue;
			}

			std::string_view suffix = style == MenuItemStyle::Disabled ? " (disabled)" : "";
			utils::PrintChat(slot, std::format(" !{} {}{}", i + 1, g_MenuManager.GetMenuItemDisplay(handle, index), suffix));
		}

		if (g_MenuManager.ClientMenuHasPrevPage(playerSlot)) {
			utils::PrintChat(slot, " !8 Back");
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			utils::PrintChat(slot, " !9 Next");
		}
		if (g_MenuManager.GetMenuExitButton(handle)) {
			utils::PrintChat(slot, " !0 Exit");
		}
	}

	void ChatMenu_Close(uint64, int) {
		// Chat history has no explicit "close" concept; nothing to do.
	}

	// -- ConsoleMenu -------------------------------------------------------

	void ConsoleMenu_Display(uint64 handle, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		utils::PrintConsole(slot, std::format("{}", g_MenuManager.GetMenuTitle(handle)));
		utils::PrintConsole(slot, "--------------------");

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int shown = VisibleItemCount(handle, offset);

		for (int i = 0; i < shown; ++i) {
			int index = offset + i;
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(handle, index);
			if (style == MenuItemStyle::Spacer) {
				utils::PrintConsole(slot, "");
				continue;
			}

			std::string_view suffix = style == MenuItemStyle::Disabled ? " (disabled)" : "";
			utils::PrintConsole(slot, std::format("{}. {}{}", i + 1, g_MenuManager.GetMenuItemDisplay(handle, index), suffix));
		}

		if (g_MenuManager.ClientMenuHasPrevPage(playerSlot)) {
			utils::PrintConsole(slot, "8. Back");
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			utils::PrintConsole(slot, "9. Next");
		}
		if (g_MenuManager.GetMenuExitButton(handle)) {
			utils::PrintConsole(slot, "0. Exit");
		}

		utils::PrintConsole(slot, "Use 'css_<number>' to make a selection.");
	}

	void ConsoleMenu_Close(uint64, int) {
		// Console output has no explicit "close" concept; nothing to do.
	}

	// -- CenterHtmlMenu ------------------------------------------------------

	constexpr int kCenterHtmlDefaultDuration = 30;

	void CenterHtmlMenu_Display(uint64 handle, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		std::string html = std::format("<b>{}</b><br>", g_MenuManager.GetMenuTitle(handle));

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int shown = VisibleItemCount(handle, offset);

		for (int i = 0; i < shown; ++i) {
			int index = offset + i;
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(handle, index);
			if (style == MenuItemStyle::Spacer) {
				html += "<br>";
				continue;
			}

			if (style == MenuItemStyle::Disabled) {
				html += std::format("<font color='#808080'>{}. {}</font><br>", i + 1, g_MenuManager.GetMenuItemDisplay(handle, index));
			} else {
				html += std::format("{}. {}<br>", i + 1, g_MenuManager.GetMenuItemDisplay(handle, index));
			}
		}

		if (g_MenuManager.ClientMenuHasPrevPage(playerSlot)) {
			html += "8. Back<br>";
		}
		if (g_MenuManager.ClientMenuHasNextPage(playerSlot)) {
			html += "9. Next<br>";
		}
		if (g_MenuManager.GetMenuExitButton(handle)) {
			html += "0. Exit<br>";
		}

		int time = g_MenuManager.GetClientMenuTime(playerSlot);
		utils::PrintHtmlCentre(slot, html, time > 0 ? time : kCenterHtmlDefaultDuration);
	}

	void CenterHtmlMenu_Close(uint64, int playerSlot) {
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
