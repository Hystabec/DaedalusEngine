#pragma once

#include "event.h"
#include "../application/input/inputCodes.h"

namespace daedalus { namespace event {

	class  MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_x(x), m_y(y)
		{
		}

		inline float getX() const { return m_x; }
		inline float getY() const { return m_y; }

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
		EVENT_CLASS_TYPE(EventType::MouseMoved)

	private:
		float m_x, m_y;
	};

	class  MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y)
			: m_x(x), m_y(y)
		{
		}

		inline float getX() const { return m_x; }
		inline float getY() const { return m_y; }

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
		EVENT_CLASS_TYPE(EventType::MouseScrolled)

	private:
		float m_x, m_y;
	};

	class  MouseButtonEvent : public Event
	{
	public:
		inline application::InputCode getButtonCode() const { return m_buttonCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)

	protected:
		MouseButtonEvent(application::InputCode buttonCode)
			: m_buttonCode(buttonCode)
		{
		}

	protected:
		application::InputCode m_buttonCode;
	};

	class  MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(application::InputCode buttonCode)
			: MouseButtonEvent(buttonCode)
		{
		}

		EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
	};

	class  MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(application::InputCode buttonCode)
			: MouseButtonEvent(buttonCode)
		{
		}

		EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
	};

} }