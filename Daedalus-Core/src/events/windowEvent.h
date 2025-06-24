#pragma once

#include "event.h"

namespace daedalus { namespace event {

	class  WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent()
		{
		}

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowClosed)
	};

	class  WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(uint32_t width, uint32_t height)
			: m_width(width), m_height(height)
		{
		}

		inline uint32_t getWidth() const { return m_width; }
		inline uint32_t getHeight() const { return m_height; }

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowResized)

	private:
		uint32_t m_width, m_height;
	};

	class  WindowFocusedEvent : public Event
	{
	public:
		WindowFocusedEvent()
		{ 
		}

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowFocused)
	};

	class  WindowUnfocusedEvent : public Event
	{
	public:
		WindowUnfocusedEvent()
		{ 
		}

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowUnfocused)
	};

	class  WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(uint32_t x, uint32_t y)
			: m_x(x), m_y(y)
		{
		}

		inline uint32_t getX() const { return m_x; }
		inline uint32_t getY() const { return m_y; }

		EVENT_CLASS_CATEGORY(EventCategory::Window)
		EVENT_CLASS_TYPE(EventType::WindowMoved)

	private:
		uint32_t m_x, m_y;
	};

} }