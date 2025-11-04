#include "ddpch.h"
#include "log.h"

#include "application/applicationCore.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/ostr.h>

namespace daedalus { namespace debug {

	static std::shared_ptr<spdlog::logger> s_coreLogger;
	static std::shared_ptr<spdlog::logger> s_clientLogger;
	static std::shared_ptr<spdlog::logger> s_scriptingLogger;
	static bool s_hasInit = false;


	void Log::init(LogFlags flags)
	{
		if (s_hasInit)
			return;

		spdlog::sink_ptr stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		spdlog::sink_ptr mainFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Deadalus.log", true);
		spdlog::sink_ptr scriptingFileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Deadalus-Scripting.log", true);

		stdoutSink->set_pattern("%^[%T][%n][%l] %v%$");
		mainFileSink->set_pattern("[%T][%n][%l] %v%$");
		scriptingFileSink->set_pattern("[%T][%l] %v%$");
		
		s_coreLogger = std::make_shared<spdlog::logger>("Core", spdlog::sinks_init_list{ stdoutSink, mainFileSink });
		spdlog::register_logger(s_coreLogger);
		s_coreLogger->set_level(spdlog::level::trace);
		s_coreLogger->flush_on(spdlog::level::trace);

		s_clientLogger = std::make_shared<spdlog::logger>("Client", spdlog::sinks_init_list{ stdoutSink, mainFileSink });
		spdlog::register_logger(s_clientLogger);
		s_clientLogger->set_level(spdlog::level::trace);
		s_clientLogger->flush_on(spdlog::level::trace);

		s_scriptingLogger = std::make_shared<spdlog::logger>("C#", spdlog::sinks_init_list{ stdoutSink, scriptingFileSink });
		spdlog::register_logger(s_scriptingLogger);
		s_scriptingLogger->set_level(spdlog::level::trace);
		s_scriptingLogger->flush_on(spdlog::level::trace);

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
		else if (caller == Caller::Scripting)
		{
			switch (type)
			{
			case Type::trace:
				s_scriptingLogger->trace(message);
				break;
			case Type::info:
				s_scriptingLogger->info(message);
				break;
			case Type::warn:
				s_scriptingLogger->warn(message);
				break;
			case Type::error:
				s_scriptingLogger->error(message);
				break;
			case Type::critical:
				s_scriptingLogger->critical(message);
				break;
			}
		}
	}

} }