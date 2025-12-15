#pragma once

#include "core.h"

#include "debugTools/logging/logFormatMacro.h"

namespace daedalus { namespace event {

	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResized, WindowFocused, WindowUnfocused, WindowMoved, WindowDrop,
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
		virtual ~Event() = default;

		virtual EventType getType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;

		inline bool isInCategory(EventCategory::Category category) const
		{
			return getCategoryFlags() & ((int)category);
		}

		inline bool handled() const { return m_handled; }
		inline void setHandled(bool handled) { m_handled = handled; }

	protected:
		bool m_handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_event(event)
		{
		}

		template<typename T, typename F>
		bool dispatch(const F& func) requires(std::is_base_of<Event, T>::value)
		{
			if (m_event.getType() == T::getStaticType())
			{
				m_event.m_handled = func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

} }

LOG_CREATE_FORMAT(daedalus::event::Event, "Event: {}", e, e.getName())

/// @brief Use this macro to bind a member function when passing to event dispatcher
#define DD_BIND_EVENT_FUN(fun) [this](auto&&... args) -> decltype(auto) { return this->fun(std::forward<decltype(args)>(args)...); }

/// @brief Use of this macro is unnecessary however it mimics using the member function macro
#define DD_BIND_EVENT_LAMBDA_FUN(fun) fun