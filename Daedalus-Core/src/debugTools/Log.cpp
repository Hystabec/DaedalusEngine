#include "Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace daedalusCore { namespace debug {

	std::shared_ptr<spdlog::logger> Log::m_coreLogger;
	std::shared_ptr<spdlog::logger> Log::m_clientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T][%n]: %v%$");
		m_coreLogger = spdlog::stdout_color_mt("Core");
		m_coreLogger->set_level(spdlog::level::trace);

		m_clientLogger = spdlog::stderr_color_mt("App");
		m_clientLogger->set_level(spdlog::level::trace);
	}

	

} }