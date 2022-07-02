#include "dmpch.h"
#include "WinWindow.h"

namespace DarkMoon
{
	static bool s_GLFWInit = false;

	Window* Window::Create(const WindowProperty& props)
	{
		return new WinWindow(props);
	}

	WinWindow::WinWindow(const WindowProperty& props)
	{
		Init(props);
	}

	WinWindow::~WinWindow()
	{
		ShutDown();
	}

	void WinWindow::Init(const WindowProperty& props)
	{
		m_Data.m_Title = props.m_Title;
		m_Data.m_Width = props.m_Width;
		m_Data.m_Height = props.m_Height;

		DM_LOG_CORE_INFO("Create window {0} ({1}, {2})", m_Data.m_Title, m_Data.m_Width, m_Data.m_Height);

		if (!s_GLFWInit)
		{
			int success = glfwInit();
			DM_CORE_ASSERT(success, "Cound not intialize GLFW!");
			s_GLFWInit = true;
		}

		m_Window = glfwCreateWindow(
			(int)m_Data.m_Width,
			(int)m_Data.m_Height,
			m_Data.m_Title.c_str(),
			nullptr,
			nullptr);

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);
	}

	void WinWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
		m_Window = nullptr;
	}

	void WinWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WinWindow::SetVSync(bool isEnabled)
	{
		glfwSwapInterval(isEnabled ? 1 : 0);
		m_Data.m_IsVSync = isEnabled;
	}

	bool WinWindow::IsVSync() const
	{
		return m_Data.m_IsVSync;
	}
}