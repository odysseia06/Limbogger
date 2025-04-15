#include "Limbogger.h"
#include <iostream>

namespace Limbogger {

	Logger& Logger::Get() {
		static Logger instance;
		return instance;
	}

	Logger::Logger() {
		SetLogFile("log.txt"); // default log file
	}

	Logger::~Logger() {
		if (m_logFile.is_open()) {
			m_logFile.close();
		}
	}

	void Logger::SetLogLevel(LogLevel level) {
		std::lock_guard<std::mutex> lock(m_mutex);
		m_logLevel = level;
	}

	void Logger::SetLogFile(const std::string& filename) {
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_logFile.is_open()) {
			m_logFile.close();
		}
		m_logFile.open(filename, std::ios::out | std::ios::app);
	}

	std::string Logger::GetCurrentTime() const {
		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

		auto timer = std::chrono::system_clock::to_time_t(now);
		std::tm bt{};
#if defined (_WIN32) || defined (_WIN64)
		localtime_s(&bt, &timer);
#else
		localtime_r(&timer, &bt);
#endif
		std::stringstream ss;
		ss << std::put_time(&bt, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << ms.count();
		return ss.str();
	}

	std::string Logger::LogLevelToString(LogLevel level) {
		switch (level) {
		case LogLevel::Trace:   return "TRACE";
		case LogLevel::Debug:   return "DEBUG";
		case LogLevel::Info:    return "INFO";
		case LogLevel::Warning: return "WARN";
		case LogLevel::Error:   return "ERROR";
		case LogLevel::Fatal:   return "FATAL";
		default:                return "UNKNOWN";
		}
	}

	std::string Logger::LogLevelToColor(LogLevel level) {
		switch (level) {
		case LogLevel::Trace:   return "\033[37m";    // White
		case LogLevel::Debug:   return "\033[34m";    // Blue
		case LogLevel::Info:    return "\033[32m";    // Green
		case LogLevel::Warning: return "\033[33m";    // Yellow
		case LogLevel::Error:   return "\033[31m";    // Red
		case LogLevel::Fatal:   return "\033[1;31m";  // Bright Red
		default:                return "\033[0m";     // Reset
		}
	}

	void Logger::Log(LogLevel level, const char* file, int line, const std::string& message) {
		if (level < m_logLevel) return;

		std::lock_guard<std::mutex> lock(m_mutex);

		std::stringstream log_entry;
		log_entry << "[" << LogLevelToString(level) << "] ";
		log_entry << "[" << GetCurrentTime() << "] ";
		log_entry << "[" << file << ":" << line << "] ";
		log_entry << message << std::endl;

		std::cout << LogLevelToColor(level) << log_entry.str() << "\033[0m";

		if (m_logFile.is_open()) {
			m_logFile << log_entry.str();
			m_logFile.flush();
		}
	}

} // namespace Limbogger