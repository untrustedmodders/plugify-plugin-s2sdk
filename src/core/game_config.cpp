#include <core/sdk/utils.h>
#include <platform.h>
#include <plugify-configs/plugify-configs.hpp>

#include "game_config.hpp"

GameConfig::GameConfig(plg::string game, plg::vector<plg::string> paths) : m_gameDir(std::move(game)), m_paths(std::move(paths)) {
}

GameConfig::~GameConfig() = default;

bool GameConfig::Initialize() {
	std::inplace_vector<std::string_view, 16> paths;
	paths.reserve(m_paths.size());
	for (const auto& path : m_paths) {
		paths.emplace_back(path);
	}
	auto config = pcf::ReadConfigs(paths);
	if (!config) {
		plg::print(LS_ERROR, "Failed to load configuration file: \"{}\"\n", pcf::GetError());
		return false;
	}

	config->JumpKey(m_gameDir);

	if (config->JumpKey("Signatures")) {
		if (config->IsObject() && config->JumpFirst()) {
			do {
				if (config->IsObject()) {
					m_libraries[config->GetName()] = config->GetString("library");
					m_signatures[config->GetName()] = config->GetString(S2SDK_PLATFORM);
				}
			} while (config->JumpNext());
			config->JumpBack();
		}
		config->JumpBack();
	}

	if (config->JumpKey("Offsets")) {
		if (config->IsObject() && config->JumpFirst()) {
			do {
				if (config->IsObject()) {
					m_offsets[config->GetName()] = config->GetAsInt32(S2SDK_PLATFORM, -1);
				}
			} while (config->JumpNext());
			config->JumpBack();
		}
		config->JumpBack();
	}

	if (config->JumpKey("Patches")) {
		if (config->IsObject() && config->JumpFirst()) {
			do {
				if (config->IsObject()) {
					m_patches[config->GetName()] = config->GetString(S2SDK_PLATFORM);
				}
			} while (config->JumpNext());
			config->JumpBack();
		}
		config->JumpBack();
	}

	auto getAddressConf = [](std::unique_ptr<pcf::Config>& config) -> AddressConf {
		AddressConf conf{config->GetString("signature"), {}, false};
		if (config->JumpKey(S2SDK_PLATFORM)) {
			if (config->IsArray() && config->JumpFirst()) {
				do {
					if (config->IsObject() && config->JumpFirst()) {
						auto name = config->GetName();
						if (conf.lastIsOffset) {
							plg::print(LS_WARNING, "Error parsing Address \"{}\", 'offset' entry must be the last entry\n", name);
							continue;
						}
						conf.read.emplace_back(config->GetAsInt32(), name == "read_offs32");
						if (name == "offset") {
							conf.lastIsOffset = true;
						}
						config->JumpBack();
					}
				} while (config->JumpNext());
				config->JumpBack();
			}
			config->JumpBack();
		}
		return conf;
	};

	if (config->JumpKey("Addresses")) {
		if (config->IsObject() && config->JumpFirst()) {
			do {
				if (config->IsObject()) {
					m_addresses[config->GetName()] = getAddressConf(config);
				}
			} while (config->JumpNext());
			config->JumpBack();
		}
		config->JumpBack();
	}

	return true;
}

const plg::vector<plg::string>& GameConfig::GetPaths() const {
	return m_paths;
}

std::string_view GameConfig::GetSignature(std::string_view name) const {
	auto it = m_signatures.find(name);
	if (it == m_signatures.end())
		return {};
	return it->second;
}

std::string_view GameConfig::GetPatch(std::string_view name) const {
	auto it = m_patches.find(name);
	if (it == m_patches.end())
		return {};
	return it->second;
}

int32_t GameConfig::GetOffset(std::string_view name) const {
	auto it = m_offsets.find(name);
	if (it == m_offsets.end())
		return -1;
	return it->second;
}

std::string_view GameConfig::GetLibrary(std::string_view name) const {
	auto it = m_libraries.find(name);
	if (it == m_libraries.end())
		return {};
	return it->second;
}

// memory addresses below 0x10000 are automatically considered invalid for dereferencing
constexpr Memory VALID_MINIMUM_MEMORY_ADDRESS(0x10000);

Memory GameConfig::GetAddress(std::string_view name) const {
	auto it = m_addresses.find(name);
	if (it == m_addresses.end())
		return {};

	const auto& addrConf = it->second;

	auto addr = ResolveSignature(addrConf.signature);
	if (!addr)
		return {};

	size_t readCount = addrConf.read.size();
	for (size_t i = 0; i < readCount; ++i) {
		const auto& [offset, rel] = addrConf.read[i];

		// NULLs in the middle of an indirection chain are bad, end NULL is ok
		if (!addr || addr < VALID_MINIMUM_MEMORY_ADDRESS)
			return nullptr;

		if (rel) {
			auto target = addr.Offset(offset);
			if (!target || target < VALID_MINIMUM_MEMORY_ADDRESS)
				return nullptr;

			addr.OffsetSelf(offset)
					.OffsetSelf(sizeof(int32_t))
					.OffsetSelf(target.Get<int32_t>());
		} else {
			addr.OffsetSelf(offset);

			// If lastIsOffset is set and this is the last iteration of the loop, don't deref
			if (!addrConf.lastIsOffset || i != readCount - 1) {
				addr.DerefSelf();
			}
		}
	}

	return addr;
}

const Module* GameConfig::GetModule(std::string_view name) const {
	const std::string_view library = GetLibrary(name);
	if (library.empty())
		return nullptr;

	return g_GameConfigManager.GetModule(library);
}

bool GameConfig::IsSymbol(std::string_view name) const {
	const std::string_view sigOrSymbol = GetSignature(name);
	if (sigOrSymbol.empty()) {
		plg::print(LS_WARNING, "Missing signature or symbol: {}\n", name);
		return false;
	}
	return sigOrSymbol[0] == '@';
}

std::string_view GameConfig::GetSymbol(std::string_view name) const {
	const std::string_view symbol = GetSignature(name);

	if (symbol.size() <= 1) {
		plg::print(LS_WARNING, "Missing symbol: {}\n", name);
		return {};
	}

	return symbol.substr(1);
}

Memory GameConfig::ResolveSignature(std::string_view name) const {
	auto module = GetModule(name);
	if (!module) {
		plg::print(LS_WARNING, "Invalid module: {}\n", name);
		return {};
	}

	Memory address;

	if (IsSymbol(name)) {
		const std::string_view symbol = GetSymbol(name);
		if (symbol.empty()) {
			plg::print(LS_WARNING, "Invalid symbol for {}\n", name);
			return {};
		}

		address = module->GetFunctionByName(symbol);
	} else {
		const std::string_view signature = GetSignature(name);
		if (signature.empty()) {
			plg::print(LS_WARNING, "Failed to find signature for {}\n", name);
			return {};
		}

		address = module->FindPattern(DynLibUtils::ParsePattern(signature));
	}

	if (!address) {
		plg::print(LS_WARNING, "Failed to find address for {}\n", name);
		return {};
	}

	return address;
}

#if S2SDK_PLATFORM_WINDOWS
#include <windows.h>
#else
#include <dlfcn.h>
#endif

GameConfigManager::GameConfigManager() {
	// metamod workaround
	if (Module("metamod.2.cs2").GetHandle() != nullptr) {
#if S2SDK_PLATFORM_WINDOWS
		int flags = DONT_RESOLVE_DLL_REFERENCES;
#else
		int flags = RTLD_LAZY | RTLD_NOLOAD;
#endif
		auto engine2 = std::make_unique<Module>();
		engine2->LoadFromPath(plg::as_string(utils::GameDirectory() / S2SDK_ROOT_BINARY S2SDK_LIBRARY_PREFIX "engine2"), flags);
		if (engine2->IsValid()) {
			m_modules.emplace("engine2", std::move(engine2));
		}
		auto server = std::make_unique<Module>();
		server->LoadFromPath(plg::as_string(utils::GameDirectory() / S2SDK_GAME_BINARY S2SDK_LIBRARY_PREFIX "server"), flags);
		if (server->IsValid()) {
			m_modules.emplace("server", std::move(server));
		}
	} else {
		auto engine2 = std::make_unique<Module>("engine2");
		if (engine2->IsValid()) {
			m_modules.emplace("engine2", std::move(engine2));
		}
		auto server = std::make_unique<Module>("server");
		if (server->IsValid()) {
			m_modules.emplace("server", std::move(server));
		}
	}
	// add more for preload
}

uint32_t GameConfigManager::LoadGameConfigFile(plg::vector<plg::string> paths) {
	std::unique_lock lock(m_mutex);

	for (auto& [id, cfg] : m_configs) {
		auto& config = *cfg;
		for (const auto& fullPath : config.GetPaths()) {
			for (const auto& path : paths) {
				if (fullPath.ends_with(path)) {
					++config.m_refCount;
					return id;
				}
			}
		}
	}

	auto gameConfig = std::make_unique<GameConfig>(S2SDK_GAME_NAME, std::move(paths));
	if (!gameConfig->Initialize()) {
		return static_cast<uint32_t>(-1);
	}

	uint32_t id = ++s_nextId;
	m_configs.emplace(id, std::move(gameConfig));
	return id;
}

void GameConfigManager::CloseGameConfigFile(uint32_t id) {
	std::unique_lock lock(m_mutex);

	auto it = m_configs.find(id);
	if (it != m_configs.end()) {
		auto& config = *it->second;
		if (--config.m_refCount == 0) {
			m_configs.erase(it);
		}
	}
}

GameConfig* GameConfigManager::GetGameConfig(uint32_t id) {
	std::shared_lock lock(m_mutex);

	auto it = m_configs.find(id);
	if (it != m_configs.end()) {
		return it->second.get();
	}

	return nullptr;
}

Module* GameConfigManager::GetModule(std::string_view name) {
	{
		std::shared_lock lock(m_mutex);
		auto it = m_modules.find(name);
		if (it != m_modules.end()) {
			return it->second.get();
		}
	}

	std::unique_lock lock(m_mutex);
	return m_modules.emplace(name, std::make_unique<Module>(name)).first->second.get();
}

GameConfigManager g_GameConfigManager;