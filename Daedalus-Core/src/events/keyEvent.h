#pragma once

#include "event.h"
#include "../application/input/inputCodes.h"

namespace daedalus { namespace event {

	class  KeyEvent : public Event
	{
	public:
		inline application::InputCode getKeyCode() const { return m_keyCode; }

		EVENT_CLASS_CATEGORY(EventCategory::Keyboard | EventCategory::Input)

	protected:
		KeyEvent(application::InputCode keycode)
			: m_keyCode(keycode)
		{
		}

	protected:
		application::InputCode m_keyCode;
	};

	class  KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(application::InputCode keycode)
			: KeyEvent(keycode)
		{ 
		}

		EVENT_CLASS_TYPE(EventType::KeyPressed)
	};

	class  KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(application::InputCode keycode)
			: KeyEvent(keycode)
		{
		}

		EVENT_CLASS_TYPE(EventType::KeyReleased)
	};

	class  KeyHeldEvent : public KeyEvent
	{
	public:
		KeyHeldEvent(application::InputCode keycode, int count)
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
		KeyTypedEvent(application::InputCode keycode)
			: KeyEvent(keycode)
		{
		}

		EVENT_CLASS_TYPE(EventType::KeyTyped)
	};

} }