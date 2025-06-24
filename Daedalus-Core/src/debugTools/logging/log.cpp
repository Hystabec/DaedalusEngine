#include "ddpch.h"
#include "log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

#include "application/applicationCore.h"

namespace daedalus { namespace debug {

	static std::shared_ptr<spdlog::logger> s_coreLogger;
	static std::shared_ptr<spdlog::logger> s_clientLogger;
	static bool s_hasInit = false;


	void Log::init(LogFlags flags)
	{
		if (s_hasInit)
			return;

		spdlog::set_pattern("%^[%T][%n][%l] %v%$");
		s_coreLogger = spdlog::stdout_color_mt("Core");
		s_coreLogger->set_level(spdlog::level::trace);

		s_clientLogger = spdlog::stderr_color_mt("Client");
		s_clientLogger->set_level(spdlog::level::trace);

		s_hasInit = true;
	}

	bool Log::hasInit()
	{
		return s_hasInit;
	}

	void Log::baseLog(Caller caller, Type type, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			switch (type)
			{
			case Type::trace:
				s_coreLogger->trace(message);
				break;
			case Type::info:
				s_coreLogger->info(message);
				break;
			case Type::warn:
				s_coreLogger->warn(message);
				break;
			case Type::error:
				s_coreLogger->error(message);
				break;
			case Type::critical:
				s_coreLogger->critical(message);
				break;
			}
		}
		else if (caller == Caller::Client)
		{
			switch (type)
			{
			case Type::trace:
				s_clientLogger->trace(message);
				break;
			case Type::info:
				s_clientLogger->info(message);
				break;
			case Type::warn:
				s_clientLogger->warn(message);
				break;
			case Type::error:
				s_clientLogger->error(message);
				break;
			case Type::critical:
				s_clientLogger->critical(message);
				break;
			}
		}
	}

} }