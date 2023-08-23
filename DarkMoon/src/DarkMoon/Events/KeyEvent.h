#pragma once

#include "DarkMoon/Events/Event.h"
#include "DarkMoon/Core/Input.h"

namespace DarkMoon
{
	class DARKMOON_API KeyEvent : public Event
	{
	public:
		inline KeyCode GetCode() const
		{
			return m_KeyCode;
		}

		EVENT_CALSS_CATEGORY(Keyboard | InputBoard)
	protected:
		KeyEvent(KeyCode keyCode)
			: m_KeyCode(keyCode)
		{

		}

		KeyCode m_KeyCode;
	};

	class DARKMOON_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keyCode, int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount)
		{

		}

		inline int GetRepeatCount()
		{
			return m_RepeatCount;
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent : KeyCode = " << m_KeyCode << " and (Repeat Count = " << m_RepeatCount << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_RepeatCount;
	};
	
	class DARKMOON_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode)
			: KeyEvent(keyCode)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent : KeyCode = " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class DARKMOON_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keyCode)
			: KeyEvent(keyCode)
		{

		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent : KeyCode = " << m_KeyCode ;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}