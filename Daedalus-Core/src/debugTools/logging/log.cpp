#include "ddpch.h"
#include "log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>

#include "applicationCore.h"
#include "logImguiLayer.h"

namespace daedalusCore { namespace debug {

	static std::shared_ptr<spdlog::logger> m_coreLogger;
	static std::shared_ptr<spdlog::logger> m_clientLogger;
	logImguiLayer* Log::logLayer = nullptr; //TEMP

	void Log::Init(LogFlags flags)
	{
		spdlog::set_pattern("%^[%T][%n][%l] %v%$");
		m_coreLogger = spdlog::stdout_color_mt("Core");
		m_coreLogger->set_level(spdlog::level::trace);

		m_clientLogger = spdlog::stderr_color_mt("Client");
		m_clientLogger->set_level(spdlog::level::trace);

		if ((int)flags & (int)LogFlags::Log_to_ImGui)
		{
			logLayer = new logImguiLayer(); //TEMP
			Application::Get().PushOverlay(logLayer);
		}
	}

	void Log::BaseTraceLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			m_coreLogger->trace(message);
		}
		else if (caller == Caller::Client)
		{
			m_clientLogger->trace(message);
		}
	}

	void Log::BaseInfoLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			m_coreLogger->info(message);
		}
		else if (caller == Caller::Client)
		{
			m_clientLogger->info(message);
		}

		if (logLayer) //TEMP
			logLayer->SubmitText(message); //TEMP
	}

	void Log::BaseWarnLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			m_coreLogger->warn(message);
		}
		else if (caller == Caller::Client)
		{
			m_clientLogger->warn(message);
		}
	}

	void Log::BaseErrorLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			m_coreLogger->error(message);
		}
		else if (caller == Caller::Client)
		{
			m_clientLogger->error(message);
		}
	}

	void Log::BaseCriticalLog(const Caller& caller, const std::string& message)
	{
		if (caller == Caller::Core)
		{
			m_coreLogger->critical(message);
		}
		else if (caller == Caller::Client)
		{
			m_clientLogger->critical(message);
		}
	}

} }