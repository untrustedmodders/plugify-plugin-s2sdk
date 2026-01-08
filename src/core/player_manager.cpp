#include <core/sdk/cvars.hpp>
#include <core/sdk/entity/cbaseplayercontroller.h>
#include <core/sdk/entity/cplayercontroller.h>
#include <core/sdk/entity/cplayerpawn.h>
#include <core/sdk/utils.hpp>
#include <eiface.h>
#include <inetchannelinfo.h>
#include <iserver.h>
#include <netmessages.h>
#include <isteamgameserver.h>

#include "player_manager.hpp"
#include "listeners.hpp"

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

void PlayerManager::OnGameFrame() {
	static double s_nextUpdate = 0.0;

	double currentTime = Plat_FloatTime();
	if (currentTime > s_nextUpdate) {
		UpdatePlayers();

		s_nextUpdate = currentTime + 5.0;
	}
}

void PlayerManager::OnSteamAPIActivated() {
	if (m_callbackRegistered)
		return;

	m_callbackRegistered = true;
	m_CallbackValidateAuthTicketResponse.Register(this, &PlayerManager::OnValidateAuthTicket);
}

void PlayerManager::OnSteamAPIDeactivated() {
	if (!m_callbackRegistered)
		return;

	m_CallbackValidateAuthTicketResponse.Unregister();
	m_callbackRegistered = false;
}

void PlayerManager::OnValidateAuthTicket(ValidateAuthTicketResponse_t* response) {
	if (response->m_eAuthSessionResponse != k_EAuthSessionResponseOK)
		return;

	for (const Player& player : m_players) {
		CSteamID steamID = player.GetSteamId();
		if (steamID == response->m_SteamID) {
			g_OnClientAuthenticatedListenerManager(player.GetPlayerSlot(), steamID.ConvertToUint64());
			return;
		}
	}
}

thread_local bool s_refuseConnection;

bool PlayerManager::OnClientConnect(CPlayerSlot slot, char const* name, uint64 steamID64, char const* networkID) {
	if (Player* player = ToPlayer(slot)) {
		player->Init(slot, steamID64);

		cvars::SendConVarValueQueryToClient(slot, "cl_language", CLIENT_LANGUAGE_ID);
		cvars::SendConVarValueQueryToClient(slot, "engine_ostype", CLIENT_OPERATING_SYSTEMID);

		s_refuseConnection = false;

		plg::string playerName(name);
		auto funcs = g_OnClientConnectListenerManager.Get();
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
			g_OnClientConnect_PostListenerManager(slot);
		} else {
			player->Reset();
		}
	}

	return origRet;
}

void PlayerManager::OnClientConnected(CPlayerSlot slot, bool fakePlayer) {
	g_OnClientConnectedListenerManager(slot);
}

void PlayerManager::OnClientPutInServer(CPlayerSlot slot, char const* name) {
	if (Player* player = ToPlayer(slot)) {
		// For bots only
		if (player->GetPlayerSlot() == INVALID_PLAYER_SLOT) {
			player->Init(slot, 0);
		}

		g_OnClientPutInServerListenerManager(slot);
	}
}

void PlayerManager::OnClientDisconnect(CPlayerSlot slot, ENetworkDisconnectionReason reason) {
	g_OnClientDisconnectListenerManager(slot, reason);
}

void PlayerManager::OnClientDisconnect_Post(CPlayerSlot slot, ENetworkDisconnectionReason reason) {
	g_OnClientDisconnect_PostListenerManager(slot, reason);

	if (Player* player = ToPlayer(slot)) {
		player->Reset();
	}
}

void PlayerManager::OnClientActive(CPlayerSlot slot, bool loadGame) const {
	g_OnClientActiveListenerManager(slot, loadGame);
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

void PlayerManager::UpdatePlayers() {
	if(!gpGlobals)
		return;

	for (int i = 0; i < gpGlobals->maxClients; ++i) {
		if (auto steamId = g_pEngineServer->GetClientSteamID(i)) {
			if (CPlayerController* controller = static_cast<CPlayerController*>(utils::GetController(i))) {
				g_pSteam->BUpdateUserData(*steamId, controller->GetPlayerName(), g_pSource2GameClients->GetPlayerScore(i));
			}
		}
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
	for (const auto& player : m_players) {
		if (utils::IsPlayerSlot(player.GetPlayerSlot()) && player.GetSteamId(validate) == steamid) {
			return const_cast<Player*>(&player);
		}
	}

	return nullptr;
}

std::optional<CSTeam> PlayerManager::GetTeamForTargetType(TargetType targetType) {
    switch (targetType) {
        case TargetType::T:
        case TargetType::RANDOM_T:
            return CSTeam::T;
        case TargetType::CT:
        case TargetType::RANDOM_CT:
            return CSTeam::CT;
        case TargetType::SPECTATOR:
            return CSTeam::Spectator;
        default:
            return std::nullopt;
    }
}

plg::vector<int> PlayerManager::CollectAllPlayers() const {
	std::inplace_vector<int, MaxPlayers> clients;
	for (const auto& player : m_players) {
		if (player.IsValidClient()) {
            clients.emplace_back(player.GetPlayerSlot());
        }
    }
	return { clients.begin(), clients.end() };
}

plg::vector<int> PlayerManager::CollectTeamPlayers(TargetType targetType) const {
    auto team = GetTeamForTargetType(targetType);
    if (!team) return {};

	std::inplace_vector<int, MaxPlayers> clients;
	for (const auto& player : m_players) {
		if (player.IsValidClient() && player.GetController()->GetTeam() == *team) {
			clients.emplace_back(player.GetPlayerSlot());
        }
    }
	return { clients.begin(), clients.end() };
}

plg::vector<int> PlayerManager::CollectRandomPlayer(TargetType targetType) const {
    std::inplace_vector<int, MaxPlayers> eligible;
    auto team = GetTeamForTargetType(targetType);

	for (const auto& player : m_players) {
		if (player.IsValidClient()) {
			if (team && player.GetController()->GetTeam() != *team)
				continue;
    		eligible.emplace_back(player.GetPlayerSlot());
    	}
    }

    if (!eligible.empty()) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, eligible.size() - 1);
        return { eligible[dist(gen)] };
    }

	return {};
}

plg::vector<int> PlayerManager::FindPlayerBySlot(std::string_view target) const {
    if (auto slot = plg::cast_to<int>(target)) {
    	if (auto player = ToPlayer(CPlayerSlot(*slot))) {
			if (player->IsValidClient()) {
				return { *slot };
			}
    	}
    }
    return {};
}

plg::vector<int> PlayerManager::FindPlayerByAccount(std::string_view target) const {
	if (auto accountId = plg::cast_to<uint32>(target)) {
		for (const auto& player : m_players) {
			if (player.IsValidClient() && player.GetSteamId().GetAccountID() == *accountId) {
				return { player.GetPlayerSlot() };
			}
		}
	}
    return {};
}

plg::vector<int> PlayerManager::FindPlayerBySteam(std::string_view target) const {
	if (auto steamId = plg::cast_to<uint64>(target)) {
		for (const auto& player : m_players) {
			if (player.IsValidClient() && player.GetSteamId() == *steamId) {
				return { player.GetPlayerSlot() };
			}
		}
	}
    return {};
}

plg::vector<int> PlayerManager::FindPlayerByName(std::string_view target) const {
	for (const auto& player : m_players) {
		if (player.IsValidClient() && player.GetName() == target) {
			return { player.GetPlayerSlot() };
    	}
    }
    return {};
}

plg::vector<int> PlayerManager::TargetPlayerString(CPlayerSlot caller, std::string_view target) const {
	using namespace std::string_view_literals;
	static constexpr auto targetMap = std::array{
		std::pair{"@me"sv, TargetType::SELF},
		std::pair{"@all"sv, TargetType::ALL},
		std::pair{"@t"sv, TargetType::T},
		std::pair{"@ct"sv, TargetType::CT},
		std::pair{"@spec"sv, TargetType::SPECTATOR},
		std::pair{"@random"sv, TargetType::RANDOM},
		std::pair{"@randomt"sv, TargetType::RANDOM_T},
		std::pair{"@randomct"sv, TargetType::RANDOM_CT}
	};
	auto targetType = TargetType::NONE;
	for (const auto& [key, value] : targetMap) {
		if (target == key) {
			targetType = value;
			break;
		}
	}

	// Early return for SELF target
	if (targetType == TargetType::SELF) {
		if (auto player = ToPlayer(caller)) {
			if (player->IsValidClient()) {
				return { caller };
			}
		}
		return {};
	}

	// Handle special cases first
	if (target.starts_with('#')) {
		return FindPlayerBySlot(target.substr(1));
	}
	if (target.starts_with('&')) {
		return FindPlayerByAccount(target.substr(1));
	}
	if (target.starts_with('$')) {
		return FindPlayerBySteam(target.substr(1));
	}

	// For other targets, gather valid players
	switch (targetType) {
		case TargetType::ALL:
			return CollectAllPlayers();

		case TargetType::T:
		case TargetType::CT:
		case TargetType::SPECTATOR:
			return CollectTeamPlayers(targetType);

		case TargetType::RANDOM:
		case TargetType::RANDOM_T:
		case TargetType::RANDOM_CT:
			return CollectRandomPlayer(targetType);

		case TargetType::NONE:
			return FindPlayerByName(target);

		default:
			break;
	}

	return {};
}
