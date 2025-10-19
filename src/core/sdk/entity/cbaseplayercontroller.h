/**
  * =============================================================================
 * s2sdk
 * Copyright (C) 2023-2025 untrustedmodders
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "cbaseentity.h"
#include "cbaseplayerpawn.h"
#include <ehandle.h>

enum class PlayerConnectedState : uint32_t {
	PlayerNeverConnected = 0xFFFFFFFF,
	PlayerConnected = 0x0,
	PlayerConnecting = 0x1,
	PlayerReconnecting = 0x2,
	PlayerDisconnecting = 0x3,
	PlayerDisconnected = 0x4,
	PlayerReserved = 0x5,
};

class CBasePlayerController : public CBaseEntity {
public:
	DECLARE_SCHEMA_CLASS(CBasePlayerController);

	SCHEMA_FIELD(uint64, m_steamID)
	SCHEMA_FIELD(CHandle<CBasePlayerPawn>, m_hPawn)
	SCHEMA_FIELD_POINTER(char, m_iszPlayerName)
	SCHEMA_FIELD(PlayerConnectedState, m_iConnected)
	SCHEMA_FIELD(bool, m_bIsHLTV)
	SCHEMA_FIELD(uint, m_iDesiredFOV)

	CBasePlayerPawn* GetCurrentPawn() { return *m_hPawn; }
	const char* GetPlayerName() { return m_iszPlayerName; }
	int GetPlayerSlot() { return entindex() - 1; }
	bool IsConnected() { return m_iConnected == PlayerConnectedState::PlayerConnected; }

	void SetPawn(CBasePlayerPawn* pawn) {
		addresses::CBasePlayerController_SetPawn(this, pawn, true, false, false);
	}
};
