#pragma once

#include "DarkMoon/Window.h"

#include "GLFW/glfw3.h"

namespace DarkMoon
{
	class DARKMOON_API WinWindow : public Window
	{
	public:
		WinWindow(const WindowProperty& props);

		virtual ~WinWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override
		{
			return m_Data.m_Width;
		}

		inline unsigned int GetHeight() const override
		{
			return m_Data.m_Height;
		}

		inline void SetEventCallBack(const EventCallBackFunc& callback) override
		{
			m_Data.m_CallBack = callback;
		}

		inline virtual void* GetNativeWindow() const
		{
			return m_Window;
		}

		void SetVSync(bool isEnabled) override;
		bool IsVSync() const override;
	private:
		virtual void Init(const WindowProperty& props);
		virtual void ShutDown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string m_Title;
			unsigned int m_Width;
			unsigned int m_Height;
			bool m_IsVSync;

			EventCallBackFunc m_CallBack;
		};

		WindowData m_Data;
	};
}



