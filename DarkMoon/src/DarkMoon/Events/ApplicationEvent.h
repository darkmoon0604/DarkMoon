#pragma once

#include "Event.h"

#include <sstream>

namespace DarkMoon
{
	class DARKMOON_API WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		EVENT_CLASS_TYPE(WindowClosed)
		EVENT_CALSS_CATEGORY(Applicaction)
	};

	class DARKMOON_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height)
		{

		}

		inline int GetWidth() const
		{
			return m_Width;
		}

		inline int GetHeight() const
		{
			return m_Height;
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent : width = " << m_Width << " height = " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CALSS_CATEGORY(Applicaction)
	private:
		unsigned int m_Width, m_Height;
	};

	class DARKMOON_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CALSS_CATEGORY(Applicaction)
	};

	class DARKMOON_API AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CALSS_CATEGORY(Applicaction)
	};

	class DARKMOON_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CALSS_CATEGORY(Applicaction)
	};
}