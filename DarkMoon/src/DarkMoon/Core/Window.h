#pragma once

#include "pch.h"

#include "Core.h"
#include "DarkMoon/Events/Event.h"

namespace DarkMoon
{
	struct WindowProperty
	{
		std::string m_Title;
		unsigned int m_Width;
		unsigned int m_Height;

		WindowProperty(const std::string& title = "DarkMoon Engine", unsigned int width = 1280, unsigned int height = 720)
			: m_Title(title), 
			  m_Width(width),
			  m_Height(height)
		{

		}
	};

	class DARKMOON_API Window
	{
	public:
		using EventCallBackFunc = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallBack(const EventCallBackFunc& callback) = 0;
		virtual void SetVSync(bool isEnabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProperty& props = WindowProperty());
	};
}