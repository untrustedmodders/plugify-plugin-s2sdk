#include "menu_manager.hpp"

#include <core/player_manager.hpp>
#include <core/sdk/entity/cbaseplayerpawn.h>
#include <core/sdk/entity/globaltypes.h>
#include <core/sdk/entity/services.h>
#include <core/sdk/utils.hpp>

// A WASD-style menu: navigated with movement keys and rendered as a center HTML panel
// redrawn every server frame while it's open (built entirely on MenuManager's public API,
// the same surface an out-of-module plugin registering its own menu type would use).

namespace {
	constexpr int kHtmlDuration = 2; // seconds; refreshed every frame so it never actually fades while open

	std::array<uint64_t, MaxPlayers + 1> s_lastMenuHandle{};
	std::array<uint64_t, MaxPlayers + 1> s_lastHeld{};

	void ButtonMenu_Display(uint64 handle, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		int count = g_MenuManager.GetMenuItemCount(handle);
		if (count == 0) {
			utils::PrintHtmlCentre(slot, std::format("<b>{}</b><br>", g_MenuManager.GetMenuTitle(handle)), kHtmlDuration);
			return;
		}

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int perPage = g_MenuManager.GetMenuPagination(handle);
		int pageEnd = perPage > 0 ? std::min(offset + perPage, count) : count;

		int cursor = g_MenuManager.GetClientMenuCursor(playerSlot);
		if (cursor < offset || cursor >= pageEnd || !g_MenuManager.IsMenuItemSelectable(handle, cursor)) {
			cursor = offset;
			while (cursor < pageEnd && !g_MenuManager.IsMenuItemSelectable(handle, cursor)) {
				++cursor;
			}
			g_MenuManager.SetClientMenuCursor(playerSlot, cursor);
		}

		std::string html = std::format("<b>{}</b><br>", g_MenuManager.GetMenuTitle(handle));

		for (int index = offset; index < pageEnd; ++index) {
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(handle, index);
			if (style == MenuItemStyle::Spacer) {
				html += "<br>";
				continue;
			}

			plg::string display = g_MenuManager.GetMenuItemDisplay(handle, index);
			if (index == cursor) {
				html += std::format("<font color='#FFD700'>&gt; {}</font><br>", display);
			} else if (style == MenuItemStyle::Disabled) {
				html += std::format("<font color='#808080'>{}</font><br>", display);
			} else {
				html += std::format("{}<br>", display);
			}
		}

		html += "<br><font class='fontSize-s'>[FORWARD/BACK] Move  [USE] Select";
		if (g_MenuManager.GetMenuExitButton(handle)) {
			html += "  [ATTACK2] Exit";
		}
		html += "</font>";

		utils::PrintHtmlCentre(slot, html, kHtmlDuration);
	}

	void ButtonMenu_Close(uint64, int playerSlot) {
		utils::PrintHtmlCentre(CPlayerSlot(playerSlot), " ", 1);
	}

	void MoveCursor(uint64 handle, int playerSlot, int direction) {
		int count = g_MenuManager.GetMenuItemCount(handle);
		if (count <= 0) {
			return;
		}

		int next = g_MenuManager.GetClientMenuCursor(playerSlot);
		for (int step = 0; step < count; ++step) {
			next += direction;
			if (next < 0 || next >= count) {
				return; // reached the boundary; no wraparound
			}
			if (g_MenuManager.IsMenuItemSelectable(handle, next)) {
				break;
			}
		}

		if (next < 0 || next >= count || !g_MenuManager.IsMenuItemSelectable(handle, next)) {
			return;
		}

		g_MenuManager.SetClientMenuCursor(playerSlot, next);

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int perPage = g_MenuManager.GetMenuPagination(handle);
		if (perPage > 0) {
			if (next >= offset + perPage) {
				g_MenuManager.MenuNextPage(playerSlot);
			} else if (next < offset) {
				g_MenuManager.MenuPrevPage(playerSlot);
			}
		}
	}

	void ButtonMenu_Frame(uint64 handle, int playerSlot) {
		auto* player = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
		auto* pawn = player ? player->GetPlayerPawn() : nullptr;
		if (!pawn) {
			return;
		}

		CPlayer_MovementServices* movement = pawn->m_pMovementServices;
		if (!movement) {
			return;
		}

		uint64 buttons[3];
		movement->m_nButtons->GetButtons(buttons);
		uint64 held = buttons[0];

		size_t slotIndex = static_cast<size_t>(playerSlot);
		bool freshSession = s_lastMenuHandle[slotIndex] != handle;
		uint64 prevHeld = freshSession ? held : s_lastHeld[slotIndex];

		s_lastMenuHandle[slotIndex] = handle;
		s_lastHeld[slotIndex] = held;

		auto released = [&](uint64 button) {
			return (prevHeld & button) != 0 && (held & button) == 0;
		};

		if (!freshSession) {
			if (released(IN_FORWARD)) {
				MoveCursor(handle, playerSlot, -1);
			} else if (released(IN_BACK)) {
				MoveCursor(handle, playerSlot, 1);
			} else if (released(IN_USE)) {
				g_MenuManager.SelectMenuItem(playerSlot, g_MenuManager.GetClientMenuCursor(playerSlot));
				return; // the display session likely just ended; MenuManager already invoked our Close
			} else if (released(IN_ATTACK2) && g_MenuManager.GetMenuExitButton(handle)) {
				g_MenuManager.CancelClientMenu(playerSlot, MenuCancelReason::Exit);
				return;
			}
		}

		ButtonMenu_Display(handle, playerSlot);
	}
}// namespace

void RegisterBuiltinButtonMenuType() {
	g_MenuManager.RegisterMenuType("button", &ButtonMenu_Display, &ButtonMenu_Close, &ButtonMenu_Frame);
}
