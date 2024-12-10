#pragma once

#include "event.h"

namespace daedalusCore { namespace event {

	class DD_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent()
		{
		}

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowClosed)
	};

	class DD_API WindowResizedEvent : public Event
	{
	private:
		unsigned int m_width, m_height;

	public:
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_width(width), m_height(height)
		{
		}

		inline unsigned int GetWidth() const { return m_width; }
		inline unsigned int GetHeight() const { return m_height; }

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowResized)
	};

	class DD_API WindowFocusedEvent : public Event
	{
	public:
		WindowFocusedEvent()
		{ 
		}

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowFocused)
	};

	class DD_API WindowUnfocusedEvent : public Event
	{
	public:
		WindowUnfocusedEvent()
		{ 
		}

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowUnfocused)
	};

	//moved
	class DD_API WindowMovedEvent : public Event
	{
	private:
		unsigned int m_x, m_y;
		
	public:
		WindowMovedEvent(unsigned int x, unsigned int y)
			: m_x(x), m_y(y)
		{
		}

		inline unsigned int GetX() const { return m_x; }
		inline unsigned int GetY() const { return m_y; }

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowMoved)
	};

} }