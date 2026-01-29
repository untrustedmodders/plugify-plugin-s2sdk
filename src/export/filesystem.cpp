#include <filesystem.h>
#include "utlbuffer.h"
#include "utlstring.h"
#include <tier1/utlvector.h>

PLUGIFY_WARN_PUSH()

#if defined(__clang)
PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif defined(_MSC_VER)
PLUGIFY_WARN_IGNORE(4190)
#endif

/**
 * @brief Reads a file and returns its contents as a string.
 *
 * @param localFileName The relative path of the file to read.
 * @param pathId The filesystem search path ID (e.g., "GAME"). If empty, uses "GAME".
 * @return The file contents, or an empty string on failure.
 */
extern "C" PLUGIN_API plg::string ReadFileVPK(const plg::string& localFileName, const plg::string& pathId) {
	if (!g_pFullFileSystem) {
		return {};
	}

	const char* resolvedPathId = pathId.empty() ? "GAME" : pathId.c_str();
	CUtlBuffer buffer;
	if (!g_pFullFileSystem->ReadFile(localFileName.c_str(), resolvedPathId, buffer)) {
		return {};
	}

	const auto size = static_cast<size_t>(buffer.TellPut());
	if (size == 0 || buffer.Base() == nullptr) {
		return {};
	}

	return { static_cast<const char*>(buffer.Base()), size };
}

/**
 * @brief Finds all files matching a wildcard and returns absolute paths.
 *
 * @param wildcard The wildcard pattern to match (e.g., "path/*.txt").
 * @param pathId The filesystem search path ID (e.g., "GAME"). If empty, uses "GAME".
 * @return A list of absolute file paths matching the wildcard.
 */
extern "C" PLUGIN_API plg::vector<plg::string> FindFileAbsoluteList(const plg::string& wildcard, const plg::string& pathId) {
	plg::vector<plg::string> results;
	if (!g_pFullFileSystem) {
		return results;
	}

	const char* resolvedPathId = pathId.empty() ? "GAME" : pathId.c_str();
	CUtlVector<CUtlString> files;
	g_pFullFileSystem->FindFileAbsoluteList(files, wildcard.c_str(), resolvedPathId);

	results.reserve(static_cast<size_t>(files.Count()));
	for (int i = 0; i < files.Count(); ++i) {
		const char* value = files.Element(i).Get();
		results.emplace_back(value ? value : "");
	}

	return results;
}

PLUGIFY_WARN_POP()
