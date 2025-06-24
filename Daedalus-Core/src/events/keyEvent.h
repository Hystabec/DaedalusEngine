#pragma once

#include "event.h"

namespace daedalus { namespace event {

	class  KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)

	protected:
		KeyEvent(int keycode)
			: m_keyCode(keycode)
		{
		}

	protected:
		int m_keyCode;
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
	public:
		KeyHeldEvent(int keycode, int count)
			: KeyEvent(keycode), m_count(count)
		{
		}

		inline int count() const { return m_count; }

		EVENT_CLASS_TYPE(EventType::KeyHeld)

	private:
		int m_count;
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