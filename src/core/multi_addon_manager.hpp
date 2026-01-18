#pragma once
#if defined (CS2)
#include <igameevents.h>
#include <utlqueue.h>
#include <utlvector.h>
#include <networksystem/inetworkserializer.h>
#include <steam/steam_api_common.h>
#include <steam/isteamugc.h>

class CServerSideClient;

class MultiAddonManager {
	MultiAddonManager() = default;
	~MultiAddonManager() = default;
	NONCOPYABLE(MultiAddonManager)

public:
	static auto& Instance() {
		static MultiAddonManager instance;
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
	void ClearAddons();
	void ReloadMap();

	PublishedFileId_t GetCurrentWorkshopMap() const { return m_currentWorkshopMap; }
	void SetCurrentWorkshopMap(PublishedFileId_t workshopID) { m_currentWorkshopMap = workshopID; }
	void ClearCurrentWorkshopMap() { m_currentWorkshopMap = k_PublishedFileIdInvalid; }

	static bool HasUGCConnection();
	void AddClientAddon(PublishedFileId_t addon, uint64 steamID64 = 0, bool refresh = false);
	void RemoveClientAddon(PublishedFileId_t addon, uint64 steamID64 = 0);
	void ClearClientAddons(uint64 steamID64 = 0);
	plg::vector<PublishedFileId_t> GetClientAddons(uint64 steamID64 = 0);

public:
	void OnSteamAPIActivated();
	void OnSteamAPIDeactivated();
	void OnStartupServer();
	void OnClientConnect(CPlayerSlot slot, const char* name, uint64 steamID64, const char* networkID);
	void OnClientDisconnect(CPlayerSlot slot, const char* name, uint64 steamID64, const char* networkID);
	void OnClientActive(CPlayerSlot slot, bool loadGame, const char* name, uint64 steamID64);
	void OnGameFrame();
	void OnPostEvent(INetworkMessageInternal* message, CNetMessage* data, uint64_t* clients);

	void OnSendNetMessage(CServerSideClient* client, CNetMessage* data, NetChannelBufType_t bufType);
	void OnHostStateRequest(CHostStateMgr* manager, CHostStateRequest* request);
	void OnReplyConnection(CNetworkGameServerBase* server, CServerSideClient* client);
	void OnReplyConnection_Post(CNetworkGameServerBase* server, CServerSideClient* client);

public:
	plg::vector<PublishedFileId_t> m_extraAddons;
	plg::vector<PublishedFileId_t> m_mountedAddons;
	plg::vector<PublishedFileId_t> m_globalClientAddons;

private:
	plg::vector<PublishedFileId_t> m_importantDownloads; // Important addon downloads that will trigger a map reload when finished
	std::deque<PublishedFileId_t> m_downloadQueue; // Queue of all addon downloads to print progress

	STEAM_GAMESERVER_CALLBACK_MANUAL(MultiAddonManager, OnAddonDownloaded, DownloadItemResult_t, m_CallbackDownloadItemResult);

	// Used when reloading current map
	PublishedFileId_t m_currentWorkshopMap{k_PublishedFileIdInvalid};
	bool m_callbackRegistered{};
};
inline MultiAddonManager& g_MultiAddonManager = MultiAddonManager::Instance();

#endif