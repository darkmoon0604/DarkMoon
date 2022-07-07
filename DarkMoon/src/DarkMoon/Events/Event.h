#pragma once

#include "pch.h"

#include "DarkMoon/Core.h"

namespace DarkMoon
{
	/// <summary>
	/// ÊÂ¼þÃ¶¾Ù
	/// </summary>
	enum class EventType
	{
		None = 0,
		WindowClosed, WindowResize, WindowFocus, WindosLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	};


	enum EventCategory
	{
		None,
		Applicaction = BIT(0),
		Input = BIT(1),
		Keyboard = BIT(2),
		Mouse = BIT(3),
		MouseButton = BIT(4),
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CALSS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class DARKMOON_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const
		{ 
			return GetName(); 
		}

		inline bool IsHandled() const
		{
			return m_Handled;
		}

		inline bool IsInCategory(int category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool m_Handled = false;
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFunc = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{

		}

		template<typename T>
		bool Dispatch(EventFunc<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}

			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}