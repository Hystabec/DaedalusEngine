#pragma once

#include <Windows.h>
#include <chrono>

namespace daedalusCore { namespace utils {

	class oldTimer
	{
	private:
		LARGE_INTEGER m_start;
		double m_frequency;
	public:
		oldTimer()
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

	class Timer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

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
			return duration_cast<seconds>(std::chrono::high_resolution_clock::now() - m_start).count();
		}

		float elapsedMilliseconds() const
		{
			using namespace std::chrono;
			return duration_cast<milliseconds>(std::chrono::high_resolution_clock::now() - m_start).count();
		}

		float elapsedMicroseconds() const
		{
			using namespace std::chrono;
			return duration_cast<microseconds>(std::chrono::high_resolution_clock::now() - m_start).count();
		}
	};

} }