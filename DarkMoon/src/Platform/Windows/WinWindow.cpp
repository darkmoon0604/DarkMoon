#include "pch.h"
#include "WinWindow.h"

#include "DarkMoon/Events/ApplicationEvent.h"
#include "DarkMoon/Events/KeyEvent.h"
#include "DarkMoon/Events/MouseEvent.h"
#include "DarkMoon/Render/Render.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace DarkMoon
{
	static uint8_t s_GLFWWindowCount = 0;

	static void GLFWErrorCallBack(int code, const char* desc)
	{
		DM_LOG_CORE_ERROR("GLFW Error {0}{1}", code, desc);
	}

	WinWindow::WinWindow(const WindowProperty& props)
	{
		DM_PROFILE_FUNCTION();
		Init(props);
	}

	WinWindow::~WinWindow()
	{
		DM_PROFILE_FUNCTION();
		ShutDown();
	}

	void WinWindow::Init(const WindowProperty& props)
	{
		DM_PROFILE_FUNCTION();
		m_Data.m_Title = props.m_Title;
		m_Data.m_Width = props.m_Width;
		m_Data.m_Height = props.m_Height;

		DM_LOG_CORE_INFO("Create window {0} ({1}, {2})", m_Data.m_Title, m_Data.m_Width, m_Data.m_Height);

		if (s_GLFWWindowCount == 0)
		{
			DM_PROFILE_SCOPE("glfw init");
			DM_LOG_CORE_INFO("Initializing GLFW!");
			int success = glfwInit();
			DM_CORE_ASSERT(success, "Cound not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallBack);
		}

		{
			DM_PROFILE_SCOPE("glfw create window");
#if defined(DM_DEBUG)
			if (Render::GetAPI() == RendererAPI::API::OpenGL)
			{
				// debug 模式下开启opengl调试模式
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			}
#endif
			m_Window = glfwCreateWindow(
				(int)m_Data.m_Width,
				(int)m_Data.m_Height,
				m_Data.m_Title.c_str(),
				nullptr,
				nullptr);

			++s_GLFWWindowCount;
		}
		
		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(false);

		// tell GLFW to capture our mouse
		//glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(m_Window, m_Data.m_Width / 2.0f, m_Data.m_Height / 2.0f);

		//set glfw callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			// window resize event
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowResizeEvent e(width, height);
			data.m_Width = width;
			data.m_Height = height;
			data.m_CallBack(e);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) 
		{
			// window close event
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowClosedEvent e;
			data.m_CallBack(e);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			// window key event
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent e(static_cast<KeyCode>(key), 0);
					data.m_CallBack(e);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent e(static_cast<KeyCode>(key));
					data.m_CallBack(e);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent e(static_cast<KeyCode>(key), 1);
					data.m_CallBack(e);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			// window char event
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent e(static_cast<KeyCode>(keycode));
			data.m_CallBack(e);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) 
		{
			// window mouse event
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent e(static_cast<MouseCode>(button));
					data.m_CallBack(e);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent e(static_cast<MouseCode>(button));
					data.m_CallBack(e);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			// window scroll event
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent e((float)xOffset, (float)yOffset);
			data.m_CallBack(e);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			// window mouse moved event
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseMovedEvent e((float)xPos, (float)yPos);
			data.m_CallBack(e);
		});
	}

	void WinWindow::ShutDown()
	{
		DM_PROFILE_FUNCTION();
		if (m_Window == nullptr)
			return;
		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;
		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
			m_Window = nullptr;
		}
	}

	void WinWindow::OnUpdate()
	{
		DM_PROFILE_FUNCTION();
		m_Context->SwapBuffer();
		glfwPollEvents();
	}

	void WinWindow::SetVSync(bool isEnabled)
	{
		DM_PROFILE_FUNCTION();
		glfwSwapInterval(isEnabled ? 1 : 0);
		m_Data.m_IsVSync = isEnabled;
	}

	bool WinWindow::IsVSync() const
	{
		return m_Data.m_IsVSync;
	}

	void WinWindow::OnClosed()
	{
		ShutDown();
	}

	void WinWindow::IsShowCursor(bool isShow)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, isShow ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}
}