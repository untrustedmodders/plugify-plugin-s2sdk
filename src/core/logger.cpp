#include "logger.hpp"

Logger Logger::instance{S2SDK_PACKAGE};

Logger::Logger(const char* name, RegisterTagsFunc registerTagsFunc, int flags, LoggingVerbosity_t verbosity, const Color& defaultColor) {
	m_channelID = LoggingSystem_RegisterLoggingChannel(name, registerTagsFunc, flags, verbosity, defaultColor);
}

void Logger::AddTagToChannel(const char* tagName) const {
	LoggingSystem_AddTagToChannel(m_channelID, tagName);
}

bool Logger::HasTag(const char* tag) const {
	return LoggingSystem_HasTag(m_channelID, tag);
}

bool Logger::IsChannelEnabled(LoggingSeverity_t severity) const {
	return LoggingSystem_IsChannelEnabled(m_channelID, severity);
}

bool Logger::IsChannelEnabled(LoggingVerbosity_t verbosity) const {
	return LoggingSystem_IsChannelEnabled(m_channelID, verbosity);
}

LoggingVerbosity_t Logger::GetChannelVerbosity() const {
	return LoggingSystem_GetChannelVerbosity(m_channelID);
}

void Logger::SetChannelVerbosity(LoggingVerbosity_t verbosity) const {
	return LoggingSystem_SetChannelVerbosity(m_channelID, verbosity);
}

void Logger::SetChannelVerbosityByName(const char* name, LoggingVerbosity_t verbosity) {
	return LoggingSystem_SetChannelVerbosityByName(name, verbosity);
}

void Logger::SetChannelVerbosityByTag(const char* tag, LoggingVerbosity_t verbosity) {
	LoggingSystem_SetChannelVerbosityByTag(tag, verbosity);
}

int Logger::GetChannelColor() const {
	return LoggingSystem_GetChannelColor(m_channelID);
}

void Logger::SetChannelColor(int color) const {
	LoggingSystem_SetChannelColor(m_channelID, color);
}

LoggingChannelFlags_t Logger::GetChannelFlags() const {
	return LoggingSystem_GetChannelFlags(m_channelID);
}

void Logger::SetChannelFlags(LoggingChannelFlags_t flags) const {
	LoggingSystem_SetChannelFlags(m_channelID, flags);
}

void Logger::Log(std::string_view message, LoggingSeverity_t severity, const std::source_location& location) const  {
	const LoggingRareOptions_t options {
		.m_File = location.file_name(),
		.m_Line = static_cast<int>(location.line()),
		.m_Function = location.function_name(),
	};
	switch (severity) {
		case LS_ERROR:
			LoggingSystem_LogDirect(m_channelID, LS_ERROR, options, RED, message.data());
		break;
		case LS_WARNING:
			LoggingSystem_LogDirect(m_channelID, LS_WARNING, options, YELLOW, message.data());
		break;
		case LS_MESSAGE:
			LoggingSystem_LogDirect(m_channelID, LS_MESSAGE, options, WHITE, message.data());
		break;
		case LS_DETAILED:
			LoggingSystem_LogDirect(m_channelID, LS_MESSAGE, options, GREEN, message.data());
		break;
		default:
			break;
	}
}

LoggingResponse_t Logger::Log(LoggingSeverity_t severity, const char* message) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		response = LoggingSystem_LogDirect(m_channelID, severity, message);
	}

	return response;
}

LoggingResponse_t Logger::Log(LoggingSeverity_t severity, const Color& color, const char* message) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		response = LoggingSystem_LogDirect(m_channelID, severity, color, message);
	}

	return response;
}

LoggingResponse_t Logger::Log(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const char* message) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
		response = LoggingSystem_LogDirect(m_channelID, severity, code, message);
	}

	return response;
}

LoggingResponse_t Logger::Log(LoggingSeverity_t severity, const LoggingRareOptions_t& code, const Color& color, const char* message) const {
	LoggingResponse_t response = LR_ABORT;

	if (IsChannelEnabled(severity)) {
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
