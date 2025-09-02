#pragma once

#include "logFormatMacro.h"

//included here for application
#include <string>
#include <format>

/*
TO DO: 
expand to log to file

log to custom console if added?


if no format is made for a type, std::formatter will call a static_assert. 
- implement a static_assert that triggers in Log::strFormatter with a more useful message?

*/

namespace daedalus { namespace debug {

	class  Log
	{
	public:
		enum class Caller
		{
			Core,
			Client,
			Scripting
		};

		enum class Type
		{
			trace,
			info,
			warn,
			error,
			critical
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
		static bool hasInit();

		/// @brief This function can be used to get a string formatted using the logger formatting
		template<typename...Args>
		static std::string formatLogMessage(const char* fmt, Args&&...args)
		{
			return strFormatter(nullptr, fmt, args...);
		}

		template<typename T>
		static void log(Caller caller, Type type, T&& message)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, "{}", message);
			if (!check)
				baseLog(caller, type, formatted);
		}

		template<typename...Args>
		static void log(Caller caller, Type type, const char* fmt, Args&&... args)
		{
			bool check = false;
			const std::string& formatted = strFormatter(&check, fmt, args...);
			if (!check)
				baseLog(caller, type, formatted);
		}

	private:
		static void baseLog(Caller caller, Type type, const std::string& message);

		template<typename...Args> 
		static std::string strFormatter(bool* errorCheck, const char* fmt, Args&&...args)
		{
			try
			{
				return std::vformat(fmt, std::make_format_args(args...));
			}
			catch(const std::format_error& ex)
			{
				log(Caller::Core, Type::error, "Log message formatter error: {}", ex.what());
				(*errorCheck) = true;
				return "";
			}
		}

	};

} }

#if !DD_LOGGING_DISABLED

//core macros

#define DD_CORE_LOG_TRACE(...)       daedalus::debug::Log::log(daedalus::debug::Log::Caller::Core, daedalus::debug::Log::Type::trace,  __VA_ARGS__)
#define DD_CORE_LOG_INFO(...)        daedalus::debug::Log::log(daedalus::debug::Log::Caller::Core, daedalus::debug::Log::Type::info, __VA_ARGS__)
#define DD_CORE_LOG_WARN(...)        daedalus::debug::Log::log(daedalus::debug::Log::Caller::Core, daedalus::debug::Log::Type::warn, __VA_ARGS__)
#define DD_CORE_LOG_ERROR(...)       daedalus::debug::Log::log(daedalus::debug::Log::Caller::Core, daedalus::debug::Log::Type::error, __VA_ARGS__)
#define DD_CORE_LOG_CRITICAL(...)    daedalus::debug::Log::log(daedalus::debug::Log::Caller::Core, daedalus::debug::Log::Type::critical, __VA_ARGS__)

//client macros

#define DD_LOG_TRACE(...)       daedalus::debug::Log::log(daedalus::debug::Log::Caller::Client, daedalus::debug::Log::Type::trace, __VA_ARGS__)
#define DD_LOG_INFO(...)        daedalus::debug::Log::log(daedalus::debug::Log::Caller::Client, daedalus::debug::Log::Type::info, __VA_ARGS__)
#define DD_LOG_WARN(...)        daedalus::debug::Log::log(daedalus::debug::Log::Caller::Client, daedalus::debug::Log::Type::warn, __VA_ARGS__)
#define DD_LOG_ERROR(...)       daedalus::debug::Log::log(daedalus::debug::Log::Caller::Client, daedalus::debug::Log::Type::error,  __VA_ARGS__)
#define DD_LOG_CRITICAL(...)    daedalus::debug::Log::log(daedalus::debug::Log::Caller::Client, daedalus::debug::Log::Type::critical, __VA_ARGS__)

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