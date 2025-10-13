#include "logger.hpp"

Logger::Logger(const char* name, RegisterTagsFunc registerTagsFunc, int flags, LoggingVerbosity_t verbosity, const Color& defaultColor) {
	m_channelID = LoggingSystem_RegisterLoggingChannel(name, registerTagsFunc, flags, verbosity, defaultColor);
}

void Logger::AddTagToChannel(const char* tagName) const {
	std::unique_lock lock(m_mutex);
	LoggingSystem_AddTagToChannel(m_channelID, tagName);
}

bool Logger::HasTag(const char* tag) const {
	std::shared_lock lock(m_mutex);
	return LoggingSystem_HasTag(m_channelID, tag);
}

bool Logger::IsChannelEnabled(LoggingSeverity_t severity) const {
	std::shared_lock lock(m_mutex);
	return LoggingSystem_IsChannelEnabled(m_channelID, severity);
}

bool Logger::IsChannelEnabled(LoggingVerbosity_t verbosity) const {
	std::shared_lock lock(m_mutex);
	return LoggingSystem_IsChannelEnabled(m_channelID, verbosity);
}

LoggingVerbosity_t Logger::GetChannelVerbosity() const {
	std::shared_lock lock(m_mutex);
	return LoggingSystem_GetChannelVerbosity(m_channelID);
}

void Logger::SetChannelVerbosity(LoggingVerbosity_t verbosity) const {
	std::unique_lock lock(m_mutex);
	return LoggingSystem_SetChannelVerbosity(m_channelID, verbosity);
}

void Logger::SetChannelVerbosityByName(const char* pName, LoggingVerbosity_t verbosity) {
	std::unique_lock lock(m_mutex);
	return LoggingSystem_SetChannelVerbosityByName(pName, verbosity);
}

void Logger::SetChannelVerbosityByTag(const char* pTag, LoggingVerbosity_t verbosity) {
	std::unique_lock lock(m_mutex);
	LoggingSystem_SetChannelVerbosityByTag(pTag, verbosity);
}

int Logger::GetChannelColor() const {
	return LoggingSystem_GetChannelColor(m_channelID);
}

void Logger::SetChannelColor(int color) const {
	std::unique_lock lock(m_mutex);
	LoggingSystem_SetChannelColor(m_channelID, color);
}

LoggingChannelFlags_t Logger::GetChannelFlags() const {
	std::shared_lock lock(m_mutex);
	return LoggingSystem_GetChannelFlags(m_channelID);
}

void Logger::SetChannelFlags(LoggingChannelFlags_t flags) const {
	std::unique_lock lock(m_mutex);
	LoggingSystem_SetChannelFlags(m_channelID, flags);
}

LoggingResponse_t Logger::Log(LoggingSeverity_t severity, const char* message) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		std::unique_lock lock(m_mutex);
		response = LoggingSystem_LogDirect(m_channelID, severity, message);
	}

	return response;
}

LoggingResponse_t Logger::Log(LoggingSeverity_t severity, const Color& color, const char* message) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		std::unique_lock lock(m_mutex);
		response = LoggingSystem_LogDirect(m_channelID, severity, color, message);
	}

	return response;
}

LoggingResponse_t Logger::Log(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const char* message) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		std::unique_lock lock(m_mutex);
		response = LoggingSystem_LogDirect(m_channelID, severity, code, message);
	}

	return response;
}

LoggingResponse_t Logger::Log(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const Color& color, const char* message) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		std::unique_lock lock(m_mutex);
		response = LoggingSystem_LogDirect(m_channelID, severity, code, color, message);
	}

	return response;
}

LoggingResponse_t Logger::LogFormat(LoggingSeverity_t severity, const char* format, ...) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		char buffer[MAX_LOGGING_MESSAGE_LENGTH];

		va_list params;

		va_start(params, format);
		V_vsnprintf(buffer, sizeof(buffer), format, params);
		va_end(params);

		response = Log(severity, buffer);
	}

	return response;
}

LoggingResponse_t Logger::LogFormat(LoggingSeverity_t severity, const Color& color, const char* format, ...) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		char buffer[MAX_LOGGING_MESSAGE_LENGTH];

		va_list params;

		va_start(params, format);
		V_vsnprintf(buffer, sizeof(buffer), format, params);
		va_end(params);

		response = Log(severity, color, buffer);
	}

	return response;
}

LoggingResponse_t Logger::LogFormat(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const char* format, ...) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		char buffer[MAX_LOGGING_MESSAGE_LENGTH];

		va_list params;

		va_start(params, format);
		V_vsnprintf(buffer, sizeof(buffer), format, params);
		va_end(params);

		response = Log(severity, code, buffer);
	}

	return response;
}

LoggingResponse_t Logger::LogFormat(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const Color& color, const char* format, ...) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		char buffer[MAX_LOGGING_MESSAGE_LENGTH];

		va_list params;

		va_start(params, format);
		V_vsnprintf(buffer, sizeof(buffer), format, params);
		va_end(params);

		response = Log(severity, code, color, buffer);
	}

	return response;
}

Logger g_Logger(S2SDK_PACKAGE, &Logger::RegisterTags);