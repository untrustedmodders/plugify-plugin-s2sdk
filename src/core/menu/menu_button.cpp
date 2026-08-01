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
	// The HUD panel has no scrollback and no fixed height on the server side, so a page that's
	// too tall pushes the footer legend off the bottom of the screen. Capping how many items a
	// button-menu page ever shows keeps title + items + footer within a safe, always-visible
	// window; menus asking for a smaller page size are left alone, only larger/unlimited ones
	// get clamped down.
	constexpr int kMaxVisibleButtonLines = 6;

	struct ButtonMenuState {
		MenuId id{};                 // the menu handle this state was last polled for; used to detect a fresh session
		MenuId lastHeld{};           // buttons held as of the previous frame, for edge detection
		int lastDrawnCursor{-1};     // cursor position as of the last redraw
		bool frozen{};               // whether we forced MOVETYPE_NONE on this client
		double nextRefreshTime{};    // next time a keep-alive redraw is due, even with no input
		MoveType_t savedMoveType{};  // the client's movement type before we froze it
	};

	std::array<ButtonMenuState, MaxPlayers + 1> s_state{};

	// Renders one button-menu control image by its configured name (e.g. MenuButtonImageUp, "d-p" while
	// held), per MenuButtonImagePath/Extension/Width/Height. Always carries a trailing space so callers
	// can concatenate labels directly.
	std::string ButtonImage(std::string_view name) {
		return std::format("<img style='vertical-align:middle;' src='s2r://{}/{}.{}' width='{}' height='{}' /> ",
			g_pCoreConfig->MenuButtonImagePath, name, g_pCoreConfig->MenuButtonImageExtension,
			g_pCoreConfig->MenuButtonImageWidth, g_pCoreConfig->MenuButtonImageHeight);
	}

	// Renders `imageName`, or its "-p" variant while `pressed` is true.
	std::string ButtonLabel(std::string_view imageName, bool pressed) {
		return pressed ? ButtonImage(std::format("{}-p", imageName)) : ButtonImage(imageName);
	}

	void ButtonMenu_Display(MenuId id, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		int count = g_MenuManager.GetMenuItemCount(id);
		if (count == 0) {
			utils::PrintHtmlCentre(slot, std::format("<b>{}</b><br>", g_MenuManager.GetMenuTitle(id)), g_pCoreConfig->MenuButtonHtmlDuration);
			return;
		}

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int perPage = g_MenuManager.GetMenuPagination(id);
		int pageEnd = perPage > 0 ? std::min(offset + perPage, count) : count;

		int cursor = g_MenuManager.GetClientMenuCursor(playerSlot);
		if (cursor < offset || cursor >= pageEnd || !g_MenuManager.IsMenuItemSelectable(id, cursor)) {
			cursor = offset;
			while (cursor < pageEnd && !g_MenuManager.IsMenuItemSelectable(id, cursor)) {
				++cursor;
			}
			g_MenuManager.SetClientMenuCursor(playerSlot, cursor);
		}

		uint64 held = s_state[static_cast<size_t>(playerSlot)].lastHeld;
		auto isHeld = [&](InputBitMask_t button) { return (held & button) != 0; };

		std::string html;
		auto out = std::back_inserter(html);
		std::format_to(out, "<b>{}</b><br>", g_MenuManager.GetMenuTitle(id));

		for (int index = offset; index < pageEnd; ++index) {
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(id, index);
			if (style == MenuItemStyle::Spacer) {
				html += "<br>";
				continue;
			}

			plg::string display = g_MenuManager.GetMenuItemDisplay(id, index);
			if (index == cursor) {
				// The select hint rides along with the highlighted row instead of living in
				// the footer, so it's obvious which item pressing "select" will act on.
				std::format_to(out, "<font color='{}'>&gt; {}</font> {}<br>", g_pCoreConfig->MenuHighlightColor, display,
					ButtonLabel(g_pCoreConfig->MenuButtonImageSelect, isHeld(g_pCoreConfig->MenuButtonKeySelect)));
			} else if (style == MenuItemStyle::Disabled) {
				std::format_to(out, "<font color='{}'>{}</font><br>", g_pCoreConfig->MenuDisabledColor, display);
			} else {
				std::format_to(out, "{}<br>", display);
			}
		}

		html += "<br>";
		std::format_to(out, "{}{}",
			ButtonLabel(g_pCoreConfig->MenuButtonImageUp, isHeld(g_pCoreConfig->MenuButtonKeyUp)),
			ButtonLabel(g_pCoreConfig->MenuButtonImageDown, isHeld(g_pCoreConfig->MenuButtonKeyDown)));

		bool hasPrevPage = g_MenuManager.ClientMenuHasPrevPage(playerSlot);
		bool hasNextPage = g_MenuManager.ClientMenuHasNextPage(playerSlot);
		bool showExit = g_MenuManager.GetMenuExitBackButton(id) || g_MenuManager.GetMenuExitButton(id);

		// The images are fixed-width, so pad a missing prev/next slot with filler so the
		// footer's alignment stays consistent regardless of which controls are active.
		if (hasPrevPage) {
			html += ButtonLabel(g_pCoreConfig->MenuButtonImageLeft, isHeld(g_pCoreConfig->MenuButtonKeyLeft));
			html += hasNextPage ? ButtonLabel(g_pCoreConfig->MenuButtonImageRight, isHeld(g_pCoreConfig->MenuButtonKeyRight)) : ButtonImage(g_pCoreConfig->MenuButtonImageEmptyHalf);
		} else if (hasNextPage) {
			html += ButtonImage(g_pCoreConfig->MenuButtonImageEmptyHalf);
			html += ButtonLabel(g_pCoreConfig->MenuButtonImageRight, isHeld(g_pCoreConfig->MenuButtonKeyRight));
		} else if (showExit) {
			html += ButtonImage(g_pCoreConfig->MenuButtonImageEmpty);
		}

		if (showExit) {
			html += ButtonLabel(g_pCoreConfig->MenuButtonImageExit, isHeld(g_pCoreConfig->MenuButtonKeyExit));
		}

		utils::PrintHtmlCentre(slot, html, g_pCoreConfig->MenuButtonHtmlDuration);
	}

	void ButtonMenu_Close(MenuId, int playerSlot) {
		utils::PrintHtmlCentre(playerSlot, " ", 1);

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

	// Plays a configured menu sound event to the client, or does nothing if the event name is empty.
	void PlayMenuSound(int playerSlot, const plg::string& soundName) {
		if (soundName.empty()) {
			return;
		}
		utils::PlaySoundToClient(playerSlot, CHAN_AUTO, soundName.c_str(), VOL_NORM, SNDLVL_NONE, 0, PITCH_NORM, Vector(0.0f, 0.0f, 0.0f), 0.0f);
	}

	// Returns true if the cursor actually moved (i.e. it wasn't already at the boundary).
	bool MoveCursor(MenuId id, int playerSlot, int direction) {
		int count = g_MenuManager.GetMenuItemCount(id);
		if (count <= 0) {
			return false;
		}

		int next = g_MenuManager.GetClientMenuCursor(playerSlot);
		for (int step = 0; step < count; ++step) {
			next += direction;
			if (next < 0 || next >= count) {
				return false; // reached the boundary; no wraparound
			}
			if (g_MenuManager.IsMenuItemSelectable(id, next)) {
				break;
			}
		}

		if (next < 0 || next >= count || !g_MenuManager.IsMenuItemSelectable(id, next)) {
			return false;
		}

		g_MenuManager.SetClientMenuCursor(playerSlot, next);

		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int perPage = g_MenuManager.GetMenuPagination(id);
		if (perPage > 0) {
			if (next >= offset + perPage) {
				g_MenuManager.MenuNextPage(playerSlot);
			} else if (next < offset) {
				g_MenuManager.MenuPrevPage(playerSlot);
			}
		}

		return true;
	}

	void ButtonMenu_Frame(MenuId id, int playerSlot) {
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
		bool freshSession = state.id != id;

		if (freshSession) {
			state.id = id;
			state.lastDrawnCursor = -1; // force the first draw below
			state.nextRefreshTime = 0;

			int perPage = g_MenuManager.GetMenuPagination(id);
			int itemCount = g_MenuManager.GetMenuItemCount(id);
			if (perPage <= 0 ? itemCount > kMaxVisibleButtonLines : perPage > kMaxVisibleButtonLines) {
				g_MenuManager.SetMenuPagination(id, kMaxVisibleButtonLines);
			}

			if (g_pCoreConfig->MenuButtonFreezePlayer) {
				state.savedMoveType = pawn->m_MoveType;
				state.frozen = true;
				pawn->SetMoveType(MOVETYPE_NONE);
			} else {
				state.frozen = false;
			}
		} else if (state.frozen && pawn->m_MoveType != MOVETYPE_NONE) {
			// Something outside our control (e.g. a respawn on round start) reset the
			// client's move type mid-session; re-save whatever it was just set to and
			// re-freeze, since the one-shot freeze above only fires at session start.
			state.savedMoveType = pawn->m_MoveType;
			pawn->SetMoveType(MOVETYPE_NONE);
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
				if (MoveCursor(id, playerSlot, -1)) {
					PlayMenuSound(playerSlot, g_pCoreConfig->MenuSoundScroll);
				}
			} else if (released(g_pCoreConfig->MenuButtonKeyDown)) {
				if (MoveCursor(id, playerSlot, 1)) {
					PlayMenuSound(playerSlot, g_pCoreConfig->MenuSoundScroll);
				}
			} else if (released(g_pCoreConfig->MenuButtonKeyLeft)) {
				if (g_MenuManager.MenuPrevPage(playerSlot)) {
					PlayMenuSound(playerSlot, g_pCoreConfig->MenuSoundScroll);
				}
			} else if (released(g_pCoreConfig->MenuButtonKeyRight)) {
				if (g_MenuManager.MenuNextPage(playerSlot)) {
					PlayMenuSound(playerSlot, g_pCoreConfig->MenuSoundScroll);
				}
			} else if (released(g_pCoreConfig->MenuButtonKeySelect)) {
				bool selected = g_MenuManager.SelectMenuItem(playerSlot, g_MenuManager.GetClientMenuCursor(playerSlot));
				PlayMenuSound(playerSlot, selected ? g_pCoreConfig->MenuSoundClick : g_pCoreConfig->MenuSoundDisabled);
				return; // the display session likely just ended; MenuManager already invoked our Close
			} else if (released(g_pCoreConfig->MenuButtonKeyExit) && g_MenuManager.GetMenuExitBackButton(id)) {
				PlayMenuSound(playerSlot, g_pCoreConfig->MenuSoundBack);
				g_MenuManager.CancelClientMenu(playerSlot, MenuCancelReason::ExitBack);
				return;
			} else if (released(g_pCoreConfig->MenuButtonKeyExit) && g_MenuManager.GetMenuExitButton(id)) {
				PlayMenuSound(playerSlot, g_pCoreConfig->MenuSoundExit);
				g_MenuManager.CancelClientMenu(playerSlot, MenuCancelReason::Exit);
				return;
			}
		}

		double now = TimerSystem::GetTickedTime();
		int cursor = g_MenuManager.GetClientMenuCursor(playerSlot);
		if (cursor != state.lastDrawnCursor || now >= state.nextRefreshTime) {
			ButtonMenu_Display(id, playerSlot);
			state.lastDrawnCursor = cursor;
			state.nextRefreshTime = now + g_pCoreConfig->MenuButtonRefreshInterval;
		}
	}
}// namespace

void RegisterBuiltinButtonMenuType() {
	g_MenuManager.RegisterMenuType("button", &ButtonMenu_Display, &ButtonMenu_Close, &ButtonMenu_Frame);
}
