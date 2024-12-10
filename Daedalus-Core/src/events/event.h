#pragma once

#include "../core.h"

#include <functional>

namespace daedalusCore { namespace event {

	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResized, WindowFocused, WindowUnfocused, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	namespace EventCategory
	{
		enum Category
		{
			None = 0,
			Window = BIT(0),
			Input = BIT(1),
			Keyboard = BIT(2),
			Mouse = BIT(3),
			MouseButton = BIT(4)
		};
	}

#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return  (##type); }\
								virtual EventType GetType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category;}

	class DD_API Event
	{
		friend EventDispatcher;
	protected:
		bool m_handled = false;

	public:
		virtual EventType GetType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		inline bool IsInCategory(EventCategory::Category category)
		{
			return GetCategoryFlags() & ((int)category);
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFun = std::function<bool(T&)>;
	private:
		Event& m_event;

	public:
		EventDispatcher(Event& event)
			: m_event(event)
		{
		}

		template<typename T>
		bool Dispatch(EventFun<T> func) requires(std::is_base_of<Event, T>::value)
		{
			if (m_event.GetType() == T::GetStaticType())
			{
				m_event.m_handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}
	};

} }