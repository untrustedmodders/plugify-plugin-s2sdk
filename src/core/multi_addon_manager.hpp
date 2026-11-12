#pragma once
#if defined (CS2)
#include <igameevents.h>
#include <utlqueue.h>
#include <utlvector.h>
#include <networksystem/inetworkserializer.h>
#include <steam/steam_api_common.h>
#include <steam/isteamugc.h>
#include <steam/isteamuser.h>

class CServerSideClient;
class CServerSideClientBase;

class MultiAddonManager {
	MultiAddonManager() = default;
	~MultiAddonManager() = default;
	NONCOPYABLE(MultiAddonManager)

	static MultiAddonManager instance;
public:
	static auto& Instance() noexcept {
		return instance;
	}

	std::string BuildAddonPath(PublishedFileId_t addon);
	bool MountAddon(PublishedFileId_t addon, bool addToTail = false);
	bool UnmountAddon(PublishedFileId_t addon, bool remove = true);
	bool AddAddon(PublishedFileId_t addon, bool refresh = false);
	bool RemoveAddon(PublishedFileId_t addon, bool refresh = false);
	bool DownloadAddon(PublishedFileId_t addon, bool important = false, bool force = false);
	void PrintDownloadProgress() const;
	void RefreshAddons(bool reloadMap = false);
	plg::vector<PublishedFileId_t> GetServerAddons() const;
	void ClearAddons();
	void ReloadMap();
	void RestoreCurrentWorkshopMap(std::string_view map);

	PublishedFileId_t GetCurrentWorkshopMap() const { return m_currentWorkshopMap; }
	void SetCurrentWorkshopMap(PublishedFileId_t workshopID) { m_currentWorkshopMap = workshopID; }
	void ClearCurrentWorkshopMap() { m_currentWorkshopMap = k_PublishedFileIdInvalid; }

	static bool HasUGCConnection();
	bool AddPrivateAddon(PublishedFileId_t addon, bool refresh = false);
	bool RemovePrivateAddon(PublishedFileId_t addon, bool refresh = false);
	bool AddClientAddon(PublishedFileId_t addon, uint64 steamID64 = 0, bool refresh = false);
	bool RemoveClientAddon(PublishedFileId_t addon, uint64 steamID64 = 0);
	void ClearClientAddons(uint64 steamID64 = 0);
	plg::vector<PublishedFileId_t> GetClientAddons(uint64 steamID64 = 0);
	bool IsClientAddonAllowed(PublishedFileId_t addon, uint64 steamID64);
	plg::vector<PublishedFileId_t> GetSentAddons(uint64 steamID64);
	void SetConVarSentAddons(std::string_view value);

public:
	void OnPluginStart();
	void OnPluginEnd();
	void OnSteamAPIActivated(bool reloadMap = true);
	void OnStartupServer();
	void OnClientConnect(uint64 steamID64);
	void OnClientDisconnect(CPlayerSlot slot, const char* name, uint64 steamID64, const char* networkID, ENetworkDisconnectionReason reason);
	void OnGameFrame();
	void OnPostEvent(INetworkMessageInternal* message, CNetMessage* data, uint64_t* clients);

	void OnSendNetMessage(CServerSideClientBase* client, CNetMessage* data, NetChannelBufType_t bufType);
	void OnSerializeMessage(const CNetMessage* data);
	void OnHostStateRequest(CHostStateMgr* manager, CHostStateRequest* request);
	void OnSwitchToLoop(const char* loopModeName);
	bool OnReplyConnection(CNetworkGameServerBase* server, CServerSideClient* client);
	void OnReplyConnection_Post(CNetworkGameServerBase* server, CServerSideClient* client);
	uint64 OnScriptGetAddon();

public:
	plg::vector<PublishedFileId_t> m_extraAddons;
	plg::vector<PublishedFileId_t> m_mountedAddons;
	plg::vector<PublishedFileId_t> m_globalClientAddons;
	plg::vector<PublishedFileId_t> m_privateAddons; // Mounted like extra addons, but only sent with SendAddon or s2_sent_addons

private:
	plg::vector<PublishedFileId_t> m_importantDownloads; // Important addon downloads that will trigger a map reload when finished
	struct AddonDownloadInfo {
		PublishedFileId_t addon;
		bool important;
		bool force;
	};
	plg::vector<AddonDownloadInfo> m_preSteamLoginDownloads; // Downloads that failed to start because the server wasn't logged on to Steam yet
	std::deque<PublishedFileId_t> m_downloadQueue; // Queue of all addon downloads to print progress
	std::set<uint64> m_timedOutClients;
	std::map<uint64, plg::vector<PublishedFileId_t>> m_conVarSentAddons; // Addons sent to clients by s2_sent_addons

	STEAM_GAMESERVER_CALLBACK_MANUAL(MultiAddonManager, OnAddonDownloaded, DownloadItemResult_t, m_CallbackDownloadItemResult);
	STEAM_GAMESERVER_CALLBACK_MANUAL(MultiAddonManager, OnSteamServersConnected, SteamServersConnected_t, m_CallbackSteamServersConnected);

	// Set when the host state request is made, becomes current once the level actually starts loading
	PublishedFileId_t m_loadingWorkshopMap{k_PublishedFileIdInvalid};
	// Used when reloading current map
	PublishedFileId_t m_currentWorkshopMap{k_PublishedFileIdInvalid};
};
inline MultiAddonManager& g_MultiAddonManager = MultiAddonManager::Instance();

#endif