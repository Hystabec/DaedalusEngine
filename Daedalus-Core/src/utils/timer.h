#pragma once

#include "../core.h"

namespace daedalusCore { namespace utils {

#ifdef USING_WINDOWS_TIMER
	class  Timer
	{
	public:
		Timer()
		{
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			m_frequency = 1.0 / frequency.QuadPart;
			QueryPerformanceCounter(&m_start);
		}

		void reset()
		{
			QueryPerformanceCounter(&m_start);
		}


		float elapsedSeconds() const
		{
			return elapsed();
		}

		float elapsedMilliseconds() const
		{
			return (elapsed() * 1000.0f);
		}

		float elapsedMicroseconds() const
		{
			return (elapsed() * 1000.0f * 1000.0f);
		}

	private:
		float elapsed() const
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			unsigned _int64 cycles = current.QuadPart - m_start.QuadPart;
			return (float)(m_frequency * cycles);
		}

	private:
		LARGE_INTEGER m_start;
		double m_frequency;
	};
#else
	class  Timer
	{
	public:
		Timer()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		void reset()
		{
			m_start = std::chrono::high_resolution_clock::now();
		}

		float elapsedSeconds() const
		{
			using namespace std::chrono;
			return (float)duration_cast<seconds>(std::chrono::high_resolution_clock::now() - m_start).count();
		}

		float elapsedMilliseconds() const
		{
			using namespace std::chrono;
			return (float)duration_cast<milliseconds>(std::chrono::high_resolution_clock::now() - m_start).count();
		}

		float elapsedMicroseconds() const
		{
			using namespace std::chrono;
			return (float)duration_cast<microseconds>(std::chrono::high_resolution_clock::now() - m_start).count();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};
#endif

} }