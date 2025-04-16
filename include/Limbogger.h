#pragma once

#if defined(_WIN32) || defined(_WIN64)
#ifdef LIMBOGGER_EXPORTS
#define LIMBOGGER_API __declspec(dllexport)
#else
#define LIMBOGGER_API __declspec(dllimport)
#endif
#elif defined(__GNUC__) || defined(__clang__)
#define LIMBOGGER_API __attribute__((visibility("default")))
#else
#define LIMBOGGER_API
#endif

#include <string> // TODO? should we use our own string class?
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Limbogger {

	enum class LogLevel {
		Trace,
		Debug,
		Info,
		Warning,
		Error,
		Fatal
	};

	class LIMBOGGER_API Logger {
	public:
		static Logger& Get();

		void Log(LogLevel level, const char* file, int line, const std::string& message);

		void SetLogLevel(LogLevel level);
		void SetLogFile(const std::string& filename);
	private:
		Logger();
		~Logger();

		std::string GetCurrentTime() const;
		std::string LogLevelToString(LogLevel level);
		std::string LogLevelToColor(LogLevel level);

		std::ofstream m_logFile;
		std::mutex m_mutex; // LEARN what is mutex
		LogLevel m_logLevel = LogLevel::Trace;
	};

#define LOG_TRACE(msg)   Limbogger::Logger::Get().Log(Limbogger::LogLevel::Trace, __FILE__, __LINE__, msg)
#define LOG_DEBUG(msg)   Limbogger::Logger::Get().Log(Limbogger::LogLevel::Debug, __FILE__, __LINE__, msg)
#define LOG_INFO(msg)    Limbogger::Logger::Get().Log(Limbogger::LogLevel::Info, __FILE__, __LINE__, msg)
#define LOG_WARN(msg)    Limbogger::Logger::Get().Log(Limbogger::LogLevel::Warning, __FILE__, __LINE__, msg)
#define LOG_ERROR(msg)   Limbogger::Logger::Get().Log(Limbogger::LogLevel::Error, __FILE__, __LINE__, msg)
#define LOG_FATAL(msg)   Limbogger::Logger::Get().Log(Limbogger::LogLevel::Fatal, __FILE__, __LINE__, msg)

} // namespace Limbogger
