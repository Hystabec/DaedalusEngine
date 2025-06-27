#pragma once

namespace daedalus { namespace application {

	/// @brief DeltaTime by default will give time as seconds
	class DeltaTime
	{
	public:
		DeltaTime(float time = 0.0f)
			: m_time(time)
		{
		}

		operator float() const { return m_time; }

		float getSeconds() const { return m_time; }
		float getMilliseconds() const { return m_time * 1000.0f; }

	private:
		float m_time;
	};

} }
	
LOG_CREATE_FORMAT(daedalus::application::DeltaTime, "{}", dt, (float)dt);