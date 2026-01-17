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

#include "cbaseplayercontroller.h"
#include "ccitadelplayerpawn.h"
#include <sharedInterface.h>

class CCitadelPlayerController : public CBasePlayerController {
public:
	DECLARE_SCHEMA_CLASS(CCitadelPlayerController);

	SCHEMA_FIELD(CHandle<CCitadelPlayerPawn>, m_hHeroPawn)

	CCSPlayerController_InGameMoneyServices* m_pInGameMoneyServices() {
		return nullptr;
	}
	CCSPlayerController_ActionTrackingServices* m_pActionTrackingServices() {
		return nullptr;
	}

	// Returns the actual player pawn
	CCitadelPlayerPawn* GetPlayerPawn() {
		return *m_hHeroPawn;
	}

	CCitadelPlayerPawn* GetObserverPawn() {
		return nullptr;
	}

	/*CServerSideClient* GetServerSideClient() {
		return utils::GetClientBySlot(GetPlayerSlot());
	}*/

	bool IsBot() {
		return m_fFlags & FL_CONTROLLER_FAKECLIENT;
	}

	void ChangeTeam(int iTeam) {
		// TODO:
	}

	void SwitchTeam(CSTeam iTeam) {
		// TODO:
	}

	// Respawns the player if the player is not alive, does nothing otherwise.
	void Respawn() {
		// TODO:
	}
};


