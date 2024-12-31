#include "ddpch.h"
#include "log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

#include "applicationCore.h"

namespace daedalusCore { namespace debug {

	static std::shared_ptr<spdlog::logger> s_coreLogger;
	static std::shared_ptr<spdlog::logger> s_clientLogger;

	void Log::init(LogFlags flags)
	{
		spdlog::set_pattern("%^[%T][%n][%l] %v%$");
		s_coreLogger = spdlog::stdout_color_mt("Core");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stderr_color_mt("Client");
		s_clientLogger->set_level(spdlog::level::trace);
	}

	void Log::baseTraceLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			s_coreLogger->trace(message);
		}
		else if (caller == Caller::Client)
		{
			s_clientLogger->trace(message);
		}
	}

	void Log::baseInfoLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			s_coreLogger->info(message);
		}
		else if (caller == Caller::Client)
		{
			s_clientLogger->info(message);
		}
	}

	void Log::baseWarnLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			s_coreLogger->warn(message);
		}
		else if (caller == Caller::Client)
		{
			s_clientLogger->warn(message);
		}
	}

	void Log::baseErrorLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			s_coreLogger->error(message);
		}
		else if (caller == Caller::Client)
		{
			s_clientLogger->error(message);
		}
	}

	void Log::baseCriticalLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			s_coreLogger->critical(message);
		}
		else if (caller == Caller::Client)
		{
			s_clientLogger->critical(message);
		}
	}

} }