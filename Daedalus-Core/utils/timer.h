#pragma once

#include <Windows.h>
#include <chrono>

namespace daedalusCore { namespace utils {

	class Timer
	{
	private:
		LARGE_INTEGER m_start;
		double m_frequency;
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

		float elapsed()
		{
			LARGE_INTEGER current;
			QueryPerformanceCounter(&current);
			unsigned _int64 cycles = current.QuadPart - m_start.QuadPart;
			return (float)(m_frequency * cycles);
		}
	};

	class modTimer
	{
		using Clock = std::chrono::high_resolution_clock;
		using TimePoint = std::chrono::time_point<Clock>;

	private:
		TimePoint m_start;

	public:
		modTimer()
		{
			m_start = Clock::now();
		}

		void reset()
		{
			m_start = Clock::now();
		}

		float getSeconds() const
		{
			using namespace std::chrono;
			return duration_cast<seconds>(Clock::now() - m_start).count();
		}

		float getMilliseconds() const
		{
			using namespace std::chrono;
			return duration_cast<milliseconds>(Clock::now() - m_start).count();
		}

		float GetMicroseconds() const
		{
			using namespace std::chrono;
			return duration_cast<microseconds>(Clock::now() - m_start).count();
		}
	};

} }