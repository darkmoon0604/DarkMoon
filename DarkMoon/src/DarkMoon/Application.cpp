#include "pch.h"
#include "Application.h"

#include "GLFW/glfw3.h"

namespace DarkMoon {
#define BIND_EVENT_FUNC(f) std::bind(&Application::f, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(BIND_EVENT_FUNC(OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_isRuning)
		{
			glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			for (auto layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowClosedEvent>(BIND_EVENT_FUNC(OnWindowClosed));

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

}
