#pragma once

#include <dynlibutils/vthook.hpp>
#include <dynlibutils/memaddr.hpp>
#include <plugify-configs/plugify-configs.hpp>

using Memory = DynLibUtils::CMemory;
using MemoryProtection = DynLibUtils::VirtualUnprotector;

enum class PatchState {
	NotApplied,
	Applied,
	Failed,
	Restored
};

struct PatchBytes {
	plg::vector<uint8_t> bytes;

	PatchBytes() = default;
	explicit PatchBytes(plg::vector<uint8_t> data) : bytes(std::move(data)) {}

	static Result<PatchBytes> FromPattern(std::string_view pattern);
	plg::string ToString() const;

	size_t Size() const { return bytes.size(); }
	const uint8_t* Data() const { return bytes.data(); }
};

struct AppliedPatch {
	plg::string name;
	Memory address;
	PatchBytes originalBytes;
	PatchBytes patchBytes;
	PatchState state;
	plg::string error;

	AppliedPatch()
		: state(PatchState::NotApplied) {}

	AppliedPatch(plg::string n, Memory addr, PatchBytes orig, PatchBytes patch)
		: name(std::move(n))
		, address(addr)
		, originalBytes(std::move(orig))
		, patchBytes(std::move(patch))
		, state(PatchState::NotApplied) {}

	bool IsApplied() const { return state == PatchState::Applied; }
	bool IsFailed() const { return state == PatchState::Failed; }
};

struct PatchOptions {
	bool saveOriginalBytes = true;  // Save bytes for restore
	bool verifyAfterApply = true;   // Verify patch was applied
	bool allowPartialApply = false; // Continue if some patches fail
	bool handleProtection = true;   // Automatically handle memory protection
};

class PatchManager {
public:
	PatchManager() = default;
	~PatchManager();

	// Apply patches
	Result<void> ApplyPatch(std::string_view name, Memory address, const PatchBytes& patchBytes, const PatchOptions& options = {});
	Result<void> ApplyPatch(std::string_view name, Memory address, std::string_view pattern, const PatchOptions& options = {});

	// Restore patches
	Result<void> RestorePatch(std::string_view name);
	Result<void> RestoreAll();

	// Batch operations
	//Result<void> ApplyMultiple(const plg::vector<std::pair<plg::string, Memory>>& patches, const PatchOptions& options = {});
	//Result<void> RestoreMultiple(const plg::vector<plg::string>& names);

	// Query patch state
	bool IsPatchApplied(std::string_view name) const;
	std::optional<PatchState> GetPatchState(std::string_view name) const;
	std::optional<AppliedPatch> GetPatchInfo(std::string_view name) const;

	// Get all patches
	plg::vector<plg::string> GetAppliedPatches() const;
	plg::vector<plg::string> GetFailedPatches() const;
	size_t GetPatchCount() const;

	// Verification
	Result<bool> VerifyPatch(std::string_view name) const;
	Result<void> VerifyAll() const;

	// Statistics
	struct Stats {
		size_t totalPatches = 0;
		size_t appliedPatches = 0;
		size_t failedPatches = 0;
		size_t restoredPatches = 0;
		size_t totalBytesPatched = 0;
	};

	Stats GetStats() const;

	// Clear all patches (restores first if applied)
	void Clear();

private:
	Result<PatchBytes> ReadOriginalBytes(Memory address, size_t size) const;
	Result<void> WriteBytes(Memory address, const PatchBytes& bytes, bool handleProtection);
	Result<bool> VerifyBytes(Memory address, const PatchBytes& expected) const;

private:
	plg::flat_hash_map<plg::string, AppliedPatch, plg::string_hash, std::equal_to<>> m_patches;
	mutable std::shared_mutex m_mutex;
};