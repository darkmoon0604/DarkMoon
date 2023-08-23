#include "pch.h"

#include "Application.h"
#include "DarkMoon/Render/Render.h"
#include "DarkMoon/Core/Input.h"
#include "DarkMoon/Core/KeyCodes.h"

#include <GLFW/glfw3.h>

namespace DarkMoon {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		DM_PROFILE_FUNCTION();

		DM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		//WindowProperty wp = WindowProperty("DarkMoon Engine", 1920, 1080);
		m_Window = Scope<Window>(Window::Create());
		m_Window->SetEventCallBack(DM_BIND_EVENT_FUNC(Application::OnEvent));

		Render::Init();

		m_ImguiLayer = new ImguiLayer();
		PushOverlay(m_ImguiLayer);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_LINE
	}

	Application::~Application()
	{
		DM_PROFILE_FUNCTION();

		Render::Shutdown();
	}

	void Application::Run()
	{
		DM_PROFILE_FUNCTION();
		while (m_isRuning)
		{
			DM_PROFILE_SCOPE("RunLoop");
			if (Input::IsKeyPressed(Key::Escape))
			{
				m_isRuning = false;
				m_Window->OnClosed();
				return;
			}

			float time = (float)glfwGetTime();
			TimeStep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				{
					DM_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : m_LayerStack)
					{
						layer->OnUpdate(ts);
					}
				}
				
				m_ImguiLayer->OnBegin();
				{
					DM_PROFILE_SCOPE("LayerStack OnImGuiRender");
					for (auto layer : m_LayerStack)
					{
						layer->OnImguiRender();
					}
				}
				
				m_ImguiLayer->OnEnd();
			}

			//auto [x, y] = Input::GetMousePosition();
			//DM_LOG_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		DM_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(DM_BIND_EVENT_FUNC(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(DM_BIND_EVENT_FUNC(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.IsHandled())
			{
				break;
			}
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		DM_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		DM_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		m_isRuning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Render::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
