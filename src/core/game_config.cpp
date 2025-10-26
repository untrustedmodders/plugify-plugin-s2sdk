#include <core/sdk/utils.hpp>

#include "game_config.hpp"

ModuleProvider::ModuleProvider() {
	PreloadModules();
}

void ModuleProvider::PreloadModules() {
	// Metamod workaround - check if metamod is loaded
	const bool hasMetamod = Module("metamod.2.cs2").GetHandle() != nullptr;

#if S2SDK_PLATFORM_WINDOWS
	const int flags = hasMetamod ? DONT_RESOLVE_DLL_REFERENCES : 0;
#else
	const int flags = hasMetamod ? (RTLD_LAZY | RTLD_NOLOAD) : 0;
#endif

	// Helper lambda to load a module
	auto loadModule = [&](std::string_view name, const auto& path) {
		auto module = std::make_shared<Module>();

		if (hasMetamod) {
			module->LoadFromPath(plg::as_string(path), flags);
		} else {
			module = std::make_shared<Module>(name);
		}

		if (module->IsValid()) {
			std::unique_lock lock(m_mutex);
			m_modules[name] = std::move(module);
		}
	};

	// Load common modules
	loadModule("engine2", utils::GameDirectory() / S2SDK_ROOT_BINARY S2SDK_LIBRARY_PREFIX "engine2");
	loadModule("server", utils::GameDirectory() / S2SDK_GAME_BINARY S2SDK_LIBRARY_PREFIX "server");
}

std::shared_ptr<Module> ModuleProvider::GetModule(std::string_view name) {
	// Try to find existing module
	{
		std::shared_lock lock(m_mutex);
		auto it = m_modules.find(name);
		if (it != m_modules.end()) {
			return it->second;
		}
	}

	// Load module
	return LoadModuleInternal(name);
}

std::shared_ptr<Module> ModuleProvider::LoadModuleInternal(std::string_view name) {
	std::unique_lock lock(m_mutex);

	auto module = std::make_shared<Module>(name);
	if (!module->IsValid()) {
		plg::print(LS_WARNING, "Failed to load module: {}\n", name);
		return {};
	}

	auto ref = module;
	m_modules[name] = std::move(module);
	return ref;
}

bool ModuleProvider::HasModule(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_modules.contains(name);
}


Result<void> ConfigLoader::Load() {
	// Parse config files
	if (auto result = ParseConfigFile(m_options.gameDirectory); !result) {
		return result;
	}

	return {};
}

Result<void> ConfigLoader::ParseConfigFile(std::string_view gameName) {
	if (gameName.empty()) {
		return MakeError("No game name provided");
	}

	if (m_options.configPaths.empty()) {
		return MakeError("No config paths provided");
	}

	std::inplace_vector<std::string_view, 16> paths;
	paths.reserve(m_options.configPaths.size());
	for (const auto& path : m_options.configPaths) {
		paths.emplace_back(path);
	}

	auto config = pcf::ReadConfigs(paths);
	if (!config) {
		return MakeError("Reading error: {}", pcf::GetError());
	}

	// Jump to game-specific section
	if (!config->JumpKey(gameName)) {
		return MakeError("Game section not found: {}", gameName);
	}

	// Load each section
	struct SectionLoader {
		const char* name;
		Result<void> (ConfigLoader::*loader)(pcf::Config*);
	};

	constexpr SectionLoader loaders[] = {
		{"Signatures", &ConfigLoader::LoadSignatures},
		{"Offsets", &ConfigLoader::LoadOffsets},
		{"Patches", &ConfigLoader::LoadPatches},
		{"Addresses", &ConfigLoader::LoadAddresses},
		{"VTables", &ConfigLoader::LoadVTables},
	};

	bool hasErrors = false;
	for (const auto& section : loaders) {
		if (auto result = (this->*section.loader)(config.get()); !result) {
			if (m_options.strictMode) {
				return result;
			}
			plg::print(LS_WARNING, "{} loading failed: {}\n", section.name, result.error());
			hasErrors = true;
		}
	}

	if (hasErrors && m_options.strictMode) {
		return MakeError("Configuration loading completed with errors");
	}

	return {};
}

Result<void> ConfigLoader::LoadSignatures(pcf::Config* config) {
	if (!config->JumpKey("Signatures")) {
		// No signatures section is okay
		return {};
	}

	if (config->IsObject() && config->JumpFirst()) {
		do {
			if (config->IsObject()) {
				SignatureData sig;
				sig.name = config->GetName();
				sig.library = config->GetString("library");
				plg::string signature = config->GetString(S2SDK_PLATFORM);

				if (signature.empty()) {
					plg::print(LS_WARNING, "Empty signature for: {}\n", sig.name);
					continue;
				}

				// Determine type
				if (signature[0] == '@') {
					sig.type = SignatureData::Type::Symbol;
					sig.value = signature.substr(1);
				} else {
					sig.type = SignatureData::Type::Pattern;
					sig.value = std::move(signature);
				}

				m_signatures[sig.name] = std::move(sig);
			}
		} while (config->JumpNext());
		config->JumpBack();
	}
	config->JumpBack();

	return {};
}

Result<void> ConfigLoader::LoadOffsets(pcf::Config* config) {
	if (!config->JumpKey("Offsets")) {
		return {};
	}

	if (config->IsObject() && config->JumpFirst()) {
		do {
			if (config->IsObject()) {
				OffsetData offset;
				offset.name = config->GetName();
				offset.value = config->GetAsInt32(S2SDK_PLATFORM, -1);
				m_offsets[offset.name] = std::move(offset);
			}
		} while (config->JumpNext());
		config->JumpBack();
	}
	config->JumpBack();

	return {};
}

Result<void> ConfigLoader::LoadVTables(pcf::Config* config) {
	if (!config->JumpKey("VTables")) {
		return {};
	}

	if (config->IsObject() && config->JumpFirst()) {
		do {
			if (config->IsObject()) {
				VTableData vt;
				vt.name = config->GetName();
				vt.library = config->GetString("library");
				vt.table = config->GetString("table");
				m_vtables[vt.name] = std::move(vt);
			}
		} while (config->JumpNext());
		config->JumpBack();
	}
	config->JumpBack();

	return {};
}

Result<void> ConfigLoader::LoadPatches(pcf::Config* config) {
	if (!config->JumpKey("Patches")) {
		return {};
	}

	if (config->IsObject() && config->JumpFirst()) {
		do {
			if (config->IsObject()) {
				PatchData patch;
				patch.name = config->GetName();
				patch.baseAddress = config->GetString("address");
				patch.pattern = config->GetString(S2SDK_PLATFORM);
				m_patches[patch.name] = std::move(patch);
			}
		} while (config->JumpNext());
		config->JumpBack();
	}
	config->JumpBack();

	return {};
}

Result<void> ConfigLoader::LoadAddresses(pcf::Config* config) {
	if (!config->JumpKey("Addresses")) {
		return {};
	}

	if (config->IsObject() && config->JumpFirst()) {
		do {
			if (config->IsObject()) {
				auto name = config->GetName();
				auto result = ParseAddressConfig(config, name);
				if (result) {
					m_addresses[name] = std::move(*result);
				} else {
					plg::print(LS_WARNING, "Failed to parse address '{}': {}\n",
							   name, result.error());
				}
			}
		} while (config->JumpNext());
		config->JumpBack();
	}
	config->JumpBack();

	return {};
}

Result<AddressData> ConfigLoader::ParseAddressConfig(pcf::Config* config, std::string_view name) {
	AddressData addr;
	addr.name = name;
	addr.baseSignature = config->GetString("signature");

	if (addr.baseSignature.empty()) {
		return MakeError("Missing base signature");
	}

	// Parse indirection chain
	if (config->JumpKey(S2SDK_PLATFORM)) {
		if (config->IsArray() && config->JumpFirst()) {
			do {
				if (config->IsObject() && config->JumpFirst()) {
					auto stepName = config->GetName();
					auto value = config->GetAsInt32();

					if (stepName == "offset") {
						addr.indirections.push_back(IndirectionStep::MakeOffset(value));
					} else if (stepName == "read") {
						addr.indirections.push_back(IndirectionStep::MakeDereference(value));
					} else if (stepName == "read_offs32") {
						addr.indirections.push_back(IndirectionStep::MakeRelative(value));
					} else {
						plg::print(LS_WARNING, "Unknown indirection type: {}\n", stepName);
					}

					config->JumpBack();
				}
			} while (config->JumpNext());
			config->JumpBack();
		}
		config->JumpBack();
	}

	return addr;
}

std::optional<SignatureData> ConfigLoader::GetSignature(std::string_view name) const {
	auto it = m_signatures.find(name);
	if (it == m_signatures.end()) return std::nullopt;
	return it->second;
}

std::optional<AddressData> ConfigLoader::GetAddress(std::string_view name) const {
	auto it = m_addresses.find(name);
	if (it == m_addresses.end()) return std::nullopt;
	return it->second;
}

std::optional<OffsetData> ConfigLoader::GetOffset(std::string_view name) const {
	auto it = m_offsets.find(name);
	if (it == m_offsets.end()) return std::nullopt;
	return it->second;
}

std::optional<VTableData> ConfigLoader::GetVTable(std::string_view name) const {
	auto it = m_vtables.find(name);
	if (it == m_vtables.end()) return std::nullopt;
	return it->second;
}

std::optional<PatchData> ConfigLoader::GetPatch(std::string_view name) const {
	auto it = m_patches.find(name);
	if (it == m_patches.end()) return std::nullopt;
	return it->second;
}

template <typename Fn>
void ConfigLoader::ForEachSignature(Fn&& fn) const {
	std::for_each(m_signatures.begin(), m_signatures.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

template <typename Fn>
void ConfigLoader::ForEachAddress(Fn&& fn) const  {
	std::for_each(m_addresses.begin(), m_addresses.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

template <typename Fn>
void ConfigLoader::ForEachVTable(Fn&& fn) const  {
	std::for_each(m_vtables.begin(), m_vtables.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

template <typename Fn>
void ConfigLoader::ForEachOffset(Fn&& fn) const  {
	std::for_each(m_offsets.begin(), m_offsets.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

template <typename Fn>
void ConfigLoader::ForEachPatch(Fn&& fn) const  {
	std::for_each(m_patches.begin(), m_patches.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

void ConfigCache::CacheSignature(const ResolvedSignature& signature) {
	{
		std::unique_lock lock(m_mutex);
		m_signatures[signature.name] = signature;
	}
	if (signature.address) {
		m_stats.cachedSignatures.fetch_add(1, std::memory_order_relaxed);
	} else {
		m_stats.failedSignatures.fetch_add(1, std::memory_order_relaxed);
	}
}

void ConfigCache::CacheAddress(const ResolvedAddress& address) {
	{
		std::unique_lock lock(m_mutex);
		m_addresses[address.name] = address;
	}
	if (address.address) {
		m_stats.cachedAddresses.fetch_add(1, std::memory_order_relaxed);
	} else {
		m_stats.failedAddresses.fetch_add(1, std::memory_order_relaxed);
	}
}

void ConfigCache::CacheVTable(const ResolvedVTable& vtable) {
	{
		std::unique_lock lock(m_mutex);
		m_vtables[vtable.name] = vtable;
	}
	if (vtable.address) {
		m_stats.cachedVTables.fetch_add(1, std::memory_order_relaxed);
	} else {
		m_stats.failedVTables.fetch_add(1, std::memory_order_relaxed);
	}
}

std::optional<ResolvedSignature> ConfigCache::GetSignature(std::string_view name) const {
	m_stats.totalLookups.fetch_add(1, std::memory_order_relaxed);

	std::shared_lock lock(m_mutex);
	auto it = m_signatures.find(name);
	if (it != m_signatures.end()) {
		m_stats.cacheHits.fetch_add(1, std::memory_order_relaxed);
		return it->second;
	}
	return std::nullopt;
}

std::optional<ResolvedAddress> ConfigCache::GetAddress(std::string_view name) const {
	m_stats.totalLookups.fetch_add(1, std::memory_order_relaxed);

	std::shared_lock lock(m_mutex);
	auto it = m_addresses.find(name);
	if (it != m_addresses.end()) {
		m_stats.cacheHits.fetch_add(1, std::memory_order_relaxed);
		return it->second;
	}
	return std::nullopt;
}

std::optional<ResolvedVTable> ConfigCache::GetVTable(std::string_view name) const {
	m_stats.totalLookups.fetch_add(1, std::memory_order_relaxed);

	std::shared_lock lock(m_mutex);
	auto it = m_vtables.find(name);
	if (it != m_vtables.end()) {
		m_stats.cacheHits.fetch_add(1, std::memory_order_relaxed);
		return it->second;
	}
	return std::nullopt;
}

bool ConfigCache::HasSignature(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_signatures.contains(name);
}

bool ConfigCache::HasAddress(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_addresses.contains(name);
}

bool ConfigCache::HasVTable(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_vtables.contains(name);
}

ConfigCache::Stats ConfigCache::GetStats() const {
	std::shared_lock lock(m_mutex);
	return m_stats.snapshot();
}

void ConfigCache::ResetStats() {
	std::unique_lock lock(m_mutex);
	m_stats.clear();
}

void ConfigCache::Clear() {
	std::unique_lock lock(m_mutex);
	m_signatures.clear();
	m_addresses.clear();
	m_vtables.clear();
	m_stats.clear();
}

template <typename Fn>
void ConfigCache::ForEachSignature(Fn&& fn) const {
	std::shared_lock lock(m_mutex);
	for (const auto& [_, sig] : m_signatures) {
		fn(sig);
	}
}

template <typename Fn>
void ConfigCache::ForEachAddress(Fn&& fn) const {
	std::shared_lock lock(m_mutex);
	for (const auto& [_, addr] : m_addresses) {
		fn(addr);
	}
}

template <typename Fn>
void ConfigCache::ForEachVTable(Fn&& fn) const {
	std::shared_lock lock(m_mutex);
	for (const auto& [_, addr] : m_vtables) {
		fn(addr);
	}
}

GameConfig::GameConfig(LoadOptions options)
	: m_options(std::move(options))
	, m_initialized(false)
	, m_loader(m_options)
{}

GameConfig::~GameConfig() {
	auto _ = RestoreAllPatches();
}

#define GC_TRY_VOID(expr) \
{ auto _result = (expr); \
   if (!_result) return _result; }

Result<void> GameConfig::Initialize() {
    auto start = std::chrono::high_resolution_clock::now();

	std::unique_lock lock(m_mutex);

	if (m_initialized) {
		return MakeError("GameConfig already initialized");
	}

	// Phase 1: Load configuration data
	GC_TRY_VOID(m_loader.Load());

	// Phase 2: Preload based on cache strategy
	switch (m_options.cacheStrategy) {
		case CacheStrategy::Eager:
			GC_TRY_VOID(PreloadSignatures());
			GC_TRY_VOID(PreloadAddresses());
			GC_TRY_VOID(PreloadVTables());
			break;

		case CacheStrategy::Hybrid:
			GC_TRY_VOID(PreloadSignatures());
			// Addresses/vtables loaded on-demand
			break;

		case CacheStrategy::Lazy:
			// Everything loaded on-demand
			break;
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto ns = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	plg::print(LS_DETAILED, "Initialize of [{}] took {} ms\n", plg::join(m_options.configPaths, ", "), ns);

	m_initialized = true;
	return {};
}

Result<void> GameConfig::PreloadSignatures() {
	std::atomic_flag hadErrors = {};

	m_loader.ForEachSignature([&](const SignatureData& sig) {
		auto result = m_signatureResolver.Resolve(sig);
		if (result) {
			m_cache.CacheSignature(*result);
		} else {
			plg::print(LS_WARNING, "Failed to resolve signature '{}': {}\n",
					   sig.name, result.error());
			m_cache.CacheSignature(ResolvedSignature::Failed(sig.name, result.error()));
			hadErrors.test_and_set(std::memory_order_relaxed);
		}
	});

	if (hadErrors.test_and_set(std::memory_order_relaxed) && m_options.strictMode) {
		return MakeError("One or more signatures failed to resolve");
	}

	return {};
}

Result<void> GameConfig::PreloadAddresses() {
	std::atomic_flag hadErrors = {};

	m_loader.ForEachAddress([&](const AddressData& addr) {
		// Get or resolve base signature
		auto baseSig = m_cache.GetSignature(addr.baseSignature);
		if (!baseSig) {
			auto sigResult = ResolveSignatureInternal(addr.baseSignature);
			if (sigResult) {
				baseSig = *sigResult;
			} else {
				plg::print(LS_WARNING, "Failed to resolve base signature '{}' for address '{}'\n",
						   addr.baseSignature, addr.name);
				m_cache.CacheAddress(ResolvedAddress::Failed(addr.name, sigResult.error()));
				hadErrors.test_and_set(std::memory_order_relaxed);
				return;
			}
		}

		auto result = m_addressResolver.Resolve(addr, *baseSig);
		if (result) {
			m_cache.CacheAddress(*result);
		} else {
			plg::print(LS_WARNING, "Failed to resolve address '{}': {}\n",
					   addr.name, result.error());
			m_cache.CacheAddress(ResolvedAddress::Failed(addr.name, result.error()));
			hadErrors.test_and_set(std::memory_order_relaxed);
		}
	});

	if (hadErrors.test_and_set(std::memory_order_relaxed) && m_options.strictMode) {
		return MakeError("One or more addresses failed to resolve");
	}

	return {};
}

Result<void> GameConfig::PreloadVTables() {
	std::atomic_flag hadErrors;

	m_loader.ForEachVTable([&](const VTableData& vt) {
		auto result = m_vtableResolver.Resolve(vt);
		if (result) {
			m_cache.CacheVTable(*result);
		} else {
			plg::print(LS_WARNING, "Failed to resolve vtable '{}': {}\n",
					   vt.name, result.error());
			m_cache.CacheVTable(ResolvedVTable::Failed(vt.name, result.error()));
			hadErrors.test_and_set(std::memory_order_relaxed);
		}
	});

	if (hadErrors.test_and_set(std::memory_order_relaxed) && m_options.strictMode) {
		return MakeError("One or more vtables failed to resolve");
	}

	return {};
}

Result<void> GameConfig::ApplyAllPatches(const PatchOptions& options) {
	std::atomic_flag hadErrors = {};

	m_loader.ForEachPatch([&](const PatchData& patch) {
		auto result = ApplyPatch(patch.name, patch.baseAddress, options);
		if (!result) {
			plg::print(LS_WARNING, "Failed to apply patch '{}': {}\n",
					   patch.name, result.error());
			hadErrors.test_and_set(std::memory_order_relaxed);
		}
	});

	if (hadErrors.test_and_set(std::memory_order_relaxed) && m_options.allowPartialApply) {
		return MakeError("One or more patches failed to apply");
	}
	return {};
}

Result<void> GameConfig::RestorePatch(std::string_view name) {
	auto& patchManager = GameConfigManager::Instance().GetPatchManager();
	return patchManager.RestorePatch(name);
}

Result<void> GameConfig::RestoreAllPatches() {
	std::atomic_flag hadErrors = {};

	m_loader.ForEachPatch([&](const PatchData& patch) {
		auto result = RestorePatch(patch.name);
		if (!result) {
			plg::print(LS_WARNING, "Failed to restore patch '{}': {}\n",
					   patch.name, result.error());
			hadErrors.test_and_set(std::memory_order_relaxed);
		}
	});

	if (hadErrors.test_and_set(std::memory_order_relaxed)) {
		return MakeError("One or more patches failed to restore");
	}
	return {};
}

bool GameConfig::IsPatchApplied(std::string_view name) const {
	auto& patchManager = GameConfigManager::Instance().GetPatchManager();
	return patchManager.IsPatchApplied(name);
}

PatchManager::Stats GameConfig::GetPatchStats() const {
	auto& patchManager = GameConfigManager::Instance().GetPatchManager();
	return patchManager.GetStats();
}

Result<ResolvedSignature> GameConfig::ResolveSignatureInternal(std::string_view name) {
	// Check cache first
	if (auto cached = m_cache.GetSignature(name)) {
		if (!cached->address) {
			return MakeError("Signature previously failed to resolve: {}", cached->error);
		}
		return *cached;
	}

	// Resolve and cache
	auto result = m_signatureResolver.ResolveByName(name, m_loader);
	if (result) {
		m_cache.CacheSignature(*result);
	} else {
		// Cache the failure
		m_cache.CacheSignature(ResolvedSignature::Failed(name, result.error()));
	}

	return result;
}

Result<ResolvedAddress> GameConfig::ResolveAddressInternal(std::string_view name) {
	// Check cache first
	if (auto cached = m_cache.GetAddress(name)) {
		if (!cached->address) {
			return MakeError("Address previously failed to resolve: {}", cached->error);
		}
		return *cached;
	}

	// Resolve and cache
	auto result = m_addressResolver.ResolveByName(
		name,
		m_loader,
		[&](std::string_view sigName) -> std::optional<ResolvedSignature> {
			auto sigResult = ResolveSignatureInternal(sigName);
			if (sigResult) {
				return *sigResult;
			}
			return std::nullopt;
		}
	);

	if (result) {
		m_cache.CacheAddress(*result);
	} else {
		m_cache.CacheAddress(ResolvedAddress::Failed(name, result.error()));
	}

	return result;
}

Result<ResolvedVTable> GameConfig::ResolveVTableInternal(std::string_view name) {
	// Check cache first
	if (auto cached = m_cache.GetVTable(name)) {
		if (!cached->address) {
			return MakeError("VTable previously failed to resolve: {}", cached->error);
		}
		return *cached;
	}

	// Resolve and cache
	auto result = m_vtableResolver.ResolveByName(name, m_loader);
	if (result) {
		m_cache.CacheVTable(*result);
	} else {
		// Cache the failure
		m_cache.CacheVTable(ResolvedVTable::Failed(name, result.error()));
	}

	return result;
}

Result<void> GameConfig::ApplyPatch(
	std::string_view name,
	std::string_view address,
	const PatchOptions& options
) {
	// Get patch pattern from config
	auto patchResult = GetPatch(name);
	if (!patchResult) {
		return MakeError(std::move(patchResult.error()));
	}

	// Get target address
	auto addrResult = HasAddress(address) ? GetAddress(address) : GetSignature(address);
	if (!addrResult) {
		return MakeError(std::move(addrResult.error()));
	}

	// Apply the patch
	auto& patchManager = GameConfigManager::Instance().GetPatchManager();
	return patchManager.ApplyPatch(name, *addrResult, *patchResult, options);
}

Result<Memory> GameConfig::GetAddress(std::string_view name) {
	std::shared_lock lock(m_mutex);

	if (!m_initialized) {
		return MakeError("GameConfig not initialized");
	}

	auto result = ResolveAddressInternal(name);
	if (!result) {
		return MakeError(std::move(result.error()));
	}

	return result->address;
}

Result<Memory> GameConfig::GetSignature(std::string_view name) {
	std::shared_lock lock(m_mutex);

	if (!m_initialized) {
		return MakeError("GameConfig not initialized");
	}

	auto result = ResolveSignatureInternal(name);
	if (!result) {
		return MakeError(std::move(result.error()));
	}

	return result->address;
}

Result<Memory> GameConfig::GetVTable(std::string_view name) {
	std::shared_lock lock(m_mutex);

	if (!m_initialized) {
		return MakeError("GameConfig not initialized");
	}

	auto result = ResolveVTableInternal(name);
	if (!result) {
		return MakeError(std::move(result.error()));
	}

	return result->address;
}

Result<int32_t> GameConfig::GetOffset(std::string_view name) {
	std::shared_lock lock(m_mutex);

	if (!m_initialized) {
		return MakeError("GameConfig not initialized");
	}

	auto offset = m_loader.GetOffset(name);
	if (!offset) {
		return MakeError("Offset not found: {}", name);
	}

	return offset->value;
}

Result<plg::string> GameConfig::GetPatch(std::string_view name) {
	std::shared_lock lock(m_mutex);

	if (!m_initialized) {
		return MakeError("GameConfig not initialized");
	}

	auto patch = m_loader.GetPatch(name);
	if (!patch) {
		return MakeError("Patch not found: {}", name);
	}

	return patch->pattern;
}

Result<std::shared_ptr<Module>> GameConfig::GetModule(std::string_view name) {
	std::shared_lock lock(m_mutex);

	auto& provider = GameConfigManager::Instance().GetModuleProvider();
	auto module = provider.GetModule(name);
	if (!module) {
		return MakeError("Module not found: {}", name);
	}

	return module;
}

bool GameConfig::HasAddress(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_loader.GetAddress(name).has_value();
}

bool GameConfig::HasSignature(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_loader.GetSignature(name).has_value();
}

bool GameConfig::HasVTable(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_loader.GetVTable(name).has_value();
}

bool GameConfig::HasOffset(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_loader.GetOffset(name).has_value();
}

bool GameConfig::HasPatch(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	return m_loader.GetPatch(name).has_value();
}

void GameConfig::PreloadAll() {
	std::unique_lock lock(m_mutex);

	if (!m_initialized) {
		plg::print(LS_WARNING, "Cannot preload - GameConfig not initialized\n");
		return;
	}

	auto sigResult = PreloadSignatures();
	if (!sigResult) {
		plg::print(LS_WARNING, "Cannot preload - {}\n", sigResult.error());
		return;
	}
	auto addrResult = PreloadAddresses();
	if (!addrResult) {
		plg::print(LS_WARNING, "Cannot preload - {}\n", addrResult.error());
		return;
	}
}

void GameConfig::ClearCache() {
	std::unique_lock lock(m_mutex);
	m_cache.Clear();
}

ConfigCache::Stats GameConfig::GetCacheStats() const {
	std::shared_lock lock(m_mutex);
	return m_cache.GetStats();
}

GameConfig::DiagnosticsInfo GameConfig::GetDiagnostics() const {
	std::shared_lock lock(m_mutex);

	DiagnosticsInfo info;
	info.initialized = m_initialized;
	info.totalSignatures = m_loader.GetSignatureCount();
	info.totalAddresses = m_loader.GetAddressCount();
	info.totalVTables = m_loader.GetVTableCount();
	info.cacheStats = m_cache.GetStats();

	// Count resolved signatures and addresses
	m_cache.ForEachSignature([&](const ResolvedSignature& sig) {
		if (sig.address) {
			++info.resolvedSignatures;
		} else {
			info.failedSignatures.push_back(sig.name);
		}
	});

	m_cache.ForEachAddress([&](const ResolvedAddress& addr) {
		if (addr.address) {
			++info.resolvedAddresses;
		} else {
			info.failedAddresses.push_back(addr.name);
		}
	});

	m_cache.ForEachVTable([&](const ResolvedVTable& vt) {
		if (vt.address) {
			++info.resolvedVTables;
		} else {
			info.failedVTables.push_back(vt.name);
		}
	});

	return info;
}

void GameConfig::PrintDiagnostics() const {
	auto diag = GetDiagnostics();

	plg::print(LS_MESSAGE, "=== GameConfig Diagnostics ===\n");
	plg::print(LS_MESSAGE, "Initialized: {}\n", diag.initialized ? "Yes" : "No");
	plg::print(LS_MESSAGE, "Signatures: {}/{} resolved\n",
			   diag.resolvedSignatures, diag.totalSignatures);
	plg::print(LS_MESSAGE, "Addresses: {}/{} resolved\n",
			   diag.resolvedAddresses, diag.totalAddresses);

	auto& stats = diag.cacheStats;
	plg::print(LS_MESSAGE, "Cache Stats:\n");
	plg::print(LS_MESSAGE, "  Cached Signatures: {}\n", stats.cachedSignatures);
	plg::print(LS_MESSAGE, "  Cached Addresses: {}\n", stats.cachedAddresses);
	plg::print(LS_MESSAGE, "  Total Lookups: {}\n", stats.totalLookups);
	plg::print(LS_MESSAGE, "  Cache Hits: {}\n", stats.cacheHits);

	if (!diag.failedSignatures.empty()) {
		plg::print(LS_WARNING, "Failed Signatures:\n");
		for (const auto& name : diag.failedSignatures) {
			plg::print(LS_WARNING, "  - {}\n", name);
		}
	}

	if (!diag.failedAddresses.empty()) {
		plg::print(LS_WARNING, "Failed Addresses:\n");
		for (const auto& name : diag.failedAddresses) {
			plg::print(LS_WARNING, "  - {}\n", name);
		}
	}

	plg::print(LS_MESSAGE, "==============================\n");
}

Result<ResolvedSignature> SignatureResolver::Resolve(const SignatureData& signature) const {
	auto& provider = GameConfigManager::Instance().GetModuleProvider();
	auto module = provider.GetModule(signature.library);
	if (!module) {
		return MakeError("Module not found: {}", signature.library);
	}

	Result<Memory> addrResult = signature.IsSymbol()
									? ResolveSymbol(module, signature.value)
									: ResolvePattern(module, signature.value);

	if (!addrResult) {
		return MakeError(std::move(addrResult.error()));
	}

	return ResolvedSignature{*addrResult, signature.name};
}

Result<ResolvedSignature> SignatureResolver::ResolveByName(
	std::string_view name,
	const ConfigLoader& loader
) const {
	auto sigData = loader.GetSignature(name);
	if (!sigData) {
		return MakeError("Signature not found in config: {}", name);
	}

	return Resolve(*sigData);
}

Result<Memory> SignatureResolver::ResolvePattern(const std::shared_ptr<Module>& module, std::string_view pattern) const {
	auto address = module->FindPattern(DynLibUtils::ParsePattern(pattern));
	if (!address) {
		return MakeError("Pattern not found: {}", pattern);
	}
	return address;
}

Result<Memory> SignatureResolver::ResolveSymbol(const std::shared_ptr<Module>& module, std::string_view symbol) const {
	auto address = module->GetFunctionByName(symbol);
	if (!address) {
		return MakeError("Symbol not found: {}", symbol);
	}
	return address;
}

Result<ResolvedAddress> AddressResolver::Resolve(
	const AddressData& address,
	const ResolvedSignature& baseSignature
) const {
	if (!baseSignature) {
		return MakeError("Base signature is invalid");
	}

	auto finalAddr = ApplyIndirections(
		baseSignature.address,
		address.indirections
	);

	if (!finalAddr) {
		return MakeError(std::move(finalAddr.error()));
	}

	return ResolvedAddress{*finalAddr, address.name, address.baseSignature};
}

Result<ResolvedAddress> AddressResolver::ResolveByName(
	std::string_view name,
	const ConfigLoader& loader,
	const std::function<std::optional<ResolvedSignature>(std::string_view)>& sigLookup
) const {
	auto addrData = loader.GetAddress(name);
	if (!addrData) {
		return MakeError("Address not found in config: {}", name);
	}

	auto baseSig = sigLookup(addrData->baseSignature);
	if (!baseSig) {
		return MakeError("Base signature not resolved: {}", addrData->baseSignature);
	}

	return Resolve(*addrData, *baseSig);
}

Result<Memory> AddressResolver::ApplyIndirections(
	Memory baseAddress,
	const plg::vector<IndirectionStep>& steps
) const {
	Memory current = baseAddress;

	for (size_t i = 0; i < steps.size(); ++i) {
		if (!current || current < VALID_MINIMUM_ADDRESS) {
			return MakeError("Invalid address in indirection chain");
		}

		auto result = ApplyStep(current, steps[i]);
		if (!result) {
			return result;
		}

		current = *result;
	}

	return current;
}

Result<Memory> AddressResolver::ApplyStep(Memory current, const IndirectionStep& step) const {
	switch (step.type) {
		case IndirectionStep::Type::Offset:
			return current.Offset(step.offset);

		case IndirectionStep::Type::Dereference:
			return current.Offset(step.offset).DerefSelf();

		case IndirectionStep::Type::RelativeOffset: {
			auto target = current.Offset(step.offset);
			if (!target || target < VALID_MINIMUM_ADDRESS) {
				return MakeError("Invalid relative offset target");
			}

			return current.Offset(step.offset)
				.Offset(sizeof(int32_t))
				.Offset(target.Get<int32_t>());
		}
	}

	return MakeError("Unknown indirection type");
}

Result<ResolvedVTable> VTableResolver::Resolve(const VTableData& vtable) const {
	auto& provider = GameConfigManager::Instance().GetModuleProvider();
	auto module = provider.GetModule(vtable.library);
	if (!module) {
		return MakeError("Module not found: {}", vtable.library);
	}

	Result<Memory> addrResult = ResolveTable(module, vtable.name);

	if (!addrResult) {
		return MakeError(std::move(addrResult.error()));
	}

	return ResolvedVTable{*addrResult, vtable.name};
}

Result<ResolvedVTable> VTableResolver::ResolveByName(
	std::string_view name,
	const ConfigLoader& loader
) const {
	auto vtData = loader.GetVTable(name);
	if (!vtData) {
		return MakeError("VTable not found in config: {}", name);
	}

	return Resolve(*vtData);
}

Result<Memory> VTableResolver::ResolveTable(const std::shared_ptr<Module>& module, std::string_view table) const {
	auto address = module->GetVirtualTableByName(table);
	if (!address) {
		return MakeError("VTable not found: {}", table);
	}
	return address;
}

GameConfigManager& GameConfigManager::Instance() {
	static GameConfigManager instance;
	return instance;
}

GameConfigManager::GameConfigManager() {
	m_moduleProvider.PreloadModules();
}

Result<uint32_t> GameConfigManager::LoadConfig(LoadOptions options) {
	std::unique_lock lock(m_mutex);

	// Check for existing config with same paths
	for (const auto& [id, entry] : m_configs) {
		const auto& existingPaths = entry.config->GetOptions().configPaths;

		bool matchFound = false;
		for (const auto& optPath : options.configPaths) {
			for (const auto& existPath : existingPaths) {
				if (existPath.ends_with(optPath)) {
					matchFound = true;
					break;
				}
			}
			if (matchFound) break;
		}

		if (matchFound) {
			// Reuse existing config
			const_cast<ConfigEntry&>(entry).refCount++;
			return id;
		}
	}

	// Create new config
	auto config = std::make_unique<GameConfig>(std::move(options));

	if (auto result = config->Initialize(); !result) {
		return MakeError(std::move(result.error()));
	}

	config->ApplyAllPatches();

	uint32_t id = m_nextId++;
	m_configs.emplace(id, ConfigEntry{std::move(config)});

	return id;
}

void GameConfigManager::UnloadConfig(uint32_t id) {
	std::unique_lock lock(m_mutex);

	auto it = m_configs.find(id);
	if (it != m_configs.end()) {
		if (--it->second.refCount == 0) {
			m_configs.erase(it);
		}
	}
}

GameConfig* GameConfigManager::GetConfig(uint32_t id) {
	std::shared_lock lock(m_mutex);

	auto it = m_configs.find(id);
	if (it != m_configs.end()) {
		return it->second.config.get();
	}

	return nullptr;
}

ModuleProvider& GameConfigManager::GetModuleProvider() {
	return m_moduleProvider;
}

PatchManager& GameConfigManager::GetPatchManager() {
	return m_patchManager;
}

#undef GC_TRY_VOID
