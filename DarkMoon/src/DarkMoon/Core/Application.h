#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

#include "DarkMoon/Events/Event.h"
#include "DarkMoon/Events/ApplicationEvent.h"

#include "DarkMoon/Imgui/ImguiLayer.h"

#include "DarkMoon/Render/Shader.h"
#include "DarkMoon/Render/VertexBuffer.h"
#include "DarkMoon/Render/VertexArray.h"
#include "DarkMoon/Render/IndexBuffer.h"
#include "DarkMoon/Render/OrthographicCamera.h"

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
	private:
		std::unique_ptr<Window> m_Window;
		ImguiLayer* m_ImguiLayer;
		bool m_isRuning = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVertexArray;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};

	// 需要在客户端程序定义
	Application* CreateApplication();
}


