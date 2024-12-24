#pragma once

#include "event.h"

namespace daedalusCore { namespace event {

	class  KeyEvent : public Event
	{
	protected:
		int m_keyCode;

	protected:
		KeyEvent(int keycode)
			: m_keyCode(keycode)
		{
		}

	public:
		inline int GetKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)
	};

	class  KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode)
			: KeyEvent(keycode)
		{ 
		}

		EVENT_CLASS_TYPE(EventType::KeyPressed)
	};

	class  KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode)
		{
		}

		EVENT_CLASS_TYPE(EventType::KeyReleased)
	};

	class  KeyHeldEvent : public KeyEvent
	{
	private:
		int m_count;
	public:
		KeyHeldEvent(int keycode, int count)
			: KeyEvent(keycode), m_count(count)
		{
		}

		inline int Count() const { return m_count; }

		EVENT_CLASS_TYPE(EventType::KeyHeld)
	};

	class  KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode)
		{
		}

		EVENT_CLASS_TYPE(EventType::KeyTyped)
	};

} }