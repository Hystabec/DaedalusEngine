#pragma once

//
// Basic instrumentation profiler by Cherno
// https://github.com/TheCherno/Hazel/blob/master
//

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>
#include <sstream>

namespace daedalus { namespace debug {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;
		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void beginSession(const std::string& name, const std::string& filepath = "results")
		{
			std::lock_guard lock(m_mutex);
			if (m_CurrentSession)
			{
				if (Log::hasInit())
					DD_CORE_LOG_ERROR("Instrumentor::BeginSession('{}') when session '{}' already open.", name, m_CurrentSession->Name);

				internalEndSession();
			}
			m_OutputStream.open(filepath + ".profile.json");

			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession({ name });
				writeHeader();
			}
			else
			{
				if (Log::hasInit())
					DD_CORE_LOG_ERROR("Instrumentor could not open results file '{}'.", filepath);
			}
		}

		void endSession()
		{
			std::lock_guard lock(m_mutex);
			internalEndSession();
		}

		void writeProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";


			std::lock_guard lock(m_mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Instrumentor& get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		Instrumentor()
			: m_CurrentSession(nullptr)
		{
		}

		~Instrumentor()
		{
			endSession();
		}

		void writeHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void writeFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void internalEndSession()
		{
			if (m_CurrentSession)
			{
				writeFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}

	private:
		std::mutex m_mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				stop();
		}

		void stop()
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };

			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() -
				std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::get().writeProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id()});

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};

	namespace InstrumentorUtils {

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		template <size_t N, size_t K>
		constexpr auto cleanup_output_string(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
					matchIndex++;
				if (matchIndex == K - 1)
					srcIndex += matchIndex;
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
} }

#undef DD_USING_PROFILING
#ifdef DD_USING_PROFILING
	// Resolve which function signature macro will be used. Note that this only
	// is resolved when the (pre)compiler starts, so the syntax highlighting
	// could mark the wrong one in your editor!
	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define DD_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define DD_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define DD_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define DD_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define DD_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define DD_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define DD_FUNC_SIG __func__
	#else
		#define DD_FUNC_SIG "DD_FUNC_SIG unknown!"
	#endif

	#define DD_PROFILE_BEGIN_SESSION(name, filepath)	daedalus::debug::Instrumentor::get().beginSession(name, filepath)
	#define DD_PROFILE_END_SESSION()					daedalus::debug::Instrumentor::get().endSession()

	#define DD_PROFILE_SCOPE_LINE2(name, line)			constexpr auto fixedName##line = daedalus::debug::InstrumentorUtils::cleanup_output_string(name, "__cdecl ");\
														daedalus::debug::InstrumentationTimer timer##line(fixedName##line.Data)

	#define DD_PROFILE_SCOPE_LINE(name, line)			DD_PROFILE_SCOPE_LINE2(name, line)
	#define DD_PROFILE_SCOPE(name)						DD_PROFILE_SCOPE_LINE(name, __LINE__)

	#define DD_PROFILE_FUNCTION()						DD_PROFILE_SCOPE(DD_FUNC_SIG)
#else
	#define DD_PROFILE_BEGIN_SESSION(name, fileName)
	#define DD_PROFILE_END_SESSION()
	#define DD_PROFILE_SCOPE(name)
	#define DD_PROFILE_FUNCTION()
#endif