#pragma once

#include "Core.h"

namespace DarkMoon {
	class DRAKMOON_API Application
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


