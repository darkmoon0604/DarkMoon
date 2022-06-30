#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace DarkMoon {
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		DM_LOG_TRACE(e);

		while (true)
		{

		}
	}
}
