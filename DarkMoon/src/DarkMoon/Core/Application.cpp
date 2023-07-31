#include "pch.h"

#include "Application.h"
#include "DarkMoon/Render/Render.h"

#include <GLFW/glfw3.h>

namespace DarkMoon {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
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

	void Application::Run()
	{
		while (m_isRuning)
		{
			float time = (float)glfwGetTime();
			TimeStep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(ts);
				}
			}

			m_ImguiLayer->OnBegin();
			for (auto layer : m_LayerStack)
			{
				layer->OnImguiRender();
			}
			m_ImguiLayer->OnEnd();

			//auto [x, y] = Input::GetMousePosition();
			//DM_LOG_CORE_TRACE("{0}, {1}", x, y);

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(DM_BIND_EVENT_FUNC(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(DM_BIND_EVENT_FUNC(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
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
