#pragma once

#include "event.h"

namespace daedalusCore { namespace event {

	class  MouseMovedEvent : public Event
	{
	private:
		float m_x, m_y;

	public:
		MouseMovedEvent(float x, float y)
			: m_x(x), m_y(y)
		{
		}

		inline float GetX() const { return m_x; }
		inline float GetY() const { return m_y; }

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
		EVENT_CLASS_TYPE(EventType::MouseMoved)
	};

	class  MouseScrolledEvent : public Event
	{
	private:
		float m_x, m_y;

	public:
		MouseScrolledEvent(float x, float y)
			: m_x(x), m_y(y)
		{
		}

		inline float GetX() const { return m_x; }
		inline float GetY() const { return m_y; }

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
		EVENT_CLASS_TYPE(EventType::MouseScrolled)
	};

	class  MouseButtonEvent : public Event
	{
	protected:
		int m_buttonCode;

		MouseButtonEvent(int buttonCode)
			: m_buttonCode(buttonCode)
		{
		}

	public:
		inline int GetButtonCode() const { return m_buttonCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Mouse | EventCategory::Input)
	};

	class  MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int buttonCode)
			: MouseButtonEvent(buttonCode)
		{
		}

		EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
	};

	class  MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int buttonCode)
			: MouseButtonEvent(buttonCode)
		{
		}

		EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
	};

} }