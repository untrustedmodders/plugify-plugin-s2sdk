#pragma once

#include <bitvec.h>
#include <network_connection.pb.h>
#include <serversideclient.h>
#include <steam/isteamuser.h>
#include <steam/steam_api_common.h>
#include <steam/steamclientpublic.h>

enum class TargetType {
	NONE,
	PLAYER,
	SELF,
	RANDOM,
	RANDOM_T,
	RANDOM_CT,
	ALL,
	SPECTATOR,
	T,
	CT,
};

class CPlayerPawnComponent;
class CCSObserverPawn;

enum class CvarValueStatus {
	ValueIntact = 0,	// It got the value fine.
	CvarNotFound = 1,
	NotACvar = 2,		// There's a ConCommand, but it's not a ConVar.
	CvarProtected = 3	// The cvar was marked with FCVAR_SERVER_CAN_NOT_QUERY, so the server is not allowed to have its value.
};

using CvarValueCallback = void(*)(CPlayerSlot slot, int cookie, CvarValueStatus code, const plg::string& name, const plg::string& value, const plg::vector<plg::any>& data);

struct CvarQuery {
	CvarValueCallback callback;
	plg::vector<plg::any> data;
};

class Player {
public:
	Player() = default;
	~Player() = default;

	void Init(int slot, uint64 steamID64) {
		m_slot = slot;
		m_unauthenticatedSteamID = CSteamID(steamID64);
		m_language.clear();
		m_operatingSystem.clear();
		m_queryCallback.clear();
	}

	void Reset() {
		m_slot = -1;
		m_unauthenticatedSteamID = k_steamIDNil;
		m_language.clear();
		m_operatingSystem.clear();
		m_queryCallback.clear();
	}

	CBasePlayerController* GetController() const;
	CBasePlayerPawn* GetCurrentPawn() const;
	CBasePlayerPawn* GetPlayerPawn() const;
	CBasePlayerPawn* GetObserverPawn() const;
	CServerSideClientBase* GetClient() const;

	// Returns true if this client has been fully authenticated by Steam
	bool IsAuthenticated() const;
	bool IsConnected() const;
	bool IsInGame() const;
	bool IsFakeClient() const;
	bool IsSourceTV() const;
	bool IsAlive() const;
	bool IsValidClient() const;
	std::string_view GetName() const;
	std::string_view GetIpAddress() const;
	std::string_view GetLanguage() const;
	std::string_view GetOperatingSystem() const;
	CSteamID GetSteamId(bool validated = false) const;
	INetChannelInfo* GetNetInfo() const;
	float GetTimeConnected() const;
	float GetLatency() const;

	void Kick(const char* internalReason, ENetworkDisconnectionReason reason) const;

	uint64 GetSteamId64(bool validated = false) const {
		return GetSteamId(validated).ConvertToUint64();
	}

	CPlayerSlot GetPlayerSlot() const {
		return m_slot;
	}

	CEntityIndex GetEntityIndex() const {
		return m_slot + 1;
	}

	void QueryCvar(int queryCvarCookie, CvarQuery query);
	void OnRepondCvarValue(const CCLCMsg_RespondCvarValue_t& msg);

private:
	int m_slot{-1};
	CSteamID m_unauthenticatedSteamID{k_steamIDNil};
	plg::string m_language;
	plg::string m_operatingSystem;
	plg::flat_hash_map<int, CvarQuery> m_queryCallback;
};

class PlayerManager {
public:
	PlayerManager() = default;
	~PlayerManager() = default;

	Player* ToPlayer(CServerSideClientBase* client) const;
	Player* ToPlayer(CPlayerPawnComponent* component) const;
	Player* ToPlayer(CBasePlayerController* controller) const;
	Player* ToPlayer(CBasePlayerPawn* pawn) const;
	Player* ToPlayer(CPlayerSlot slot) const;
	Player* ToPlayer(CEntityIndex entIndex) const;
	Player* ToPlayer(CPlayerUserId userID) const;
	Player* ToPlayer(CSteamID steamid, bool validate = false) const;

	void OnSteamAPIActivated();
	bool OnClientConnect(CPlayerSlot slot, char const* name, uint64 steamID64, char const* networkID);
	bool OnClientConnect_Post(CPlayerSlot slot, bool origRet);
	void OnClientConnected(CPlayerSlot slot, bool fakePlayer);
	void OnClientPutInServer(CPlayerSlot slot, char const* name);
	void OnClientDisconnect(CPlayerSlot slot, ENetworkDisconnectionReason reason);
	void OnClientDisconnect_Post(CPlayerSlot slot, ENetworkDisconnectionReason reason);
	void OnClientActive(CPlayerSlot slot, bool loadGame) const;

	bool QueryCvarValue(CPlayerSlot slot, std::string_view convarName, CvarValueCallback callback, const plg::vector<plg::any>& data);
	void OnRespondCvarValue(CServerSideClientBase* client, const CCLCMsg_RespondCvarValue_t& msg);

	STEAM_GAMESERVER_CALLBACK_MANUAL(PlayerManager, OnValidateAuthTicket, ValidateAuthTicketResponse_t, m_CallbackValidateAuthTicketResponse);

	std::vector<Player*> GetOnlinePlayers() const;
	static int MaxClients();

	TargetType TargetPlayerString(int caller, std::string_view target, plg::vector<int>& clients);

protected:
	std::array<Player, MAXPLAYERS + 1> m_players{};
	std::mutex m_mutex;
	bool m_callbackRegistered{};
};

extern PlayerManager g_PlayerManager;