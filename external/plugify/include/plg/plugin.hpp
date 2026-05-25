#pragma once

#include <cstdint>
#include <string_view>
#include <optional>
#include <span>
#include <concepts>
#include <utility>
#include <chrono>
#include <type_traits>

#include "plg/any.hpp"
#include "plg/version.hpp"
#include "plg/expected.hpp"
#include "plg/source_location.hpp"
#include "api.hpp"

namespace plg {
	using hndl = void*;
	using uuid = ptrdiff_t;
	using zone = uint64_t;

	enum class Severity { Unknown, Trace, Debug, Info, Warning, Error, Fatal };

	using GetBaseDirFn = plg::string (*)();
	using GetExtensionsDirFn = plg::string (*)();
	using GetConfigsDirFn = plg::string (*)();
	using GetDataDirFn = plg::string (*)();
	using GetLogsDirFn = plg::string (*)();
	using GetCacheDirFn = plg::string (*)();
	using IsLoadedFn = bool (*)(std::string_view name, std::optional<plg::range_set<>> constraints);
	using LogFn = void (*)(std::string_view message, Severity severity, const plg::source_location& location);
	using BeginZoneFn = plg::zone (*)(std::string_view name, const plg::source_location& location);
	using EndZoneFn = void (*)(plg::zone handle);

	extern GetBaseDirFn GetBaseDir;
	extern GetExtensionsDirFn GetExtensionsDir;
	extern GetConfigsDirFn GetConfigsDir;
	extern GetDataDirFn GetDataDir;
	extern GetLogsDirFn GetLogsDir;
	extern GetCacheDirFn GetCacheDir;
	extern IsLoadedFn IsLoaded;
	extern LogFn Log;
	extern BeginZoneFn BeginZone;
	extern EndZoneFn EndZone;

	struct Scope {
		explicit Scope(std::string_view name, const plg::source_location& location = plg::source_location::current()) : handle(BeginZone(name, location)) {
			Log(name, Severity::Trace, location);
		}
		~Scope() {
			if (handle != 0) {
				EndZone(handle);
			}
		}

	private:
		plg::zone handle;
	};

	struct Dependency {
		plg::string name;
		plg::range_set<> constraints;
		bool optional;
	};

	namespace plugin {
		using GetIdFn = plg::uuid (*)(plg::hndl plugin);
		using GetNameFn = plg::string (*)(plg::hndl plugin);
		using GetLicenseFn = plg::string (*)(plg::hndl plugin);
		using GetDescriptionFn = plg::string (*)(plg::hndl plugin);
		using GetVersionFn = plg::version<> (*)(plg::hndl plugin);
		using GetAuthorFn = plg::string (*)(plg::hndl plugin);
		using GetWebsiteFn = plg::string (*)(plg::hndl plugin);
		using GetLocationFn = plg::string (*)(plg::hndl plugin);
		using GetDependenciesFn = plg::vector<plg::Dependency> (*)(plg::hndl plugin);
		extern GetIdFn GetId;
		extern GetNameFn GetName;
		extern GetDescriptionFn GetDescription;
		extern GetVersionFn GetVersion;
		extern GetAuthorFn GetAuthor;
		extern GetWebsiteFn GetWebsite;
		extern GetLicenseFn GetLicense;
		extern GetLocationFn GetLocation;
		extern GetDependenciesFn GetDependencies;

		extern plg::hndl handle;
	}

	class Plugin {
	protected:
		Plugin() = default;
		~Plugin() = default;

	public:
		plg::uuid GetId() const { return plugin::GetId(plugin::handle); }
		plg::string GetName() const { return plugin::GetName(plugin::handle); }
		plg::string GetDescription() const { return plugin::GetDescription(plugin::handle); }
		plg::version<> GetVersion() const { return plugin::GetVersion(plugin::handle); }
		plg::string GetAuthor() const { return plugin::GetAuthor(plugin::handle); }
		plg::string GetWebsite() const { return plugin::GetWebsite(plugin::handle); }
		plg::string GetLicense() const { return plugin::GetLicense(plugin::handle); }
		plg::string GetLocation() const { return plugin::GetLocation(plugin::handle); }
		plg::vector<plg::Dependency> GetDependencies() const { return plugin::GetDependencies(plugin::handle); }

		virtual plg::PluginResult OnPluginStart() { return {}; }
		virtual plg::PluginResult OnPluginUpdate(std::chrono::milliseconds) { return {}; }
		virtual plg::PluginResult OnPluginEnd() { return {}; }
	};

	Plugin* GetPlugin();
} // namespace plg

#if PLUGIFY_COMPILER_CLANG
#define PLUGIFY_LINKAGE() PLUGIFY_WARN_IGNORE("-Wreturn-type-c-linkage")
#elif PLUGIFY_COMPILER_MSVC
#define PLUGIFY_LINKAGE() PLUGIFY_WARN_IGNORE(4190)
#else
#define PLUGIFY_LINKAGE()
#endif

#define PLUGIFY_PLUGIN(plugin_api, plugin_addr) \
PLUGIFY_WARN_PUSH() \
PLUGIFY_LINKAGE() \
	namespace plg { \
		GetBaseDirFn GetBaseDir{nullptr}; \
		GetExtensionsDirFn GetExtensionsDir{nullptr}; \
		GetConfigsDirFn GetConfigsDir{nullptr}; \
		GetDataDirFn GetDataDir{nullptr}; \
		GetLogsDirFn GetLogsDir{nullptr}; \
		GetCacheDirFn GetCacheDir{nullptr}; \
		IsLoadedFn IsLoaded{nullptr}; \
		LogFn Log{nullptr}; \
		BeginZoneFn BeginZone{nullptr}; \
		EndZoneFn EndZone{nullptr}; \
		namespace plugin { \
			plg::hndl handle{nullptr}; \
			GetIdFn GetId{nullptr}; \
			GetNameFn GetName{nullptr}; \
			GetDescriptionFn GetDescription{nullptr}; \
			GetVersionFn GetVersion{nullptr}; \
			GetAuthorFn GetAuthor{nullptr}; \
			GetWebsiteFn GetWebsite{nullptr}; \
			GetLicenseFn GetLicense{nullptr}; \
			GetLocationFn GetLocation{nullptr}; \
			GetDependenciesFn GetDependencies{nullptr}; \
		} \
		extern "C" plugin_api int Plugify_PluginInit(void** data, size_t len, int version, void* handle) { \
			if (version < kApiVersion) { \
				return kApiVersion; \
			} \
			std::span<void*> api(data, len); \
			size_t i = 0; \
			GetBaseDir = reinterpret_cast<GetBaseDirFn>(api[i++]); \
			GetExtensionsDir = reinterpret_cast<GetExtensionsDirFn>(api[i++]); \
			GetConfigsDir = reinterpret_cast<GetConfigsDirFn>(api[i++]); \
			GetDataDir = reinterpret_cast<GetDataDirFn>(api[i++]); \
			GetLogsDir = reinterpret_cast<GetLogsDirFn>(api[i++]); \
			GetCacheDir = reinterpret_cast<GetCacheDirFn>(api[i++]); \
			IsLoaded = reinterpret_cast<IsLoadedFn>(api[i++]); \
			Log = reinterpret_cast<LogFn>(api[i++]); \
			BeginZone = reinterpret_cast<BeginZoneFn>(api[i++]); \
			EndZone = reinterpret_cast<EndZoneFn>(api[i++]); \
			plugin::GetId = reinterpret_cast<plugin::GetIdFn>(api[i++]); \
			plugin::GetName = reinterpret_cast<plugin::GetNameFn>(api[i++]); \
			plugin::GetDescription = reinterpret_cast<plugin::GetDescriptionFn>(api[i++]); \
			plugin::GetVersion = reinterpret_cast<plugin::GetVersionFn>(api[i++]); \
			plugin::GetAuthor = reinterpret_cast<plugin::GetAuthorFn>(api[i++]); \
			plugin::GetWebsite = reinterpret_cast<plugin::GetWebsiteFn>(api[i++]); \
			plugin::GetLicense = reinterpret_cast<plugin::GetLicenseFn>(api[i++]); \
			plugin::GetLocation = reinterpret_cast<plugin::GetLocationFn>(api[i++]); \
			plugin::GetDependencies = reinterpret_cast<plugin::GetDependenciesFn>(api[i++]); \
			plugin::handle = handle; \
			return 0; \
		} \
		plg::Plugin* GetPlugin() { \
			return plugin_addr; \
		} \
		template<typename T> \
		constexpr bool has_overridden_OnPluginStart = requires { \
			{ std::declval<T>().OnPluginStart() }; \
		} && !std::is_same_v<decltype(&T::OnPluginStart), decltype(&Plugin::OnPluginStart)>; \
		template<typename T> \
		constexpr bool has_overridden_OnPluginUpdate = requires { \
			{ std::declval<T>().OnPluginUpdate(std::chrono::milliseconds{0}) }; \
		} && !std::is_same_v<decltype(&T::OnPluginUpdate), decltype(&Plugin::OnPluginUpdate)>; \
		template<typename T> \
		constexpr bool has_overridden_OnPluginEnd = requires { \
			{ std::declval<T>().OnPluginEnd() }; \
		} && !std::is_same_v<decltype(&T::OnPluginEnd), decltype(&Plugin::OnPluginEnd)>; \
		extern "C" plugin_api PluginResult Plugify_PluginStart() { \
			return GetPlugin()->OnPluginStart(); \
		} \
		extern "C" plugin_api PluginResult Plugify_PluginUpdate(std::chrono::milliseconds dt) { \
			return GetPlugin()->OnPluginUpdate(dt); \
		} \
		extern "C" plugin_api PluginResult Plugify_PluginEnd() { \
			return GetPlugin()->OnPluginEnd(); \
		} \
		extern "C" plugin_api PluginContext* Plugify_PluginContext() { \
			using plugin_type = std::remove_cvref_t<decltype(*plugin_addr)>; \
			static PluginContext context = { \
				.hasUpdate = has_overridden_OnPluginUpdate<plugin_type>, \
				.hasStart = has_overridden_OnPluginStart<plugin_type>, \
				.hasEnd = has_overridden_OnPluginEnd<plugin_type>, \
				.hasDebug = PLUGIFY_IS_DEBUG \
			}; \
			return &context; \
		} \
	} \
PLUGIFY_WARN_POP()

namespace plg {
	namespace raw {
		template<typename T>
		struct vector {
			alignas(plg::vector<T>) unsigned char pad[sizeof(plg::vector<T>)]{};
		};

		struct string {
			alignas(plg::string) unsigned char pad[sizeof(plg::string)]{};
		};

		struct variant {
			alignas(plg::any) unsigned char pad[sizeof(plg::any)]{};
		};
	} // namespace raw

	template<typename T, typename V>
	[[nodiscard]] PLUGIFY_FORCE_INLINE T as_raw(V&& value) {
		T ret{};
		std::construct_at(reinterpret_cast<V*>(&ret), std::forward<V>(value));
		return ret;
	}
} // namespace plg
