#include "pch.h"
#include "WinWindow.h"

#include "DarkMoon/Events/ApplicationEvent.h"
#include "DarkMoon/Events/KeyEvent.h"
#include "DarkMoon/Events/MouseEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace DarkMoon
{
	static bool s_GLFWInit = false;

	static void GLFWErrorCallBack(int code, const char* desc)
	{
		DM_LOG_CORE_ERROR("GLFW Error {0}{1}", code, desc);
	}

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
			glfwSetErrorCallback(GLFWErrorCallBack);
			s_GLFWInit = true;
		}

		m_Window = glfwCreateWindow(
			(int)m_Data.m_Width,
			(int)m_Data.m_Height,
			m_Data.m_Title.c_str(),
			nullptr,
			nullptr);

		
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(false);

		//set glfw callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowResizeEvent e(width, height);
			data.m_Width = width;
			data.m_Height = height;
			data.m_CallBack(e);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowClosedEvent e;
			data.m_CallBack(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(key, 0);
					data.m_CallBack(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(key);
					data.m_CallBack(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(key, 1);
					data.m_CallBack(e);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(keycode);
			data.m_CallBack(e);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) 
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(button);
					data.m_CallBack(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(button);
					data.m_CallBack(e);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent e((float)xOffset, (float)yOffset);
			data.m_CallBack(e);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent e((float)xPos, (float)yPos);
			data.m_CallBack(e);
		});
	}

	void WinWindow::ShutDown()
	{
		glfwDestroyWindow(m_Window);
		m_Window = nullptr;
	}

	void WinWindow::OnUpdate()
	{
		m_Context->SwapBuffer();
		glfwPollEvents();
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