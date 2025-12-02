#pragma once

#include "../core.h"

namespace daedalus::utils {

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
			return duration<float>(high_resolution_clock::now() - m_start).count();
		}

		double elapsedMilliseconds() const
		{
			using namespace std::chrono;
			return duration<double, std::milli>(high_resolution_clock::now() - m_start).count();
		}

		double elapsedMicroseconds() const
		{
			using namespace std::chrono;
			return duration<double, std::micro>(high_resolution_clock::now() - m_start).count();
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
	};

}