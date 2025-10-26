#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/entity/cplayercontroller.h>
#include <core/sdk/entity/cplayerpawn.h>
#include <core/sdk/utils.hpp>
#include <core/sdk/cvars.hpp>
#include <eiface.h>
#include <inetchannelinfo.h>
#include <iserver.h>
#include <netmessages.h>

#include "player_manager.hpp"
#include "listeners.hpp"

PlayerManager g_PlayerManager;

constexpr int CLIENT_LANGUAGE_ID = INT_MAX;
constexpr int CLIENT_OPERATING_SYSTEMID = INT_MAX - 1;

CBasePlayerController* Player::GetController() const {
	auto entIndex = GetEntityIndex();
	CBaseEntity* entity = static_cast<CBaseEntity*>(g_pGameEntitySystem->GetEntityInstance(entIndex));
	if (!entity) {
		return nullptr;
	}

	return entity->IsPlayerController() ? static_cast<CPlayerController*>(entity) : nullptr;
}

CBasePlayerPawn* Player::GetCurrentPawn() const {
	auto controller = GetController();
	return controller ? controller->GetCurrentPawn() : nullptr;
}

CBasePlayerPawn* Player::GetPlayerPawn() const {
	auto controller = GetController();
	return controller ? reinterpret_cast<CPlayerController*>(controller)->GetPlayerPawn() : nullptr;
}

CBasePlayerPawn* Player::GetObserverPawn() const {
	auto controller = GetController();
	return controller ? reinterpret_cast<CPlayerController*>(controller)->GetObserverPawn() : nullptr;
}

CServerSideClientBase* Player::GetClient() const {
	return utils::GetClientBySlot(GetPlayerSlot());
}

bool Player::IsAuthenticated() const {
	auto client = GetClient();
	return client && client->IsConnected() && !client->IsFakeClient() && g_pEngineServer->IsClientFullyAuthenticated(GetPlayerSlot());
}

bool Player::IsConnected() const {
	auto client = GetClient();
	return client && client->IsConnected();
}

bool Player::IsInGame() const {
	auto client = GetClient();
	return client && client->IsActive();
}

bool Player::IsFakeClient() const {
	auto client = GetClient();
	return client && client->IsFakeClient();
}

bool Player::IsSourceTV() const {
	auto client = GetClient();
	return client && client->IsHLTV();
}

bool Player::IsAlive() const {
	auto pawn = GetPlayerPawn();
	return pawn && pawn->IsAlive();
}

bool Player::IsValidClient() const {
	if (!utils::IsPlayerSlot(GetPlayerSlot())) {
		return false;
	}

	CServerSideClientBase* client = GetClient();
	if (!client) {
		return false;
	}

	auto controller = utils::GetController(client->GetPlayerSlot());
	return controller && controller->IsPlayerController() && client->IsConnected() && client->IsInGame() && !client->IsHLTV();
}

std::string_view Player::GetName() const {
	auto client = GetClient();
	return client ? client->GetClientName() : "<unknown>";
}

std::string_view Player::GetIpAddress() const {
	auto client = GetClient();
	return client ? client->GetNetChannel()->GetRemoteAddress().ToString(true) : "<unknown>";
}

std::string_view Player::GetLanguage() const {
	return m_language;
}

std::string_view Player::GetOperatingSystem() const {
	return m_operatingSystem;
}

CSteamID Player::GetSteamId(bool validated) const {
	bool authenticated = IsAuthenticated();
	if (validated && !authenticated) {
		return k_steamIDNil;
	}

	CServerSideClientBase* client = GetClient();
	if (client && authenticated) {
		return client->GetClientSteamID();
	}

	return m_unauthenticatedSteamID;
}

INetChannelInfo* Player::GetNetInfo() const {
	return g_pEngineServer->GetPlayerNetInfo(m_slot);
}

float Player::GetTimeConnected() const {
	if (!IsConnected() || IsFakeClient()) {
		return 0;
	}

	return GetNetInfo()->GetTimeConnected();
}

float Player::GetLatency() const {
	return GetNetInfo()->GetAvgLatency();
}

void Player::Kick(const char* internalReason, ENetworkDisconnectionReason reason) const {
	g_pEngineServer->KickClient(GetPlayerSlot(), internalReason, reason);
}

void Player::QueryCvar(int queryCvarCookie, CvarQuery query) {
	m_queryCallback[queryCvarCookie] = std::move(query);
}

void Player::OnRepondCvarValue(const CCLCMsg_RespondCvarValue_t& msg) {
	switch (msg.cookie()) {
		case CLIENT_LANGUAGE_ID:
			m_language = msg.value();
			break;

		case CLIENT_OPERATING_SYSTEMID:
			m_operatingSystem = msg.value();
			break;

		default: {
			auto it = m_queryCallback.find(msg.cookie());
			if (it != m_queryCallback.end()) {
				auto& [callback, data] = it->second;
				callback(m_slot, msg.cookie(), static_cast<CvarValueStatus>(msg.status_code()), msg.name(), msg.value(), data);
				m_queryCallback.erase(it);
			}
			break;
		}
	}
}

void PlayerManager::OnSteamAPIActivated() {
	if (m_callbackRegistered)
		return;

	m_callbackRegistered = true;
	m_CallbackValidateAuthTicketResponse.Register(this, &PlayerManager::OnValidateAuthTicket);
}

void PlayerManager::OnValidateAuthTicket(ValidateAuthTicketResponse_t* response) {
	if (response->m_eAuthSessionResponse != k_EAuthSessionResponseOK)
		return;

	for (const Player& player : m_players) {
		CSteamID steamID = player.GetSteamId();
		if (steamID == response->m_SteamID) {
			GetOnClientAuthenticatedListenerManager()(player.GetPlayerSlot(), steamID.GetAccountID());
			return;
		}
	}
}

thread_local bool s_refuseConnection;

bool PlayerManager::OnClientConnect(CPlayerSlot slot, char const* name, uint64 steamID64, char const* networkID) {
	if (Player* player = ToPlayer(slot)) {
		player->Init(slot, steamID64);

		s_refuseConnection = false;

		plg::string playerName(name);
		auto funcs = GetOnClientConnectListenerManager().Get();
		for (const auto& func : funcs) {
			auto res = func(slot, playerName, networkID);
			s_refuseConnection |= !res;
		}

		return s_refuseConnection;
	}

	return true;
}

bool PlayerManager::OnClientConnect_Post(CPlayerSlot slot, bool origRet) {
	if (Player* player = ToPlayer(slot)) {
		if (s_refuseConnection) {
			origRet = false;
		}

		if (origRet) {
			GetOnClientConnect_PostListenerManager()(slot);
		} else {
			player->Reset();
		}
	}

	return origRet;
}

void PlayerManager::OnClientConnected(CPlayerSlot slot, bool fakePlayer) {
	if (!fakePlayer) {
		cvars::SendConVarValueQueryToClient(slot, "cl_language", CLIENT_LANGUAGE_ID);
		cvars::SendConVarValueQueryToClient(slot, "engine_ostype", CLIENT_OPERATING_SYSTEMID);
	}
	GetOnClientConnectedListenerManager()(slot);
}

void PlayerManager::OnClientPutInServer(CPlayerSlot slot, char const* name) {
	if (Player* player = ToPlayer(slot)) {
		// For bots only
		if (player->GetPlayerSlot() == INVALID_PLAYER_SLOT) {
			player->Init(slot, 0);
		}

		GetOnClientPutInServerListenerManager()(slot);
	}
}

void PlayerManager::OnClientDisconnect(CPlayerSlot slot, ENetworkDisconnectionReason reason) {
	GetOnClientDisconnectListenerManager()(slot, reason);
}

void PlayerManager::OnClientDisconnect_Post(CPlayerSlot slot, ENetworkDisconnectionReason reason) {
	GetOnClientDisconnect_PostListenerManager()(slot, reason);

	if (Player* player = ToPlayer(slot)) {
		player->Reset();
	}
}

void PlayerManager::OnClientActive(CPlayerSlot slot, bool loadGame) const {
	GetOnClientActiveListenerManager()(slot, loadGame);
}

bool PlayerManager::QueryCvarValue(CPlayerSlot slot, std::string_view convarName, CvarValueCallback callback, const plg::vector<plg::any>& data) {
	if (Player* player = ToPlayer(slot)) {
		int queryCvarCookie = cvars::SendConVarValueQueryToClient(slot, convarName);
		if (queryCvarCookie != -1) {
			std::scoped_lock lock(m_mutex);
			player->QueryCvar(queryCvarCookie, CvarQuery{callback, data});
			return true;
		}
	}

	return false;
}

void PlayerManager::OnRespondCvarValue(CServerSideClientBase* client, const CCLCMsg_RespondCvarValue_t& msg) {
	if (Player* player = ToPlayer(client)) {
		player->OnRepondCvarValue(msg);
	}
}

int PlayerManager::MaxClients() {
	return gpGlobals ? gpGlobals->maxClients : -1;
}

Player* PlayerManager::ToPlayer(CServerSideClientBase* client) const {
	return ToPlayer(client->GetPlayerSlot());
}

Player* PlayerManager::ToPlayer(CPlayerPawnComponent* component) const {
	return ToPlayer(component->GetPawn());
}

Player* PlayerManager::ToPlayer(CBasePlayerController* controller) const {
	if (!controller) {
		return nullptr;
	}

	return ToPlayer(CPlayerSlot(controller->GetPlayerSlot()));
}

Player* PlayerManager::ToPlayer(CBasePlayerPawn* pawn) const {
	if (!pawn) {
		return nullptr;
	}

	CBasePlayerController* controller = pawn->GetController();
	if (!controller) {
		return nullptr;
	}

	return ToPlayer(controller);
}

Player* PlayerManager::ToPlayer(CPlayerSlot slot) const {
	if (utils::IsPlayerSlot(slot)) {
		return const_cast<Player*>(&m_players.at(slot));
	}

	return nullptr;
}

Player* PlayerManager::ToPlayer(CEntityIndex entIndex) const {
	CBaseEntity* entity = static_cast<CBaseEntity*>(GameEntitySystem()->GetEntityInstance(entIndex));
	if (!entity) {
		return nullptr;
	}

	if (entity->IsPlayerPawn()) {
		return ToPlayer(static_cast<CBasePlayerPawn*>(entity));
	}

	if (entity->IsPlayerController()) {
		return ToPlayer(static_cast<CBasePlayerController*>(entity));
	}

	return nullptr;
}

Player* PlayerManager::ToPlayer(CPlayerUserId userID) const {
	for (int slot = 0; slot < MaxClients(); slot++) {
		if (g_pEngineServer->GetPlayerUserId(slot) == userID) {
			return ToPlayer(CPlayerSlot(slot));
		}
	}

	return nullptr;
}

Player* PlayerManager::ToPlayer(CSteamID steamid, bool validate) const {
	auto steam64 = steamid.ConvertToUint64();
	for (auto& player: GetOnlinePlayers()) {
		if (player->GetSteamId64(validate) == steam64) {
			return player;
		}
	}

	return nullptr;
}

std::inplace_vector<Player*, MAXPLAYERS> PlayerManager::GetOnlinePlayers() const {
	std::inplace_vector<Player*, MAXPLAYERS> players;
	for (const auto& player : m_players) {
		if (utils::IsPlayerSlot(player.GetPlayerSlot())) {
			players.emplace_back(const_cast<Player*>(&player));
		}
	}

	return players;
}

TargetType PlayerManager::TargetPlayerString(int caller, std::string_view target, plg::vector<int>& clients) {
	TargetType targetType = TargetType::NONE;

	if (target == "@me")
		targetType = TargetType::SELF;
	else if (target == "@all")
		targetType = TargetType::ALL;
	else if (target == "@t")
		targetType = TargetType::T;
	else if (target == "@ct")
		targetType = TargetType::CT;
	else if (target == "@spec")
		targetType = TargetType::SPECTATOR;
	else if (target == "@random")
		targetType = TargetType::RANDOM;
	else if (target == "@randomt")
		targetType = TargetType::RANDOM_T;
	else if (target == "@randomct")
		targetType = TargetType::RANDOM_CT;

	clients.clear();

	if (targetType == TargetType::SELF && caller != -1) {
		clients.emplace_back(caller);
	} else if (targetType == TargetType::ALL) {
		for (int i = 0; i < MaxClients(); ++i) {
			if (!m_players[i].IsConnected())
				continue;

			CBasePlayerController* player = utils::GetController(i);

			if (!player || !player->IsPlayerController() || !player->IsConnected())
				continue;

			clients.emplace_back(i);
		}
	} else if (targetType >= TargetType::SPECTATOR) {
		for (int i = 0; i < MaxClients(); ++i) {
			if (!m_players[i].IsConnected())
				continue;

			CBasePlayerController* player = utils::GetController(i);

			if (!player || !player->IsPlayerController() || !player->IsConnected())
				continue;

			if (player->m_iTeamNum != (targetType == TargetType::T ? CS_TEAM_T : targetType == TargetType::CT ? CS_TEAM_CT : CS_TEAM_SPECTATOR))
				continue;

			clients.emplace_back(i);
		}
	} else if (targetType >= TargetType::RANDOM && targetType <= TargetType::RANDOM_CT) {
		int attempts = 0;

		while (clients.empty() == 0 && attempts < 10000) {
			int i = rand() % (MaxClients() - 1);

			// Prevent infinite loop
			attempts++;

			if (!m_players[i].IsConnected())
				continue;

			CBasePlayerController* player = utils::GetController(i);

			if (!player || !player->IsPlayerController() || !player->IsConnected())
				continue;

			if (targetType >= TargetType::RANDOM_T && (player->m_iTeamNum != (targetType == TargetType::RANDOM_T ? CS_TEAM_T : CS_TEAM_CT)))
				continue;

			clients.emplace_back(i);
		}
	} else if (target.starts_with('#')) {
		if (auto slot = plg::cast_to<int>(target.substr(1))) {
			targetType = TargetType::PLAYER;
			CBasePlayerController* player = utils::GetController(*slot);
			if (player && player->IsPlayerController() && player->IsConnected()) {
				clients.emplace_back(*slot);
			}
		}
	} else {
		for (int i = 0; i < MaxClients(); ++i) {
			if (!m_players[i].IsConnected())
				continue;

			CBasePlayerController* player = utils::GetController(i);

			if (!player || !player->IsPlayerController() || !player->IsConnected())
				continue;

			if (target.find(player->GetPlayerName()) != std::string_view::npos) {
				targetType = TargetType::PLAYER;
				clients.emplace_back(i);
				break;
			}
		}
	}

	return targetType;
}