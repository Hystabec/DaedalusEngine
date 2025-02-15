#pragma once

#include "core.h"

#include "debugTools/logging/logFormatMacro.h"

namespace daedalusCore { namespace event {

	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResized, WindowFocused, WindowUnfocused, WindowMoved,
		KeyPressed, KeyReleased, KeyHeld, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	namespace EventCategory
	{
		enum Category
		{
			None        = 0,
			Window      = BIT(0),
			Input       = BIT(1),
			Keyboard    = BIT(2),
			Mouse       = BIT(3),
			MouseButton = BIT(4)
		};
	}

#define EVENT_CLASS_TYPE(type)  static EventType getStaticType() { return (##type); }\
								virtual EventType getType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category;}

	class  Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType getType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;

		inline bool isInCategory(EventCategory::Category category) const
		{
			return getCategoryFlags() & ((int)category);
		}

		inline bool handled() const { return m_handled; }

	protected:
		bool m_handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFun = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_event(event)
		{
		}

		template<typename T>
		bool dispatch(EventFun<T> func) requires(std::is_base_of<Event, T>::value)
		{
			if (m_event.getType() == T::getStaticType())
			{
				m_event.m_handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

} }

LOG_CREATE_FORMAT(daedalusCore::event::Event, "Event: {}", e, e.getName())

//use this macro to bind a member function when passing to event dispatcher
#define DD_BIND_EVENT_FUN(fun) std::bind(&fun, this, std::placeholders::_1)

//use of this macro is unnecessary however it mimics using a member function
#define DD_BIND_EVENT_LAMBDA_FUN(fun) fun