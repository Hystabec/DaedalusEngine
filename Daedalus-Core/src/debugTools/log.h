#pragma once

#include "logFormatMacro.h"

//included here for application
#include <string>
#include <format>

/*
TO DO: 
expand to log to file

fire event when log is called?
log to custom console if added?
*/

namespace daedalusCore { namespace debug {

	class DD_API Log
	{
	public:
		enum class Caller
		{
			Core,
			Client
		};

	private:
		static void BaseTraceLog(const Caller& caller, const std::string& message);
		static void BaseInfoLog(const Caller& caller, const std::string& message);
		static void BaseWarnLog(const Caller& caller, const std::string& message);
		static void BaseErrorLog(const Caller& caller, const std::string& message);
		static void BaseCriticalLog(const Caller& caller, const std::string& message);
		
		template<typename...Args> 
		static std::string strFormatter(bool* errorCheck, const char* fmt, Args&&...args)
		{
			try
			{
				return std::vformat(fmt, std::make_format_args(args...));
			}
			catch(const std::format_error& ex)
			{
				Error(Caller::Core, "Log message formatter error: {}", ex.what());
				(*errorCheck) = true;
				return "";
			}
		}

	public:
		static void Init();

#pragma region Trace

		template<typename T> 
		static void Trace(Caller caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if(!check)
				BaseTraceLog(caller, formatted);
		}

		template<typename...Args>
		static void Trace(Caller caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				BaseTraceLog(caller, formatted);
		}

#pragma endregion
#pragma region Info

		template<typename T>
		static void Info(const Caller& caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				BaseInfoLog(caller, formatted);
		}

		template<typename...Args>
		static void Info(const Caller& caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				BaseInfoLog(caller, formatted);
		}

#pragma endregion
#pragma region Warn

		template<typename T>
		static void Warn(const Caller& caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				BaseWarnLog(caller, formatted);
		}

		template<typename...Args>
		static void Warn(const Caller& caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				BaseWarnLog(caller, formatted);
		}

#pragma endregion
#pragma region Error

		template<typename T>
		static void Error(const Caller& caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				BaseErrorLog(caller, formatted);
		}

		template<typename...Args>
		static void Error(const Caller& caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				BaseErrorLog(caller, formatted);
		}

#pragma endregion
#pragma region Critical

		template<typename T>
		static void Critical(const Caller& caller, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				BaseCriticalLog(caller, formatted);
		}

		template<typename...Args>
		static void Critical(const Caller& caller, const char* fmt, Args&&...args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				BaseCriticalLog(caller, formatted);
		}

#pragma endregion

	};

} }

#if !DD_LOGGING_DISABLED

//core macros

#define DD_CORE_LOG_TRACE(...)       daedalusCore::debug::Log::Trace(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define DD_CORE_LOG_INFO(...)        daedalusCore::debug::Log::Info(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define DD_CORE_LOG_WARN(...)        daedalusCore::debug::Log::Warn(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define DD_CORE_LOG_ERROR(...)       daedalusCore::debug::Log::Error(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define DD_CORE_LOG_CRITICAL(...)    daedalusCore::debug::Log::Critical(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)

//client macros

#define DD_LOG_TRACE(...)       daedalusCore::debug::Log::Trace(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define DD_LOG_INFO(...)        daedalusCore::debug::Log::Info(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define DD_LOG_WARN(...)        daedalusCore::debug::Log::Warn(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define DD_LOG_ERROR(...)       daedalusCore::debug::Log::Error(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define DD_LOG_CRITICAL(...)    daedalusCore::debug::Log::Critical(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)

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