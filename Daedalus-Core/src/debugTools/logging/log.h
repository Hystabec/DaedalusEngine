#pragma once

#include "logFormatMacro.h"

//included here for application
#include <string>
#include <format>

/*
TO DO: 
expand to log to file

log to custom console if added?

static_assert on Log::strFormatter to check if the args have a format set?
*/

namespace daedalusCore { namespace debug {

	class  Log
	{
	public:
		enum class Caller
		{
			Core,
			Client
		};

		enum class LogFlags	//These flags currently dont do anything
		{
			None = 0,
			LogToConsole = BIT(0),
			LogToFile = BIT(1),
			Everything = LogToConsole | LogToFile
		};

	public:
		static void init(LogFlags flags = LogFlags::LogToConsole);

		/// @brief This function can be used to get a string formatted using the logger formatting
		template<typename...Args>
		static std::string formatLogMessage(const char* fmt, Args&&...args)
		{
			return strFormatter(nullptr, fmt, args...);
		}

#pragma region Trace

		template<typename T>
		static void trace(Caller caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				baseTraceLog(caller, formatted);
		}

		template<typename...Args>
		static void trace(Caller caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				baseTraceLog(caller, formatted);
		}

#pragma endregion
#pragma region Info

		template<typename T>
		static void info(const Caller& caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				baseInfoLog(caller, formatted);
		}

		template<typename...Args>
		static void info(const Caller& caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				baseInfoLog(caller, formatted);
		}

#pragma endregion
#pragma region Warn

		template<typename T>
		static void warn(const Caller& caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				baseWarnLog(caller, formatted);
		}

		template<typename...Args>
		static void warn(const Caller& caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				baseWarnLog(caller, formatted);
		}

#pragma endregion
#pragma region Error

		template<typename T>
		static void error(const Caller& caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				baseErrorLog(caller, formatted);
		}

		template<typename...Args>
		static void error(const Caller& caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				baseErrorLog(caller, formatted);
		}

#pragma endregion
#pragma region Critical

		template<typename T>
		static void critical(const Caller& caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				baseCriticalLog(caller, formatted);
		}

		template<typename...Args>
		static void critical(const Caller& caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				baseCriticalLog(caller, formatted);
		}

#pragma endregion

	private:
		static void baseTraceLog(const Caller& caller, const std::string& message);
		static void baseInfoLog(const Caller& caller, const std::string& message);
		static void baseWarnLog(const Caller& caller, const std::string& message);
		static void baseErrorLog(const Caller& caller, const std::string& message);
		static void baseCriticalLog(const Caller& caller, const std::string& message);
		
		template<typename...Args> 
		static std::string strFormatter(bool* errorCheck, const char* fmt, Args&&...args)
		{
			try
			{
				return std::vformat(fmt, std::make_format_args(args...));
			}
			catch(const std::format_error& ex)
			{
				error(Caller::Core, "Log message formatter error: {}", ex.what());
				(*errorCheck) = true;
				return "";
			}
		}

	};

} }

#if !DD_LOGGING_DISABLED

//core macros

#define DD_CORE_LOG_TRACE(...)       daedalusCore::debug::Log::trace(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define DD_CORE_LOG_INFO(...)        daedalusCore::debug::Log::info(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define DD_CORE_LOG_WARN(...)        daedalusCore::debug::Log::warn(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define DD_CORE_LOG_ERROR(...)       daedalusCore::debug::Log::error(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define DD_CORE_LOG_CRITICAL(...)    daedalusCore::debug::Log::critical(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)

//client macros

#define DD_LOG_TRACE(...)       daedalusCore::debug::Log::trace(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define DD_LOG_INFO(...)        daedalusCore::debug::Log::info(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define DD_LOG_WARN(...)        daedalusCore::debug::Log::warn(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define DD_LOG_ERROR(...)       daedalusCore::debug::Log::error(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define DD_LOG_CRITICAL(...)    daedalusCore::debug::Log::critical(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)

#else

#define DD_CORE_LOG_TRACE(...)
#define DD_CORE_LOG_INFO(...) 
#define DD_CORE_LOG_WARN(...) 
#define DD_CORE_LOG_ERROR(...) 
#define DD_CORE_LOG_CRITICAL(...) 

#define DD_LOG_TRACE(...) 
#define DD_LOG_INFO(...)
#define DD_LOG_WARN(...) 
#define DD_LOG_ERROR(...)
#define DD_LOG_CRITICAL(...)

#endif