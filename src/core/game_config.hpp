#pragma once

#include <core/sdk/utils.h>
#include <dynlibutils/memaddr.hpp>
#include <dynlibutils/module.hpp>

using Module = DynLibUtils::CModule;
using Memory = DynLibUtils::CMemory;

class GameConfig {
	friend class GameConfigManager;
public:
	GameConfig(plg::string game, plg::vector<plg::string> path);
	GameConfig(GameConfig&& other) = default;
	~GameConfig();

	bool Initialize();

	const plg::vector<plg::string>& GetPaths() const;
	std::string_view GetLibrary(std::string_view name) const;
	std::string_view GetSignature(std::string_view name) const;
	std::string_view GetSymbol(std::string_view name) const;
	std::string_view GetPatch(std::string_view name) const;
	int32_t GetOffset(std::string_view name) const;
	Memory GetAddress(std::string_view name) const;
	const Module* GetModule(std::string_view name) const;
	bool IsSymbol(std::string_view name) const;
	Memory ResolveSignature(std::string_view name) const;

private:
	struct AddressConf {
		plg::string signature;
		plg::vector<std::pair<int, bool>> read;
		bool lastIsOffset;
	};

private:
	plg::string m_gameDir;
	plg::vector<plg::string> m_paths;
	plg::flat_hash_map<plg::string, int32_t, plg::string_hash, std::equal_to<>> m_offsets;
	plg::flat_hash_map<plg::string, plg::string, plg::string_hash, std::equal_to<>> m_signatures;
	plg::flat_hash_map<plg::string, AddressConf, plg::string_hash, std::equal_to<>> m_addresses;
	plg::flat_hash_map<plg::string, plg::string, plg::string_hash, std::equal_to<>> m_libraries;
	plg::flat_hash_map<plg::string, plg::string, plg::string_hash, std::equal_to<>> m_patches;
	size_t m_refCount{1};
};

class GameConfigManager {
public:
	GameConfigManager();
	~GameConfigManager() = default;

	uint32_t LoadGameConfigFile(plg::vector<plg::string> paths);
	void CloseGameConfigFile(uint32_t id);
	GameConfig* GetGameConfig(uint32_t id);
	Module* GetModule(std::string_view name);

private:
	plg::flat_hash_map<uint32_t, std::unique_ptr<GameConfig>> m_configs;
	plg::flat_hash_map<plg::string, std::unique_ptr<Module>, plg::string_hash, std::equal_to<>> m_modules;
	std::mutex m_mutex;
	static inline uint32_t s_nextId = static_cast<uint32_t>(-1);
};

extern GameConfigManager g_GameConfigManager;