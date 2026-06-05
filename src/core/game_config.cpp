#include <core/sdk/utils.hpp>

#include "game_config.hpp"

Module* ModuleProvider::GetModule(std::string_view name) {
	// Try to find existing module
	{
		std::shared_lock lock(m_mutex);
		auto it = m_modules.find(name);
		if (it != m_modules.end()) {
			return it->second.get();
		}
	}

	// Load module
	std::unique_lock lock(m_mutex);
	return m_modules.emplace(name, std::make_unique<Module>(name)).first->second.get();
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

	auto config = configs::Config(m_options.configPaths);
	if (auto error = config.GetError(); !error.empty()) {
		return MakeError("Reading error: {}", error);
	}

	// Jump to game-specific section
	if (!config.JumpKey(gameName)) {
		return MakeError("Game section not found: {}", gameName);
	}

	// Load each section
	struct SectionLoader {
		const char* name;
		Result<void> (ConfigLoader::*loader)(configs::Config&);
	};

	constexpr SectionLoader loaders[] = {
		{"Signatures", &ConfigLoader::LoadSignatures},
		{"Offsets", &ConfigLoader::LoadOffsets},
		{"Patches", &ConfigLoader::LoadPatches},
		{"Addresses", &ConfigLoader::LoadAddresses},
		{"VTables", &ConfigLoader::LoadVTables},
	};

	bool hasErrors = false;
	for (const auto& [name, loader] : loaders) {
		if (auto result = (this->*loader)(config); !result) {
			if (m_options.strictMode) {
				return result;
			}
			plg::print(LS_WARNING, "{} loading failed: {}\n", name, result.error());
			hasErrors = true;
		}
	}

	if (hasErrors && m_options.strictMode) {
		return MakeError("Configuration loading completed with errors");
	}

	return {};
}

Result<void> ConfigLoader::LoadSignatures(configs::Config& config) {
	if (!config.JumpKey("Signatures")) {
		// No signatures section is okay
		return {};
	}

	if (config.IsObject() && config.JumpFirst()) {
		do {
			if (config.IsObject()) {
				auto name = config.GetName();
				auto result = ParseSignatureConfig(config, name);
				if (result) {
					m_signatures[std::move(name)] = std::move(*result);
				} else {
					plg::print(LS_WARNING, "Failed to parse signature '{}': {}\n",
							   name, result.error());
				}
			}
		} while (config.JumpNext());
		config.JumpBack();
	}
	config.JumpBack();

	return {};
}

Result<void> ConfigLoader::LoadOffsets(configs::Config& config) {
	if (!config.JumpKey("Offsets")) {
		return {};
	}

	if (config.IsObject() && config.JumpFirst()) {
		do {
			if (config.IsObject()) {
				auto name = config.GetName();
				auto result = ParseOffsetsConfig(config, name);
				if (result) {
					m_offsets[std::move(name)] = std::move(*result);
				} else {
					plg::print(LS_WARNING, "Failed to parse offset '{}': {}\n",
							   name, result.error());
				}
			}
		} while (config.JumpNext());
		config.JumpBack();
	}
	config.JumpBack();

	return {};
}

Result<void> ConfigLoader::LoadVTables(configs::Config& config) {
	if (!config.JumpKey("VTables")) {
		return {};
	}

	if (config.IsObject() && config.JumpFirst()) {
		do {
			if (config.IsObject()) {
				auto name = config.GetName();
				auto result = ParseVTableConfig(config, name);
				if (result) {
					m_vtables[std::move(name)] = std::move(*result);
				} else {
					plg::print(LS_WARNING, "Failed to parse vtable '{}': {}\n",
							   name, result.error());
				}
			}
		} while (config.JumpNext());
		config.JumpBack();
	}
	config.JumpBack();

	return {};
}

Result<void> ConfigLoader::LoadPatches(configs::Config& config) {
	if (!config.JumpKey("Patches")) {
		return {};
	}

	if (config.IsObject() && config.JumpFirst()) {
		do {
			if (config.IsObject()) {
				auto name = config.GetName();
				auto result = ParsePatchConfig(config, name);
				if (result) {
					m_patches[std::move(name)] = std::move(*result);
				} else {
					plg::print(LS_WARNING, "Failed to parse patch '{}': {}\n",
							   name, result.error());
				}
			}
		} while (config.JumpNext());
		config.JumpBack();
	}
	config.JumpBack();

	return {};
}

Result<void> ConfigLoader::LoadAddresses(configs::Config& config) {
	if (!config.JumpKey("Addresses")) {
		return {};
	}

	if (config.IsObject() && config.JumpFirst()) {
		do {
			if (config.IsObject()) {
				auto name = config.GetName();
				auto result = ParseAddressConfig(config, name);
				if (result) {
					m_addresses[std::move(name)] = std::move(*result);
				} else {
					plg::print(LS_WARNING, "Failed to parse address '{}': {}\n",
							   name, result.error());
				}
			}
		} while (config.JumpNext());
		config.JumpBack();
	}
	config.JumpBack();

	return {};
}

Result<SignatureData> ConfigLoader::ParseSignatureConfig(configs::Config& config, std::string_view name) {
	SignatureData sig;
	sig.name = name;
	sig.library = config.GetString("library");
	sig.value = config.GetString(S2SDK_PLATFORM);

	// Parse references chain
	if (config.JumpKey("refs")) {
		if (config.IsArray() && config.JumpFirst()) {
			do {
				if (config.IsObject() && config.JumpFirst()) {
					auto type = config.GetName();
					auto value = config.GetString("##this##");

					if (type == "string") {
						sig.refs.emplace_back(ReferenceInfo::Type::String, std::move(value));
					} else if (type == "cvar") {
						sig.refs.emplace_back(ReferenceInfo::Type::CVar, std::move(value));
					} else if (type == "vtable") {
						sig.refs.emplace_back(ReferenceInfo::Type::VTable, std::move(value));
					} else {
						config.JumpBack(); config.JumpBack(); config.JumpBack();
						return MakeError("Unknown reference type: {}", type);
					}

					config.JumpBack();
				}
			} while (config.JumpNext());
			config.JumpBack();
		}
		config.JumpBack();
	}

	if (sig.value.empty() && sig.refs.empty()) {
		return MakeError("No signature for " S2SDK_PLATFORM ": {}", sig.name);
	}

	bool onlyVtable = !sig.refs.empty() && std::ranges::all_of(sig.refs, [](const ReferenceInfo& ref) {
		return ref.type == ReferenceInfo::Type::VTable;
	});
	if (onlyVtable) {
		return MakeError("No other references except vtables, add strings or cvars!");
	}

	// Determine type
	if (sig.value[0] == '@') {
		sig.type = SignatureData::Type::Symbol;
		sig.value.erase(0, 1);
	} else {
		sig.type = SignatureData::Type::Pattern;
	}

	return sig;
}

Result<AddressData> ConfigLoader::ParseAddressConfig(configs::Config& config, std::string_view name) {
	AddressData addr;
	addr.name = name;
	if (config.HasKey("signature")) {
		addr.base = config.GetString("signature");
		addr.type = AddressData::Type::Signature;
	} else if (config.HasKey("address")) {
		addr.base = config.GetString("address");
		addr.type = AddressData::Type::Address;
	} else if (config.HasKey("vtable")) {
		addr.base = config.GetString("vtable");
		addr.type = AddressData::Type::VTable;
	}

	if (addr.base.empty()) {
		return MakeError("Missing base");
	}

	// Parse indirection chain
	if (config.JumpKey(S2SDK_PLATFORM)) {
		if (config.IsArray() && config.JumpFirst()) {
			do {
				if (config.IsObject() && config.JumpFirst()) {
					auto type = config.GetName();
					auto value = static_cast<int32_t>(config.GetAsInt());

					if (type == "offset") {
						addr.steps.emplace_back(IndirectionStep::Type::Offset, std::move(value));
					} else if (type == "read") {
						addr.steps.emplace_back(IndirectionStep::Type::Dereference, std::move(value));
					}  else if (type == "index") {
						addr.steps.emplace_back(IndirectionStep::Type::Index, std::move(value));
					} else if (type == "read_offs32") {
						addr.steps.emplace_back(IndirectionStep::Type::Relative, std::move(value));
					} else {
						config.JumpBack(); config.JumpBack(); config.JumpBack();
						return MakeError("Unknown indirection type: {}", type);
					}

					config.JumpBack();
				}
			} while (config.JumpNext());
			config.JumpBack();
		}
		config.JumpBack();
	}

	return addr;
}

Result<OffsetData> ConfigLoader::ParseOffsetsConfig(configs::Config& config, std::string_view name) {
	OffsetData offset;
	offset.name = name;
	if (config.HasKey(S2SDK_PLATFORM)) {
		offset.value = static_cast<int32_t>(config.GetAsInt(S2SDK_PLATFORM));
	}

	if (!offset.value) {
		return MakeError("No offset for " S2SDK_PLATFORM ": {}", offset.name);
	}

	return offset;
}

Result<VTableData> ConfigLoader::ParseVTableConfig(configs::Config& config, std::string_view name) {
	VTableData vt;
	vt.name = name;
	vt.library = config.GetString("library");
	vt.table = config.GetString("table");

	if (vt.library.empty()) {
		return MakeError("No library for " S2SDK_PLATFORM ": {}", vt.name);
	}

	if (vt.table.empty()) {
		return MakeError("No table for " S2SDK_PLATFORM ": {}", vt.name);
	}

	return vt;
}

Result<PatchData> ConfigLoader::ParsePatchConfig(configs::Config& config, std::string_view name) {
	PatchData patch;
	patch.name = name;
	if (config.HasKey("signature")) {
		patch.base = config.GetString("signature");
		patch.type = PatchData::Type::Signature;
	} else if (config.HasKey("address")) {
		patch.base = config.GetString("address");
		patch.type = PatchData::Type::Address;
	} else if (config.HasKey("vtable")) {
		patch.base = config.GetString("vtable");
		patch.type = PatchData::Type::VTable;
	}
	patch.pattern = config.GetString(S2SDK_PLATFORM);

	if (patch.base.empty()) {
		return MakeError("No address for " S2SDK_PLATFORM ": {}", patch.name);
	}

	if (patch.pattern.empty()) {
		return MakeError("No pattern for " S2SDK_PLATFORM ": {}", patch.name);
	}

	return patch;
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
	std::for_each(m_options.processStrategy, m_signatures.begin(), m_signatures.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

template <typename Fn>
void ConfigLoader::ForEachAddress(Fn&& fn) const  {
	std::for_each(m_options.processStrategy, m_addresses.begin(), m_addresses.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

template <typename Fn>
void ConfigLoader::ForEachVTable(Fn&& fn) const  {
	std::for_each(m_options.processStrategy, m_vtables.begin(), m_vtables.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

template <typename Fn>
void ConfigLoader::ForEachOffset(Fn&& fn) const  {
	std::for_each(m_options.processStrategy, m_offsets.begin(), m_offsets.end(), [&](const auto& kv) {
		fn(kv.second);
	});
}

template <typename Fn>
void ConfigLoader::ForEachPatch(Fn&& fn) const  {
	std::for_each(m_options.processStrategy, m_patches.begin(), m_patches.end(), [&](const auto& kv) {
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
			GC_TRY_VOID(PreloadVTables());
			GC_TRY_VOID(PreloadAddresses());
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

	auto addrLookup = [&](const AddressData& addr) -> std::optional<Memory> {
		switch (addr.type) {
			case AddressData::Type::Address: {
				// Get or resolve base signature
				if (auto baseAddr = m_cache.GetAddress(addr.base)) {
					return baseAddr->address;
				}
				if (auto addrResult = ResolveAddressInternal(addr.base)) {
					return addrResult->address;
				} else {
					plg::print(LS_WARNING, "Failed to resolve base address '{}' for address '{}'\n",
							   addr.base, addr.name);
					m_cache.CacheAddress(ResolvedAddress::Failed(addr.name, addrResult.error()));
					hadErrors.test_and_set(std::memory_order_relaxed);
				}
				break;
			}
			case AddressData::Type::Signature: {
				// Get or resolve base signature
				if (auto baseSig = m_cache.GetSignature(addr.base)) {
					return baseSig->address;
				}
				if (auto sigResult = ResolveSignatureInternal(addr.base)) {
					return sigResult->address;
				} else {
					plg::print(LS_WARNING, "Failed to resolve base signature '{}' for address '{}'\n",
							   addr.base, addr.name);
					m_cache.CacheAddress(ResolvedAddress::Failed(addr.name, sigResult.error()));
					hadErrors.test_and_set(std::memory_order_relaxed);
				}
				break;
			}
			case AddressData::Type::VTable: {
				// Get or resolve base vtable
				if (auto baseVt = m_cache.GetVTable(addr.base)) {
					return baseVt->address;
				}
				if (auto vtResult = ResolveVTableInternal(addr.base)) {
					return vtResult->address;
				} else {
					plg::print(LS_WARNING, "Failed to resolve base vt '{}' for address '{}'\n",
							   addr.base, addr.name);
					m_cache.CacheAddress(ResolvedAddress::Failed(addr.name, vtResult.error()));
					hadErrors.test_and_set(std::memory_order_relaxed);
				}
				break;
			}
			default:
				break;
		}
		return std::nullopt;
	};

	m_loader.ForEachAddress([&](const AddressData& addr) {
		auto baseAddr = addrLookup(addr);
		if (!baseAddr)
			return;

		auto result = m_addressResolver.Resolve(addr, *baseAddr);
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
		auto result = ApplyPatch(patch, options);
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
	auto& patchManager = g_GameConfigManager.GetPatchManager();
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
	auto& patchManager = g_GameConfigManager.GetPatchManager();
	return patchManager.IsPatchApplied(name);
}

PatchManager::Stats GameConfig::GetPatchStats() const {
	auto& patchManager = g_GameConfigManager.GetPatchManager();
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
		[&](std::string_view name, AddressData::Type type) -> Result<Memory> {
			switch (type) {
				case AddressData::Type::Signature:
					if (auto sigResult = ResolveSignatureInternal(name)) {
						return sigResult->address;
					} else {
						return MakeError(std::move(sigResult.error()));
					}
				case AddressData::Type::Address:
					if (auto addrResult = ResolveAddressInternal(name)) {
						return addrResult->address;
					} else {
						return MakeError(std::move(addrResult.error()));
					}
				case AddressData::Type::VTable:
					if (auto vtResult = ResolveVTableInternal(name)) {
						return vtResult->address;
					} else {
						return MakeError(std::move(vtResult.error()));
					}
				default:
					return MakeError("Invalid address type");
			}
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
	const PatchData& patch,
	const PatchOptions& options
) {
	// Get patch pattern from config
	auto patchResult = GetPatch(patch.name);
	if (!patchResult) {
		return MakeError(std::move(patchResult.error()));
	}

	auto addrLookup = [&] -> Result<Memory> {
		switch (patch.type) {
			case PatchData::Type::Address: return GetAddress(patch.base);
			case PatchData::Type::Signature: return GetSignature(patch.base);
			case PatchData::Type::VTable: return GetVTable(patch.base);
			default: return MakeError("Invalid address type");
		}
	};

	// Get target address
	auto addrResult = addrLookup();
	if (!addrResult) {
		return MakeError(std::move(addrResult.error()));
	}

	// Apply the patch
	auto& patchManager = g_GameConfigManager.GetPatchManager();
	return patchManager.ApplyPatch(patch.name, *addrResult, *patchResult, options);
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

	return *offset->value;
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
	auto vtResult = PreloadVTables();
	if (!vtResult) {
		plg::print(LS_WARNING, "Cannot preload - {}\n", vtResult.error());
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

Result<ResolvedSignature> SignatureResolver::Resolve(
	const SignatureData& signature
) const {
	auto& provider = g_GameConfigManager.GetModuleProvider();
	auto* module = provider.GetModule(signature.library);
	if (!module) {
		return MakeError("Module not found: {}", signature.library);
	}

	Result<Memory> addrResult = signature.IsSymbol() ?
		ResolveSymbol(*module, signature.value) :
		ResolvePattern(*module, signature.value);

	if (!addrResult && !signature.refs.empty()) {
		addrResult = ResolveReferences(*module, signature.refs);
	}

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

Result<Memory> SignatureResolver::ResolvePattern(
	const Module& module,
	std::string_view pattern
) const {
	if (pattern.empty()) {
		return MakeError("No pattern provided");
	}

	auto result = module.FindPatternMulti(pattern);
	if (result.empty()) {
		return MakeError("Pattern not found: {}", pattern);
	}

	if (result.size() > 1) {
		return MakeError("Ambiguous: {} functions match pattern", result.size());
	}

	return result.front();
}

Result<Memory> SignatureResolver::ResolveSymbol(
	const Module& module,
	std::string_view symbol
) const {
	if (symbol.empty()) {
		return MakeError("No symbol provided");
	}

	auto address = module.GetFunctionByName(symbol);
	if (!address) {
		return MakeError("Symbol not found: {}", symbol);
	}
	return address;
}

Result<Memory> SignatureResolver::ResolveReferences(
	const Module& module,
	std::span<const ReferenceInfo> refs
) const {
	bool vtable = false;
	auto result = module.FindAllFunctionsFromRefs(refs, [&](const ReferenceInfo& ref) -> Result<std::vector<Memory>> {
		switch (ref.type) {
			case ReferenceInfo::Type::String:
				return module.FindStringMulti(ref.name, false, true);

			case ReferenceInfo::Type::VTable:
				vtable = true;
				return {};

			case ReferenceInfo::Type::CVar: {
				auto conVarRef = g_pCVar->FindConVar(ref.name.data(), true);
				if (!conVarRef.IsValidRef()) {
					return MakeError("ConVarRef not found: {}", ref.name);
				}
				auto conVarData = g_pCVar->GetConVarData(conVarRef);
				if (!conVarData) {
					return MakeError("ConVarData not found: {}", ref.name);
				}
				return module.FindPtrs(reinterpret_cast<uintptr_t>(conVarData));
			}
			default:
				return MakeError("Invalid reference type");
		}
	}, [&](const ReferenceInfo& ref) -> std::string_view { return ref.name; });

	if (!result) {
		return MakeError(std::move(result.error()));
	}

	if (result->empty()) {
		return MakeError("No function found matching provided references.");
	}

	if (result->size() > 1) {
		if (vtable) {
			std::vector<std::vector<CAddress>> funcs;
			funcs.push_back(std::move(*result));

			for (const auto& [type, name] : refs) {
				if (type == ReferenceInfo::Type::VTable) {
					funcs.push_back(module.GetVFunctionsFromVTable(name));
				}
			}

			std::vector<CAddress> candidates = std::move(funcs[0]);

			for (size_t i = 1; i < funcs.size(); ++i) {
				const auto& next_funcs = funcs[i];
				std::vector<CAddress> intersection;
				intersection.reserve(std::min(candidates.size(), next_funcs.size()));
				std::ranges::set_intersection(candidates, next_funcs, std::back_inserter(intersection));
				std::swap(candidates, intersection);
			}

			if (candidates.empty()) {
				return MakeError("No function found matching provided references.");
			}

			if (candidates.size() > 1) {
				return MakeError("Ambiguous: {} functions match provided references.", candidates.size());
			}

			return candidates.front();
		}

		return MakeError("Ambiguous: {} functions match provided references.", result->size());
	}

	return result->front();
}

Result<ResolvedAddress> AddressResolver::Resolve(
	const AddressData& address,
	Memory baseAddress
) const {
	if (!baseAddress) {
		return MakeError("Base address is invalid");
	}

	auto finalAddr = ApplyIndirections(baseAddress, address.steps);

	if (!finalAddr) {
		return MakeError(std::move(finalAddr.error()));
	}

	return ResolvedAddress{*finalAddr, address.name, address.base};
}

Result<ResolvedAddress> AddressResolver::ResolveByName(
	std::string_view name,
	const ConfigLoader& loader,
	const std::function<Result<Memory>(std::string_view, AddressData::Type)>& addressLookup
) const {
	auto addrData = loader.GetAddress(name);
	if (!addrData) {
		return MakeError("Address not found in config: {}", name);
	}

	auto baseAddress = addressLookup(addrData->base, addrData->type);
	if (!baseAddress) {
		return MakeError("Base signature {} not resolved: {}", addrData->base, baseAddress.error());
	}

	return Resolve(*addrData, *baseAddress);
}

Result<Memory> AddressResolver::ApplyIndirections(
	Memory baseAddress,
	std::span<const IndirectionStep> steps
) const {
	Memory current = baseAddress;

	for (size_t i = 0; i < steps.size(); ++i) {
		if (!current.IsValid()) {
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

		case IndirectionStep::Type::Index:
			return current.Deref(1, step.offset * sizeof(uintptr_t));

		case IndirectionStep::Type::Dereference:
			return current.Deref(1, step.offset);

		case IndirectionStep::Type::Relative: {
			auto target = current.Offset(step.offset);
			if (!target.IsValid()) {
				return MakeError("Invalid relative offset target");
			}

			return current.Offset(step.offset)
				.Offset(sizeof(int32_t))
				.Offset(target.Get<int32_t>());
		}
		default:
			break;
	}

	return MakeError("Unknown indirection type");
}

Result<ResolvedVTable> VTableResolver::Resolve(const VTableData& vtable) const {
	auto& provider = g_GameConfigManager.GetModuleProvider();
	auto* module = provider.GetModule(vtable.library);
	if (!module) {
		return MakeError("Module not found: {}", vtable.library);
	}

	Result<Memory> addrResult = ResolveTable(*module, vtable.table);
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

Result<Memory> VTableResolver::ResolveTable(const Module& module, std::string_view table) const {
	if (table.empty()) {
		return MakeError("No table provided");
	}

	auto address = module.GetVirtualTableByName(table);
	if (!address) {
		return MakeError("VTable not found: {}", table);
	}
	return address;
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

	if (auto result = config->ApplyAllPatches(); !result) {
		return MakeError(std::move(result.error()));
	}

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

plg::vector<GameConfig*> GameConfigManager::GetConfigs() const {
	plg::vector<GameConfig*> configs;
	configs.reserve(m_configs.size());
	std::shared_lock lock(m_mutex);
	for (const auto& [_, entry] : m_configs) {
		configs.push_back(entry.config.get());
	}
	return configs;
}

#undef GC_TRY_VOID
