#pragma once

#include "Event.h"

namespace DarkMoon
{
	class DARKMOON_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: m_MouseX(x), m_MouseY(y)
		{

		}

		inline float GetX()
		{
			return m_MouseX;
		}

		inline float GetY()
		{
			return m_MouseY;
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent : x = " << m_MouseX << " y = " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CALSS_CATEGORY(Mouse | Input)
	private:
		float m_MouseX, m_MouseY;
	};

	class DARKMOON_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float x, const float y)
			: m_XOffset(x), m_YOffset(y)
		{

		}

		inline float GetXOffset()
		{
			return m_XOffset;
		}

		inline float GetYOffset()
		{
			return m_YOffset;
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent : x = " << m_XOffset << " y = " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CALSS_CATEGORY(Mouse | Input)
	private:
		float m_XOffset, m_YOffset;
	};

	class DARKMOON_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const
		{
			return m_Button;
		}

		EVENT_CALSS_CATEGORY(Mouse | Input)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button)
		{

		}
		int m_Button;
	};

	class DARKMOON_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent : button = " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class DARKMOON_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent : button = " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}