#pragma once

#include "utils/timer.h"
#include "application/time/deltaTime.h"

namespace daedalus { namespace application {

	class Time
	{
	public:
		Time() { 
			m_timer = utils::Timer(); 
			m_timer.reset(); 
			m_lastFrameTime = m_timer.elapsedSeconds();
		};

		void update() { m_lastFrameTime = getTime(); }

		/// @brief Gets the time since the begining of the program in seconds
		float getTime() const { return m_timer.elapsedSeconds(); };

		application::DeltaTime getDeltaTime() const { return application::DeltaTime(getTime() - m_lastFrameTime); }

	private:
		utils::Timer m_timer;
		float m_lastFrameTime;
	};

} }