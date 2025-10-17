#pragma once

#include <functional>
#include <source_location>
#include <string>

#include <tier0/dbg.h>
#include <tier0/logging.h>

class Logger final {
public:
	Logger(const char* name, RegisterTagsFunc registerTagsFunc, int flags = 0, LoggingVerbosity_t verbosity = LV_DEFAULT, const Color& defaultColor = UNSPECIFIED_LOGGING_COLOR);
	~Logger() = default;

	void AddTagToChannel(const char* tagName) const;
	bool HasTag(const char* tag) const;

	bool IsChannelEnabled(LoggingSeverity_t severity) const;
	bool IsChannelEnabled(LoggingVerbosity_t verbosity) const;

	LoggingVerbosity_t GetChannelVerbosity() const;
	void SetChannelVerbosity(LoggingVerbosity_t verbosity) const;
	int GetChannelColor() const;
	void SetChannelColor(int color) const;
	LoggingChannelFlags_t GetChannelFlags() const;
	void SetChannelFlags(LoggingChannelFlags_t flags) const;

	void SetChannelVerbosityByName(const char* name, LoggingVerbosity_t verbosity);
	void SetChannelVerbosityByTag(const char* tag, LoggingVerbosity_t verbosity);
	static void RegisterTags(LoggingChannelID_t) {}

	static inline const Color WHITE = Color(255, 255, 255, 255);
	static inline const Color RED = Color(255, 0, 0, 255);
	static inline const Color YELLOW = Color(255, 255, 0, 255);
	static inline const Color GREEN = Color(0, 255, 0, 255);

	void Log(std::string_view message, LoggingSeverity_t severity, std::source_location loc = std::source_location::current()) const;
	LoggingResponse_t Log(LoggingSeverity_t severity, const char* content) const;
	LoggingResponse_t Log(LoggingSeverity_t severity, const Color& color, const char* content) const;
	LoggingResponse_t Log(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const char* content) const;
	LoggingResponse_t Log(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const Color& color, const char* content) const;

	LoggingResponse_t LogFormat(LoggingSeverity_t severity, const char* format, ...) const;
	LoggingResponse_t LogFormat(LoggingSeverity_t severity, const Color& color, const char* format, ...) const;
	LoggingResponse_t LogFormat(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const char* format, ...) const;
	LoggingResponse_t LogFormat(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const Color& color, const char* format, ...) const;

private:
	mutable std::shared_mutex m_mutex;
	LoggingChannelID_t m_channelID;
};

extern Logger g_Logger;

namespace plg {
	class Severity {
	public:
		Severity(LoggingSeverity_t severity, std::source_location location = std::source_location::current()) :
			m_severity(severity), m_location(location) {
		}

		LoggingSeverity_t m_severity;
		std::source_location m_location;
	};

	template <string_like First>
	void print(Severity severity, First&& first) {
#ifdef NDEBUG
		if (severity <= LS_DETAILED)
			return;
#endif
		g_Logger.Log(std::forward<First>(first), severity.m_severity, severity.m_location);
	}

	template <typename... Args>
	void print(Severity severity, std::format_string<Args...> fmt, Args&&... args) {
#ifdef NDEBUG
		if (severity <= LS_DETAILED)
			return;
#endif
		g_Logger.Log(std::format(fmt, std::forward<Args>(args)...), severity.m_severity, severity.m_location);
	}
}
