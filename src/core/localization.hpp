#pragma once

/**
 * Thin wrapper over the optional `translations` plugin dependency.
 *
 * The dependency is declared optional in the manifest, so the imported function pointers
 * are null whenever the plugin isn't installed. Every entry point here degrades to the
 * built-in English text in that case, and again whenever a key has no value for the
 * requested language, so s2sdk renders identically to before with or without translations.
 */
namespace lang {
	Result<void> Initialize(const plg::vector<plg::string>& paths);

	std::optional<plg::string> Get(CPlayerSlot slot, std::string_view key);

	std::optional<plg::string> Get(CPlayerSlot slot, std::string_view key, std::format_args args);

	template<typename... Args>
	std::optional<plg::string> Format(CPlayerSlot slot, std::string_view key, const Args&... args) {
		return Get(slot, key, std::make_format_args(args...));
	}
}// namespace lang
