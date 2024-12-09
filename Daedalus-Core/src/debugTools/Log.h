#pragma once

#include "../Core.h"
#include <memory>
#include <format>

namespace spdlog
{
	class logger;
}

namespace daedalusCore { namespace debug {

	class DD_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> m_coreLogger;
		static std::shared_ptr<spdlog::logger> m_clientLogger;

	public:
		enum class Caller
		{
			Core,
			Client
		};

	private:
		static void BaseTraceLog(Caller& caller, std::string& message);
		static void BaseInfoLog(Caller& caller, std::string& message);
		static void BaseWarnLog(Caller& caller, std::string& message);
		static void BaseErrorLog(Caller& caller, std::string& message);
		static void BaseCriticalLog(Caller& caller, std::string& message);
		
		template<typename...Args> 
		static std::string strFormatter(const char* fmt, Args&&...args)
		{
			return std::vformat(fmt, std::make_format_args(args...));
		}

	public:
		static void Init();

#pragma region Trace

		template<typename T> 
		static void Trace(Caller caller, T&& message)
		{
			BaseTraceLog(caller, strFormatter("{}", message));
		}

		template<typename...Args>
		static void Trace(Caller caller, const char* fmt, Args&&...args)
		{
			BaseTraceLog(caller, strFormatter(fmt, args...));
		}

#pragma endregion
#pragma region Info

		template<typename T>
		static void Info(Caller caller, T&& message)
		{
			BaseInfoLog(caller, strFormatter("{}", message));
		}

		template<typename...Args>
		static void Info(Caller caller, const char* fmt, Args&&...args)
		{
			BaseInfoLog(caller, strFormatter(fmt, args...));
		}

#pragma endregion
#pragma region Warn

		template<typename T>
		static void Warn(Caller caller, T&& message)
		{
			BaseWarmLog(caller, strFormatter("{}", message));
		}

		template<typename...Args>
		static void Warn(Caller caller, const char* fmt, Args&&...args)
		{
			BaseWarnLog(caller, strFormatter(fmt, args...));
		}

#pragma endregion
#pragma region Error

		template<typename T>
		static void Error(Caller caller, T&& message)
		{
			BaseErrorLog(caller, strFormatter("{}", message));
		}

		template<typename...Args>
		static void Error(Caller caller, const char* fmt, Args&&...args)
		{
			BaseErrorLog(caller, strFormatter(fmt, args...));
		}

#pragma endregion
#pragma region Critical

		template<typename T>
		static void Critical(Caller caller, T&& message)
		{
			BaseCriticalLog(caller, strFormatter("{}", message));
		}

		template<typename...Args>
		static void Critical(Caller caller, const char* fmt, Args&&...args)
		{
			BaseCriticalLog(caller, strFormatter(fmt, args...));
		}

#pragma endregion

	};

} }

//core macros

#define CORE_LOG_TRACE(...)       daedalusCore::debug::Log::Trace(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define CORE_LOG_INFO(...)        daedalusCore::debug::Log::Info(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define CORE_LOG_WARN(...)        daedalusCore::debug::Log::Warn(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define CORE_LOG_ERROR(...)       daedalusCore::debug::Log::Error(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)
#define CORE_LOG_CRITICAL(...)    daedalusCore::debug::Log::Critical(daedalusCore::debug::Log::Caller::Core, __VA_ARGS__)

//client macros

#define LOG_TRACE(...)       daedalusCore::debug::Log::Trace(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define LOG_INFO(...)        daedalusCore::debug::Log::Info(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define LOG_WARN(...)        daedalusCore::debug::Log::Warn(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define LOG_ERROR(...)       daedalusCore::debug::Log::Error(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)
#define LOG_CRITICAL(...)    daedalusCore::debug::Log::Critical(daedalusCore::debug::Log::Caller::Client, __VA_ARGS__)