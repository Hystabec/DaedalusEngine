#pragma once

#include <spdlog/spdlog.h>
#include "../Core.h"

//namespace spdlog
//{
//	class logger;
//}

namespace daedalusCore { namespace debug {

	class DD_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> m_coreLogger;
		static std::shared_ptr<spdlog::logger> m_clientLogger;

	private:
		static void Test_BaseLog(const char* message);
		
		template<typename...Args> 
		static const char* Test_Fomat(const char* fmt, const Args&...args)
		{
			//this works - dont really like the use of sprint_f
			//also locks into use of std formatting

			char buffer[256];
			sprintf_s(buffer, sizeof(buffer), fmt, args...);
			return buffer;
		}

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_clientLogger; }

		template<typename...Args>
		static void Test_Trace(const char* fmt, const Args&...args)
		{
			TestBaseLog(_Fomat(fmt, args...));
		}

		/*template<typename T> 
		static void Trace(const T& message)
		{
			
		}

		template<typename... Args>
		static void Trace(const char* fmt, const Args&...args)
		{
			
		}*/
	};

} }

//core macros

#define DD_CORE_LOG_TRACE(...)       daedalusCore::debug::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DD_CORE_LOG_INFO(...)        daedalusCore::debug::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DD_CORE_LOG_WARN(...)        daedalusCore::debug::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DD_CORE_LOG_ERROR(...)       daedalusCore::debug::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DD_CORE_LOG_CRITICAL(...)    daedalusCore::debug::Log::GetCoreLogger()->critical(__VA_ARGS__)