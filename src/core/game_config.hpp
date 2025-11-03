#pragma once

#include <core/sdk/utils.hpp>
#include <dynlibutils/memaddr.hpp>
#include <dynlibutils/module.hpp>
#include <plugify-configs/plugify-configs.hpp>

#include "patch_manager.hpp"

using Memory = DynLibUtils::CMemory;
using Module = DynLibUtils::CAssemblyModule<std::shared_mutex>;

struct SignatureData {
	enum class Type { Pattern, Symbol };

	Type type;
	plg::string value;      // Pattern string or symbol name
	plg::string library;    // Module/library name
	plg::string name;       // Identifier name

	bool IsSymbol() const { return type == Type::Symbol; }
	bool IsPattern() const { return type == Type::Pattern; }
};

struct IndirectionStep {
	enum class Type {
		Offset,           // Simple offset
		Dereference,      // Dereference pointer
		RelativeOffset    // Relative offset (RIP-relative on x64)
	};

	Type type;
	int32_t offset;

	static IndirectionStep MakeOffset(int32_t off) {
		return { Type::Offset, off };
	}

	static IndirectionStep MakeDereference(int32_t off = 0) {
		return { Type::Dereference, off };
	}

	static IndirectionStep MakeRelative(int32_t off) {
		return { Type::RelativeOffset, off };
	}
};

struct AddressData {
	plg::string name;
	plg::string baseSignature;  // Reference to a signature
	plg::vector<IndirectionStep> indirections;
};

struct OffsetData {
	plg::string name;
	int32_t value;
};

struct VTableData {
	plg::string name;
	plg::string library;
	plg::string table;
};

struct PatchData {
	plg::string name;
	plg::string baseAddress; // Reference to an address
	plg::string pattern;
};

enum class CacheStrategy {
	Eager,      // Resolve everything on Initialize()
	Lazy,       // Resolve on first access
	Hybrid      // Resolve signatures eagerly, addresses lazily
};

struct LoadOptions {
	plg::vector<plg::string> configPaths;
	plg::string gameDirectory = S2SDK_GAME_NAME;
	CacheStrategy cacheStrategy = CacheStrategy::Eager;
	std::execution::parallel_policy processStrategy = std::execution::par;
	bool strictMode = false;
	bool validateOnLoad = true;
	bool allowPartialApply = false;
};

class ConfigLoader {
public:
	ConfigLoader(LoadOptions& options)
		: m_options(options) {};
	~ConfigLoader() = default;

	// Load configuration from files
	Result<void> Load();

	// Access loaded data
	std::optional<SignatureData> GetSignature(std::string_view name) const;
	std::optional<AddressData> GetAddress(std::string_view name) const;
	std::optional<OffsetData> GetOffset(std::string_view name) const;
	std::optional<VTableData> GetVTable(std::string_view name) const;
	std::optional<PatchData> GetPatch(std::string_view name) const;

	// Iterate over all entries
	template <typename Fn>
	void ForEachSignature(Fn&& fn) const;
	template <typename Fn>
	void ForEachAddress(Fn&& fn) const;
	template <typename Fn>
	void ForEachVTable(Fn&& fn) const;
	template <typename Fn>
	void ForEachOffset(Fn&& fn) const;
	template <typename Fn>
	void ForEachPatch(Fn&& fn) const;

	// Query methods
	size_t GetSignatureCount() const { return m_signatures.size(); }
	size_t GetAddressCount() const { return m_addresses.size(); }
	size_t GetOffsetCount() const { return m_offsets.size(); }
	size_t GetVTableCount() const { return m_vtables.size(); }
	size_t GetPatchCount() const { return m_patches.size(); }

private:
	Result<void> ParseConfigFile(std::string_view gameName);
	Result<void> LoadSignatures(pcf::Config* config);
	Result<void> LoadAddresses(pcf::Config* config);
	Result<void> LoadOffsets(pcf::Config* config);
	Result<void> LoadVTables(pcf::Config* config);
	Result<void> LoadPatches(pcf::Config* config);
	Result<AddressData> ParseAddressConfig(pcf::Config* config, std::string_view name);

private:
	LoadOptions& m_options;

	plg::flat_hash_map<plg::string, SignatureData, plg::string_hash, std::equal_to<>> m_signatures;
	plg::flat_hash_map<plg::string, AddressData, plg::string_hash, std::equal_to<>> m_addresses;
	plg::flat_hash_map<plg::string, OffsetData, plg::string_hash, std::equal_to<>> m_offsets;
	plg::flat_hash_map<plg::string, VTableData, plg::string_hash, std::equal_to<>> m_vtables;
	plg::flat_hash_map<plg::string, PatchData, plg::string_hash, std::equal_to<>> m_patches;
};

struct ResolvedSignature {
	Memory address;
	plg::string name;
	plg::string error;

	ResolvedSignature() : address(nullptr) {}
	ResolvedSignature(Memory addr, plg::string n)
		: address(addr), name(std::move(n)) {}

	static ResolvedSignature Failed(plg::string name, plg::string error) {
		ResolvedSignature result;
		result.name = std::move(name);
		result.error = std::move(error);
		return result;
	}

	explicit operator bool() const { return address; }
};

struct ResolvedAddress {
	Memory address;
	plg::string name;
	plg::string baseSignature;
	plg::string error;

	ResolvedAddress() : address(nullptr) {}
	ResolvedAddress(Memory addr, plg::string n, plg::string base)
		: address(addr), name(std::move(n)), baseSignature(std::move(base)) {}

	static ResolvedAddress Failed(plg::string name, plg::string error) {
		ResolvedAddress result;
		result.name = std::move(name);
		result.error = std::move(error);
		return result;
	}

	explicit operator bool() const { return address; }
};

struct ResolvedVTable {
	Memory address;
	plg::string name;
	plg::string error;

	ResolvedVTable() : address(nullptr) {}
	ResolvedVTable(Memory addr, plg::string n)
		: address(addr), name(std::move(n)) {}

	static ResolvedVTable Failed(plg::string name, plg::string error) {
		ResolvedVTable result;
		result.name = std::move(name);
		result.error = std::move(error);
		return result;
	}

	explicit operator bool() const { return address; }
};

class ModuleProvider {
public:
	ModuleProvider();
	~ModuleProvider() = default;

	std::shared_ptr<Module> GetModule(std::string_view name);
	bool HasModule(std::string_view name) const;
	void PreloadModules();

private:
	std::shared_ptr<Module> LoadModuleInternal(std::string_view name);

private:
	plg::flat_hash_map<plg::string, std::shared_ptr<Module>, plg::string_hash, std::equal_to<>> m_modules;
	mutable std::shared_mutex m_mutex;
};

class SignatureResolver {
public:
	SignatureResolver() = default;
	~SignatureResolver() = default;

	// Resolve a single signature
	Result<ResolvedSignature> Resolve(const SignatureData& signature) const;

	// Resolve a signature by name from loader
	Result<ResolvedSignature> ResolveByName(
		std::string_view name,
		const ConfigLoader& loader
	) const;

private:
	Result<Memory> ResolvePattern(const std::shared_ptr<Module>& module, std::string_view pattern) const;
	Result<Memory> ResolveSymbol(const std::shared_ptr<Module>& module, std::string_view symbol) const;
};

class AddressResolver {
public:
	AddressResolver() = default;
	~AddressResolver() = default;

	// Resolve an address using cached signatures
	Result<ResolvedAddress> Resolve(
		const AddressData& address,
		const ResolvedSignature& baseSignature
	) const;

	// Resolve an address by looking up base signature
	Result<ResolvedAddress> ResolveByName(
		std::string_view name,
		const ConfigLoader& loader,
		const std::function<std::optional<ResolvedSignature>(std::string_view)>& sigLookup
	) const;

private:
	Result<Memory> ApplyIndirections(
		Memory baseAddress,
		const plg::vector<IndirectionStep>& steps
	) const;

	Result<Memory> ApplyStep(Memory current, const IndirectionStep& step) const;

private:
	static constexpr Memory VALID_MINIMUM_ADDRESS{0x10000};
};

class VTableResolver {
public:
	VTableResolver() = default;
	~VTableResolver() = default;

	// Resolve a single vtable
	Result<ResolvedVTable> Resolve(const VTableData& vtable) const;

	// Resolve a vtable by name from loader
	Result<ResolvedVTable> ResolveByName(
		std::string_view name,
		const ConfigLoader& loader
	) const;

private:
	Result<Memory> ResolveTable(const std::shared_ptr<Module>& module, std::string_view table) const;
};

class ConfigCache {
public:
	ConfigCache() = default;
	~ConfigCache() = default;

	// Cache operations
	void CacheSignature(const ResolvedSignature& signature);
	void CacheAddress(const ResolvedAddress& address);
	void CacheVTable(const ResolvedVTable& vtable);

	std::optional<ResolvedSignature> GetSignature(std::string_view name) const;
	std::optional<ResolvedAddress> GetAddress(std::string_view name) const;
	std::optional<ResolvedVTable> GetVTable(std::string_view name) const;

	bool HasSignature(std::string_view name) const;
	bool HasAddress(std::string_view name) const;
	bool HasVTable(std::string_view name) const;

	// Statistics
	template<typename T>
	struct StatsT {
		T cachedSignatures = 0;
		T cachedAddresses = 0;
		T cachedVTables = 0;
		T failedSignatures = 0;
		T failedAddresses = 0;
		T failedVTables = 0;
		T totalLookups = 0;
		T cacheHits = 0;

		void clear() {
			cachedSignatures = 0;
			cachedAddresses = 0;
			cachedVTables = 0;
			failedSignatures = 0;
			failedAddresses = 0;
			failedVTables = 0;
			totalLookups = 0;
			cacheHits = 0;
		}

		template<typename U = T> requires (T::is_always_lock_free)
		StatsT<typename U::value_type> snapshot() const {
			return {
				cachedSignatures.load(std::memory_order_relaxed),
				cachedAddresses.load(std::memory_order_relaxed),
				cachedVTables.load(std::memory_order_relaxed),
				failedSignatures.load(std::memory_order_relaxed),
				failedAddresses.load(std::memory_order_relaxed),
				failedVTables.load(std::memory_order_relaxed),
				totalLookups.load(std::memory_order_relaxed),
				cacheHits.load(std::memory_order_relaxed)
			};
		}
	};
	using Stats = StatsT<size_t>;

	Stats GetStats() const;
	void ResetStats();
	void Clear();

	// Iteration
	template <typename Fn>
	void ForEachSignature(Fn&& fn) const;
	template <typename Fn>
	void ForEachAddress(Fn&& fn) const;
	template <typename Fn>
	void ForEachVTable(Fn&& fn) const;

private:
	mutable StatsT<std::atomic_size_t> m_stats;
	plg::flat_hash_map<plg::string, ResolvedSignature, plg::string_hash, std::equal_to<>> m_signatures;
	plg::flat_hash_map<plg::string, ResolvedAddress, plg::string_hash, std::equal_to<>> m_addresses;
	plg::flat_hash_map<plg::string, ResolvedVTable, plg::string_hash, std::equal_to<>> m_vtables;

	mutable std::shared_mutex m_mutex;
};

class GameConfig {
public:
	explicit GameConfig(LoadOptions options);
	~GameConfig();

	// Initialization
	Result<void> Initialize();
	bool IsInitialized() const { return m_initialized; }

	// Primary API - Get resolved values
	Result<Memory> GetAddress(std::string_view name);
	Result<Memory> GetSignature(std::string_view name);
	Result<Memory> GetVTable(std::string_view name);
	Result<int32_t> GetOffset(std::string_view name);
	Result<plg::string> GetPatch(std::string_view name);
	Result<std::shared_ptr<Module>> GetModule(std::string_view name);

	// Inspection API
	bool HasAddress(std::string_view name) const;
	bool HasSignature(std::string_view name) const;
	bool HasVTable(std::string_view name) const;
	bool HasOffset(std::string_view name) const;
	bool HasPatch(std::string_view name) const;

	// Cache management
	void PreloadAll();  // Eagerly load everything
	void ClearCache();  // Clear cached values (keep config data)
	ConfigCache::Stats GetCacheStats() const;

	// Configuration inspection
	const ConfigLoader& GetLoader() const { return m_loader; }
	const LoadOptions& GetOptions() const { return m_options; }

	// Diagnostics
	struct DiagnosticsInfo {
		bool initialized;
		size_t totalSignatures;
		size_t resolvedSignatures;
		size_t totalAddresses;
		size_t resolvedAddresses;
		size_t totalVTables;
		size_t resolvedVTables;
		plg::vector<plg::string> failedSignatures;
		plg::vector<plg::string> failedAddresses;
		plg::vector<plg::string> failedVTables;
		ConfigCache::Stats cacheStats;
	};

	DiagnosticsInfo GetDiagnostics() const;
	void PrintDiagnostics() const;

	// Apply patch
	Result<void> ApplyPatch(std::string_view name, std::string_view address, const PatchOptions& options = {});
	Result<void> ApplyAllPatches(const PatchOptions& options = {});

	// Restore patches
	Result<void> RestorePatch(std::string_view name);
	Result<void> RestoreAllPatches();

	// Query
	bool IsPatchApplied(std::string_view name) const;
	PatchManager::Stats GetPatchStats() const;

private:
	// Internal resolution methods
	Result<ResolvedSignature> ResolveSignatureInternal(std::string_view name);
	Result<ResolvedAddress> ResolveAddressInternal(std::string_view name);
	Result<ResolvedVTable> ResolveVTableInternal(std::string_view name);

	// Eager loading
	Result<void> PreloadSignatures();
	Result<void> PreloadAddresses();
	Result<void> PreloadVTables();

private:
	LoadOptions m_options;
	bool m_initialized;

	// Components
	ConfigLoader m_loader;
	ConfigCache m_cache;

	PLUGIFY_NO_UNIQUE_ADDRESS SignatureResolver m_signatureResolver;
	PLUGIFY_NO_UNIQUE_ADDRESS AddressResolver m_addressResolver;
	PLUGIFY_NO_UNIQUE_ADDRESS VTableResolver m_vtableResolver;

	mutable std::shared_mutex m_mutex;

	friend class SignatureResolver;
	friend class AddressResolver;
	friend class VTableResolver;
};

class GameConfigManager {
	GameConfigManager();
	~GameConfigManager() = default;
	NONCOPYABLE(GameConfigManager)

public:
	static auto& Instance() {
		static GameConfigManager instance;
		return instance;
	}

	// Config management
	Result<uint32_t> LoadConfig(LoadOptions options);
	void UnloadConfig(uint32_t id);
	GameConfig* GetConfig(uint32_t id);

	ModuleProvider& GetModuleProvider();
	PatchManager& GetPatchManager();

private:
	struct ConfigEntry {
		std::unique_ptr<GameConfig> config;
		size_t refCount;

		explicit ConfigEntry(std::unique_ptr<GameConfig> cfg)
			: config(std::move(cfg)), refCount(1) {}
	};

	plg::flat_hash_map<uint32_t, ConfigEntry> m_configs;
	mutable std::shared_mutex m_mutex;
	uint32_t m_nextId{};

	ModuleProvider m_moduleProvider;
	PatchManager m_patchManager;

	friend class GameConfig;
};
inline GameConfigManager& g_GameConfigManager = GameConfigManager::Instance();

#define TRY_GET_SIGNATURE(gameConfig, name, variable)                                               \
	{                                                                                               \
		auto _result = (gameConfig) -> GetSignature(name);                                          \
		if (!(_result)) {                                                                           \
			plg::print(LS_ERROR, "Failed to resolve signature: " #name " - {}\n", _result.error()); \
		}                                                                                           \
		variable = _result->RCast<decltype(variable)>();                                            \
	}

#define TRY_GET_ADDRESS(gameConfig, name, variable)                                                \
	{                                                                                              \
		auto _result = (gameConfig) -> GetAddress(name);                                           \
		if (!(_result)) {                                                                          \
			plg::print(LS_ERROR, "Failed to resolve address: " #name " - {}\n", _result.error());  \
		}                                                                                          \
		variable = _result->RCast<decltype(variable)>();                                           \
	}

#define TRY_GET_OFFSET(gameConfig, name, variable)                                                 \
	static std::optional<int> variable;                                                            \
	if (!variable) {                                                                               \
		auto _result = (gameConfig) -> GetOffset(name);                                            \
		if (!(_result)) {                                                                          \
			plg::print(LS_ERROR, "Failed to resolve offset: " #name " - {}\n", _result.error());   \
		}                                                                                          \
		variable = *_result;                                                                       \
	}

#define TRY_GET_VTABLE(gameConfig, name, variable)                                                 \
	static Memory variable;                                                                        \
	if (!variable) {                                                                               \
		auto _result = (gameConfig) -> GetVTable(name);                                            \
		if (!(_result)) {                                                                          \
			plg::print(LS_ERROR, "Failed to resolve vtable: " #name " - {}\n", _result.error());   \
		}                                                                                          \
		variable = *_result;                                                                       \
	}
