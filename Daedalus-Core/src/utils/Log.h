#pragma once

#include <spdlog/spdlog.h>
#include "../Core.h"

namespace daedalusCore { namespace debug {

	class DD_API Log
	{
	private:
		static std::shared_ptr<spdlog::logger> m_coreLogger;
		static std::shared_ptr<spdlog::logger> m_clientLogger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_clientLogger; }

		template<typename T>
		static void TestLog(T&& message);

		template<typename... Args>
		static void TestLog(const char* fmt, Args&&...args);
	};


}

	template<typename T>
	inline void debug::Log::TestLog(T&& message)
	{
		m_coreLogger->info(message);
	}

	template<typename ...Args>
	void debug::Log::TestLog(const char* fmt, Args && ...args)
	{
		m_coreLogger->info(fmt, args...);
	}

}