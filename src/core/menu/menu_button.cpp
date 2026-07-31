#include "menu_manager.hpp"

#include <core/core_config.hpp>
#include <core/player_manager.hpp>
#include <core/sdk/entity/cbaseplayerpawn.h>
#include <core/sdk/entity/globaltypes.h>
#include <core/sdk/entity/services.h>
#include <core/sdk/utils.hpp>
#include <core/timer_system.hpp>
#include <iterator>

// A WASD-style menu: navigated with movement keys and rendered as a center HTML panel,
// redrawn only when the cursor/page changes or the keep-alive interval elapses (built
// entirely on MenuManager's public API, the same surface an out-of-module plugin
// registering its own menu type would use).

namespace {
	struct ButtonMenuState {
		MenuId id{};                 // the menu handle this state was last polled for; used to detect a fresh session
		uint64_t lastHeld{};         // buttons held as of the previous frame, for edge detection
		int lastDrawnCursor{-1};     // cursor position as of the last redraw
		bool frozen{};               // whether we forced MOVETYPE_NONE on this client
		double nextRefreshTime{};    // next time a keep-alive redraw is due, even with no input
		MoveType_t savedMoveType{};  // the client's movement type before we froze it
	};

	std::array<ButtonMenuState, MaxPlayers + 1> s_state{};

	std::string_view ButtonName(uint64_t button) {
		switch (button) {
			case IN_ATTACK: return "ATTACK";
			case IN_JUMP: return "JUMP";
			case IN_DUCK: return "DUCK";
			case IN_FORWARD: return "FORWARD";
			case IN_BACK: return "BACK";
			case IN_USE: return "USE";
			case IN_TURNLEFT: return "TURNLEFT";
			case IN_TURNRIGHT: return "TURNRIGHT";
			case IN_MOVELEFT: return "MOVELEFT";
			case IN_MOVERIGHT: return "MOVERIGHT";
			case IN_ATTACK2: return "ATTACK2";
			case IN_RELOAD: return "RELOAD";
			case IN_SPEED: return "SPEED";
			case IN_JOYAUTOSPRINT: return "SPRINT";
			case IN_USEORRELOAD: return "USE/RELOAD";
			case IN_SCORE: return "SCORE";
			case IN_ZOOM: return "ZOOM";
			case IN_LOOK_AT_WEAPON: return "LOOK_AT_WEAPON";
			default: return "?";
		}
	}

	void ButtonMenu_Display(MenuId handle, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		int count = g_MenuManager.GetMenuItemCount(handle);
		if (count == 0) {
			utils::PrintHtmlCentre(slot, std::format("<b>{}</b><br>", g_MenuManager.GetMenuTitle(handle)), g_pCoreConfig->MenuButtonHtmlDuration);
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

		std::string html;
		auto out = std::back_inserter(html);
		std::format_to(out, "<b>{}</b><br>", g_MenuManager.GetMenuTitle(handle));

		for (int index = offset; index < pageEnd; ++index) {
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(handle, index);
			if (style == MenuItemStyle::Spacer) {
				html += "<br>";
				continue;
			}

			plg::string display = g_MenuManager.GetMenuItemDisplay(handle, index);
			if (index == cursor) {
				std::format_to(out, "<font color='{}'>&gt; {}</font><br>", g_pCoreConfig->MenuHighlightColor, std::string_view(display));
			} else if (style == MenuItemStyle::Disabled) {
				std::format_to(out, "<font color='{}'>{}</font><br>", g_pCoreConfig->MenuDisabledColor, std::string_view(display));
			} else {
				std::format_to(out, "{}<br>", std::string_view(display));
			}
		}

		std::format_to(out, "<br><font class='fontSize-s'>[{}/{}] Move  [{}] Select",
			ButtonName(g_pCoreConfig->MenuButtonKeyUp), ButtonName(g_pCoreConfig->MenuButtonKeyDown), ButtonName(g_pCoreConfig->MenuButtonKeySelect));
		if (g_MenuManager.GetMenuExitButton(handle)) {
			std::format_to(out, "  [{}] Exit", ButtonName(g_pCoreConfig->MenuButtonKeyExit));
		}
		html += "</font>";

		utils::PrintHtmlCentre(slot, html, g_pCoreConfig->MenuButtonHtmlDuration);
	}

	void ButtonMenu_Close(MenuId, int playerSlot) {
		utils::PrintHtmlCentre(CPlayerSlot(playerSlot), " ", 1);

		ButtonMenuState& state = s_state[static_cast<size_t>(playerSlot)];
		if (state.frozen) {
			state.frozen = false;
			auto* player = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
			auto* pawn = player ? player->GetPlayerPawn() : nullptr;
			if (pawn && pawn->m_MoveType == MOVETYPE_NONE) {
				pawn->SetMoveType(state.savedMoveType);
			}
		}

		state.id = 0;
	}

	void MoveCursor(MenuId handle, int playerSlot, int direction) {
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

	void ButtonMenu_Frame(MenuId handle, int playerSlot) {
		auto* player = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
		auto* pawn = player ? player->GetPlayerPawn() : nullptr;
		if (!pawn) {
			return;
		}

		CPlayer_MovementServices* movement = pawn->m_pMovementServices;
		if (!movement) {
			return;
		}

		ButtonMenuState& state = s_state[static_cast<size_t>(playerSlot)];
		bool freshSession = state.id != handle;

		if (freshSession) {
			state.id = handle;
			state.lastDrawnCursor = -1; // force the first draw below
			state.nextRefreshTime = 0;

			if (g_pCoreConfig->MenuButtonFreezePlayer) {
				state.savedMoveType = pawn->m_MoveType;
				state.frozen = true;
				pawn->SetMoveType(MOVETYPE_NONE);
			} else {
				state.frozen = false;
			}
		}

		uint64 buttons[3];
		movement->m_nButtons->GetButtons(buttons);
		uint64 held = buttons[0];
		uint64 prevHeld = freshSession ? held : state.lastHeld;
		state.lastHeld = held;

		auto released = [&](uint64 button) {
			return (prevHeld & button) != 0 && (held & button) == 0;
		};

		if (!freshSession) {
			if (released(g_pCoreConfig->MenuButtonKeyUp)) {
				MoveCursor(handle, playerSlot, -1);
			} else if (released(g_pCoreConfig->MenuButtonKeyDown)) {
				MoveCursor(handle, playerSlot, 1);
			} else if (released(g_pCoreConfig->MenuButtonKeySelect)) {
				g_MenuManager.SelectMenuItem(playerSlot, g_MenuManager.GetClientMenuCursor(playerSlot));
				return; // the display session likely just ended; MenuManager already invoked our Close
			} else if (released(g_pCoreConfig->MenuButtonKeyExit) && g_MenuManager.GetMenuExitButton(handle)) {
				g_MenuManager.CancelClientMenu(playerSlot, MenuCancelReason::Exit);
				return;
			}
		}

		double now = TimerSystem::GetTickedTime();
		int cursor = g_MenuManager.GetClientMenuCursor(playerSlot);
		if (cursor != state.lastDrawnCursor || now >= state.nextRefreshTime) {
			ButtonMenu_Display(handle, playerSlot);
			state.lastDrawnCursor = cursor;
			state.nextRefreshTime = now + g_pCoreConfig->MenuButtonRefreshInterval;
		}
	}
}// namespace

void RegisterBuiltinButtonMenuType() {
	g_MenuManager.RegisterMenuType("button", &ButtonMenu_Display, &ButtonMenu_Close, &ButtonMenu_Frame);
}
