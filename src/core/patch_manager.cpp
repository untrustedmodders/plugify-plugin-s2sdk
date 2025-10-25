#include "patch_manager.hpp"

Result<PatchBytes> PatchBytes::FromPattern(std::string_view pattern) {
	plg::vector<uint8_t> bytes;
	bytes.reserve(pattern.size() / 3);

	size_t i = 0;
	while (i < pattern.size()) {
		while (i < pattern.size() && std::isspace(static_cast<unsigned char>(pattern[i]))) {
			++i;
		}
		if (i >= pattern.size()) break;
		if (i + 2 > pattern.size()) {
			return MakeError("Incomplete hex byte in pattern");
		}

		uint8_t v = 0;
		auto res = std::from_chars(pattern.data() + i,
								   pattern.data() + i + 2,
								   v, 16);
		if (res.ec != std::errc{}) {
			return MakeError("Invalid hex characters in pattern");
		}

		bytes.push_back(v);
		i += 2;
	}

	if (bytes.empty()) {
		return MakeError("Empty patch pattern");
	}

	return PatchBytes(std::move(bytes));
}

plg::string PatchBytes::ToString() const {
	plg::string result;
	result.reserve(bytes.size() * 3);

	for (size_t i = 0; i < bytes.size(); ++i) {
		if (i > 0) result.push_back(' ');
		std::format_to(std::back_inserter(result), "{:02X}", bytes[i]);
	}

	return result;
}

PatchManager::~PatchManager() {
	// Restore all patches on destruction
	for (auto& [name, patch] : m_patches) {
		if (patch.IsApplied()) {
			auto result = RestorePatch(name);
			if (!result) {
				plg::print(LS_WARNING, "Failed to restore patch '{}': {}\n", patch.name, result.error());
			}
		}
	}
}

Result<PatchBytes> PatchManager::ReadOriginalBytes(Memory address, size_t size) const {
	if (!address || size == 0) {
		return MakeError("Invalid address or size for reading");
	}

	plg::vector<uint8_t> bytes(size);
	std::memcpy(bytes.data(), address.RCast<const void*>(), size);
	return PatchBytes(std::move(bytes));
}

Result<void> PatchManager::WriteBytes(Memory address, const PatchBytes& bytes, bool handleProtection) {
	if (!address) {
		return MakeError("Invalid address for writing");
	}

	if (bytes.Size() == 0) {
		return MakeError("No bytes to write");
	}

	if (handleProtection) {
		// Temporarily make memory writable
		MemoryProtection guard(
			address,
			bytes.Size(),
			DynLibUtils::RWX
		);

		// Temporarily make memory writable
		/*auto guardResult = MemoryProtection::Modify(
			address,
			bytes.Size(),
			MemoryProtection::Protection::ReadWriteExecute
		);*/

		if (!guard.IsValid()) {
			return MakeError("Could not unprotect memory");
		}

		std::memcpy(address.RCast<void*>(), bytes.Data(), bytes.Size());
		// Guard destructor restores protection
	} else {
		std::memcpy(address.RCast<void*>(), bytes.Data(), bytes.Size());
	}

	return {};
}

Result<bool> PatchManager::VerifyBytes(Memory address, const PatchBytes& expected) const {
	if (!address) {
		return MakeError("Invalid address for verification");
	}

	auto currentBytes = ReadOriginalBytes(address, expected.Size());
	if (!currentBytes) {
		return MakeError(std::move(currentBytes.error()));
	}

	return std::memcmp(currentBytes->Data(), expected.Data(), expected.Size()) == 0;
}

Result<void> PatchManager::ApplyPatch(
	std::string_view patchName,
	Memory address,
	const PatchBytes& patchBytes,
	const PatchOptions& options
) {
	std::unique_lock lock(m_mutex);

	// Check if already applied
	auto it = m_patches.find(patchName);
	if (it != m_patches.end() && it->second.IsApplied()) {
		return MakeError("Patch {} already applied", patchName);
	}

	// Read original bytes if requested
	PatchBytes originalBytes;
	if (options.saveOriginalBytes) {
		auto origResult = ReadOriginalBytes(address, patchBytes.Size());
		if (!origResult) {
			return MakeError(std::move(origResult.error()));
		}
		originalBytes = std::move(*origResult);
	}

	// Apply the patch
	auto writeResult = WriteBytes(address, patchBytes, options.handleProtection);
	if (!writeResult) {
		// Mark as failed
		AppliedPatch failed(patchName, address, std::move(originalBytes), patchBytes);
		failed.state = PatchState::Failed;
		failed.error = writeResult.error();
		m_patches[patchName] = std::move(failed);

		return MakeError(std::move(writeResult.error()));
	}

	// Verify if requested
	if (options.verifyAfterApply) {
		auto verifyResult = VerifyBytes(address, patchBytes);
		if (!verifyResult) {
			return MakeError(std::move(verifyResult.error()));
		}
		if (!*verifyResult) {
			AppliedPatch failed(patchName, address, std::move(originalBytes), patchBytes);
			failed.state = PatchState::Failed;
			failed.error = "Verification failed after applying patch";
			m_patches[patchName] = std::move(failed);

			return MakeError("Patch {} verification failed", patchName);
		}
	}

	// Success - store patch info
	AppliedPatch applied(patchName, address, std::move(originalBytes), patchBytes);
	applied.state = PatchState::Applied;
	m_patches[patchName] = std::move(applied);

	plg::print(LS_MESSAGE, "Applied patch '{}' at {}\n", patchName, address.RCast<void*>());

	return {};
}

Result<void> PatchManager::ApplyPatch(
	std::string_view name,
	Memory address,
	std::string_view pattern,
	const PatchOptions& options
) {
	auto bytesResult = PatchBytes::FromPattern(pattern);
	if (!bytesResult) {
		return MakeError(std::move(bytesResult.error()));
	}

	return ApplyPatch(name, address, *bytesResult, options);
}

Result<void> PatchManager::RestorePatch(std::string_view name) {
	std::unique_lock lock(m_mutex);

	auto it = m_patches.find(name);
	if (it == m_patches.end()) {
		return MakeError("Patch {} not found", name);
	}

	auto& patch = it->second;

	if (!patch.IsApplied()) {
		return MakeError("Patch {} not applied", name);
	}

	if (patch.originalBytes.Size() == 0) {
		return MakeError("Original bytes of {} were not saved", name);
	}

	// Restore original bytes
	auto writeResult = WriteBytes(patch.address, patch.originalBytes, true);
	if (!writeResult) {
		patch.state = PatchState::Failed;
		patch.error = std::format("Restore failed: {}", writeResult.error());
		return writeResult;
	}

	patch.state = PatchState::Restored;

	plg::print(LS_MESSAGE, "Restored patch '{}'\n", name);

	return {};
}

Result<void> PatchManager::RestoreAll() {
	std::unique_lock lock(m_mutex);

	plg::string errorMessages;

	for (auto& [name, patch] : m_patches) {
		if (patch.IsApplied()) {
			lock.unlock();
			auto result = RestorePatch(name);
			lock.lock();

			if (!result) {
				std::format_to(std::back_inserter(errorMessages), "\n{}: {};", name, result.error());
			}
		}
	}

	if (!errorMessages.empty()) {
		return MakeError("Restore all patches: {}", errorMessages);
	}

	return {};
}

bool PatchManager::IsPatchApplied(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	auto it = m_patches.find(name);
	return it != m_patches.end() && it->second.IsApplied();
}

std::optional<PatchState> PatchManager::GetPatchState(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	auto it = m_patches.find(name);
	if (it == m_patches.end()) {
		return std::nullopt;
	}
	return it->second.state;
}

std::optional<AppliedPatch> PatchManager::GetPatchInfo(std::string_view name) const {
	std::shared_lock lock(m_mutex);
	auto it = m_patches.find(name);
	if (it == m_patches.end()) {
		return std::nullopt;
	}
	return it->second;
}

plg::vector<plg::string> PatchManager::GetAppliedPatches() const {
	std::shared_lock lock(m_mutex);
	plg::vector<plg::string> result;
	for (const auto& [name, patch] : m_patches) {
		if (patch.IsApplied()) {
			result.push_back(name);
		}
	}
	return result;
}

plg::vector<plg::string> PatchManager::GetFailedPatches() const {
	std::shared_lock lock(m_mutex);
	plg::vector<plg::string> result;
	for (const auto& [name, patch] : m_patches) {
		if (patch.IsFailed()) {
			result.push_back(name);
		}
	}
	return result;
}

size_t PatchManager::GetPatchCount() const {
	std::shared_lock lock(m_mutex);
	return m_patches.size();
}

Result<bool> PatchManager::VerifyPatch(std::string_view name) const {
	std::shared_lock lock(m_mutex);

	auto it = m_patches.find(name);
	if (it == m_patches.end()) {
		return MakeError("Patch {} not found", name);
	}

	const auto& patch = it->second;
	if (!patch.IsApplied()) {
		return false;
	}

	return VerifyBytes(patch.address, patch.patchBytes);
}

Result<void> PatchManager::VerifyAll() const {
	std::shared_lock lock(m_mutex);

	for (const auto& [name, patch] : m_patches) {
		if (patch.IsApplied()) {
			auto result = VerifyBytes(patch.address, patch.patchBytes);
			if (!result || !*result) {
				return MakeError("Patch {} verification failed", name);
			}
		}
	}

	return {};
}

PatchManager::Stats PatchManager::GetStats() const {
	std::shared_lock lock(m_mutex);

	Stats stats;
	stats.totalPatches = m_patches.size();

	for (const auto& [_, patch] : m_patches) {
		switch (patch.state) {
			case PatchState::Applied:
				stats.appliedPatches++;
				stats.totalBytesPatched += patch.patchBytes.Size();
				break;
			case PatchState::Failed:
				stats.failedPatches++;
				break;
			case PatchState::Restored:
				stats.restoredPatches++;
				break;
			case PatchState::NotApplied:
				break;
		}
	}

	return stats;
}

void PatchManager::Clear() {
	auto result = RestoreAll();
	if (!result) {
		plg::print(LS_WARNING, "Failed to restore patches: {}\n", result.error());
	}
	std::unique_lock lock(m_mutex);
	m_patches.clear();
}