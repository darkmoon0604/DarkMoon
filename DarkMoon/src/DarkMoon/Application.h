#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace DarkMoon {
	class DARKMOON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:

	};

	// 需要在客户端程序定义
	Application* CreateApplication();
}


