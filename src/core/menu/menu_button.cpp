#include "menu_manager.hpp"

#include <core/core_config.hpp>
#include <core/player_manager.hpp>
#include <core/sdk/entity/cbaseplayerpawn.h>
#include <core/sdk/entity/cplayerpawn.h>
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
		int lastDrawnCursor{-1};     // cursor position as of the last redraw
		double nextRefreshTime{};    // next time a keep-alive redraw is due, even with no input
		float savedSpeed{1.0f};      // the client's velocity modifier before we froze it
		bool frozen{};               // whether we forced the client's speed to 0 while the menu is open
		uint64 lastHeld{};           // buttons held as of the previous frame, for edge detection
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

	// Renders the bolded title line, optionally wrapped in MenuButtonTitleFontClass.
	std::string TitleHtml(std::string_view title) {
		if (g_pCoreConfig->MenuButtonTitleFontClass.empty()) {
			return std::format("<b>{}</b><br>", title);
		}
		return std::format("<font class='{}'><b>{}</b></font><br>", g_pCoreConfig->MenuButtonTitleFontClass, title);
	}

	// Wraps `html` in `cssClass` if one is configured, else returns it unchanged.
	std::string ApplyFontClass(std::string_view html, const plg::string& cssClass) {
		if (cssClass.empty()) {
			return std::string(html);
		}
		return std::format("<font class='{}'>{}</font>", cssClass, html);
	}

	// Snaps the display cursor onto a valid, selectable item within [offset, pageEnd) if it
	// isn't already on one (e.g. the page changed, or the item it pointed to became disabled).
	int ClampCursor(MenuId id, int playerSlot, int offset, int pageEnd) {
		int cursor = g_MenuManager.GetClientMenuCursor(playerSlot);
		if (cursor >= offset && cursor < pageEnd && g_MenuManager.IsMenuItemSelectable(id, cursor)) {
			return cursor;
		}

		cursor = offset;
		while (cursor < pageEnd && !g_MenuManager.IsMenuItemSelectable(id, cursor)) {
			++cursor;
		}
		g_MenuManager.SetClientMenuCursor(playerSlot, cursor);
		return cursor;
	}

	// Renders items [offset, pageEnd), highlighting `cursor` with an inline select hint so it's
	// obvious which item pressing "select" will act on. Pads with blank lines up to
	// MenuButtonMaxItems so the footer that follows always lands on the same line, regardless of
	// how many items this particular page/menu actually has.
	std::string BuildItemList(MenuId id, int offset, int pageEnd, int cursor, uint64 held) {
		bool selectPressed = (held & g_pCoreConfig->MenuButtonKeySelect) != 0;

		std::string html;
		auto out = std::back_inserter(html);

		for (int index = offset; index < pageEnd; ++index) {
			MenuItemStyle style = g_MenuManager.GetMenuItemStyle(id, index);
			if (style == MenuItemStyle::Spacer) {
				html += "<br>";
				continue;
			}

			plg::string display = g_MenuManager.GetMenuItemDisplay(id, index);
			if (index == cursor) {
				std::format_to(out, "<font color='{}'>&gt; {}</font> {}<br>", g_pCoreConfig->MenuHighlightColor, display,
					ButtonLabel(g_pCoreConfig->MenuButtonImageSelect, selectPressed));
			} else if (style == MenuItemStyle::Disabled) {
				std::format_to(out, "<font color='{}'>{}</font><br>", g_pCoreConfig->MenuDisabledColor, display);
			} else {
				std::format_to(out, "{}<br>", display);
			}
		}

		for (int shown = pageEnd - offset; shown < g_pCoreConfig->MenuButtonMaxItems; ++shown) {
			html += "<br>";
		}

		return html;
	}

	// Renders the Up/Down/Left/Right/Exit control row. The images are fixed-width, so a missing
	// prev/next slot is padded with filler instead of omitted, keeping the footer's alignment
	// consistent regardless of which controls are actually active.
	std::string BuildFooter(MenuId id, int playerSlot, uint64 held) {
		auto isHeld = [&](InputBitMask_t button) { return (held & button) != 0; };

		std::string html;
		html += ButtonLabel(g_pCoreConfig->MenuButtonImageUp, isHeld(g_pCoreConfig->MenuButtonKeyUp));
		html += ButtonLabel(g_pCoreConfig->MenuButtonImageDown, isHeld(g_pCoreConfig->MenuButtonKeyDown));

		bool hasPrevPage = g_MenuManager.ClientMenuHasPrevPage(playerSlot);
		bool hasNextPage = g_MenuManager.ClientMenuHasNextPage(playerSlot);
		bool showExit = g_MenuManager.GetMenuExitBackButton(id) || g_MenuManager.GetMenuExitButton(id);

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

		return html;
	}

	void ButtonMenu_Display(MenuId id, int playerSlot) {
		CPlayerSlot slot(playerSlot);

		int count = g_MenuManager.GetMenuItemCount(id);
		int offset = g_MenuManager.GetClientMenuOffset(playerSlot);
		int perPage = g_MenuManager.GetMenuPagination(id);
		int pageEnd = perPage > 0 ? std::min(offset + perPage, count) : count;
		int cursor = ClampCursor(id, playerSlot, offset, pageEnd);
		uint64 held = s_state[static_cast<size_t>(playerSlot)].lastHeld;

		std::string html = TitleHtml(g_MenuManager.GetMenuTitle(id));
		html += ApplyFontClass(BuildItemList(id, offset, pageEnd, cursor, held), g_pCoreConfig->MenuButtonBodyFontClass);
		html += ApplyFontClass(BuildFooter(id, playerSlot, held), g_pCoreConfig->MenuButtonFooterFontStyle);
		html += "<br> ";

		utils::PrintHtmlCentre(slot, html, g_pCoreConfig->MenuButtonHtmlDuration);
	}

	void ButtonMenu_Close(MenuId, int playerSlot) {
		utils::PrintHtmlCentre(playerSlot, " ", 1);

		ButtonMenuState& state = s_state[static_cast<size_t>(playerSlot)];
		if (state.frozen) {
			state.frozen = false;
			auto* player = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
			auto* pawn = player ? static_cast<CPlayerPawn*>(player->GetPlayerPawn()) : nullptr;
			if (pawn) {
				pawn->SetSpeed(state.savedSpeed);
			}
		}

		state.id = 0;
	}

	// Plays a configured menu sound event to the client, or does nothing if the event name is empty.
	void PlayMenuSound(int playerSlot, const plg::string& soundName) {
		g_pEngineServer->ClientCommand(playerSlot, "play %s", soundName.c_str());
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

	// Jumps the cursor a full page at a time by repeating single MoveCursor steps, so page shifts
	// and disabled-item skipping stay correct; stops early at either boundary. Returns true if the
	// cursor moved at all, i.e. this is the "move faster than Up/Down" page-jump, not a raw window
	// shift — MenuManager's own MenuNextPage/MenuPrevPage move the display window without touching
	// the cursor, which isn't what a player pressing Left/Right expects.
	bool JumpCursor(MenuId id, int playerSlot, int direction, int steps) {
		bool moved = false;
		for (int i = 0; i < steps; ++i) {
			if (!MoveCursor(id, playerSlot, direction)) {
				break;
			}
			moved = true;
		}
		return moved;
	}

	void ButtonMenu_Frame(MenuId id, int playerSlot) {
		auto* player = g_PlayerManager.ToPlayer(CPlayerSlot(playerSlot));
		auto* pawn = player ? static_cast<CPlayerPawn*>(player->GetPlayerPawn()) : nullptr;
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
			int maxItems = g_pCoreConfig->MenuButtonMaxItems;
			if (perPage <= 0 ? itemCount > maxItems : perPage > maxItems) {
				g_MenuManager.SetMenuPagination(id, maxItems);
			}

			if (g_pCoreConfig->MenuButtonFreezePlayer) {
				state.savedSpeed = pawn->GetSpeed();
				state.frozen = true;
				pawn->SetSpeed(0.0f);
			} else {
				state.frozen = false;
			}
		} else if (state.frozen && pawn->GetSpeed() != 0.0f) {
			// Something outside our control (e.g. a respawn on round start) reset the
			// client's speed mid-session; re-save whatever it was just set to and re-freeze,
			// since the one-shot freeze above only fires at session start.
			state.savedSpeed = pawn->GetSpeed();
			pawn->SetSpeed(0.0f);
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
				if (JumpCursor(id, playerSlot, -1, g_pCoreConfig->MenuButtonMaxItems)) {
					PlayMenuSound(playerSlot, g_pCoreConfig->MenuSoundScroll);
				}
			} else if (released(g_pCoreConfig->MenuButtonKeyRight)) {
				if (JumpCursor(id, playerSlot, 1, g_pCoreConfig->MenuButtonMaxItems)) {
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
