#include "dmpch.h"

#include "Application.h"

namespace DarkMoon {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_isRuning)
		{
			m_Window->OnUpdate();
		}
	}
}
