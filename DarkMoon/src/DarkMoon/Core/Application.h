#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

#include "DarkMoon/Core/TimeStep.h"

#include "DarkMoon/Events/Event.h"
#include "DarkMoon/Events/ApplicationEvent.h"

#include "DarkMoon/Imgui/ImguiLayer.h"

namespace DarkMoon {
	class DARKMOON_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
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
	};

	// 需要在客户端程序定义
	Application* CreateApplication();
}


