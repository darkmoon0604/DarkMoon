#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

#include "DarkMoon/Core/TimeStep.h"

#include "DarkMoon/Events/Event.h"
#include "DarkMoon/Events/ApplicationEvent.h"

#include "DarkMoon/Imgui/ImguiLayer.h"

int main(int argc, char** argv);

namespace DarkMoon {
	class DARKMOON_API Application
	{
	public:
		Application();
		virtual ~Application();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow()
		{
			return *m_Window;
		}

		static inline Application& Get()
		{
			return *s_Instance;
		}
	private:
		void Run();
		bool OnWindowClosed(WindowClosedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImguiLayer* m_ImguiLayer;
		bool m_isRuning = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// 需要在客户端程序定义
	Application* CreateApplication();
}


