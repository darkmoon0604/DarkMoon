#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Window.h"
#include "LayerStack.h"
#include "DarkMoon/Imgui/ImguiLayer.h"

namespace DarkMoon {
	class DARKMOON_API Application
	{
	public:
		Application();
		virtual ~Application();

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
	private:
		std::unique_ptr<Window> m_Window;
		ImguiLayer* m_ImguiLayer;
		bool m_isRuning = true;
		LayerStack m_LayerStack;

	private:
		static Application* s_Instance;
	};

	// ��Ҫ�ڿͻ��˳�����
	Application* CreateApplication();
}


