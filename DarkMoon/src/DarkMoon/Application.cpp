#include "pch.h"
#include "Application.h"

#include <glad/glad.h>

namespace DarkMoon {

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(DM_BIND_EVENT_FUNC(Application::OnEvent));

		m_ImguiLayer = new ImguiLayer();
		PushOverlay(m_ImguiLayer);

		std::string vertexSource = R"(
			#version 460 core
			layout(location = 0) in vec3 aPos;
			
			out vec3 vPos;

			void main()
			{
				vPos = aPos;
				gl_Position = vec4(aPos, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 460 core
			layout(location = 0) out vec4 fColor;
			
			in vec3 vPos;

			void main()
			{
				fColor = vec4(vPos * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertexSource, fragmentSource));

		float vertices[9]
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};

		unsigned int indices[] = { 0, 1, 2 };

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		m_VBO.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		m_IBO.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_LINE
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_isRuning)
		{
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Use();
			glBindVertexArray(m_VAO);
			glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr);

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
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		m_isRuning = false;
		return true;
	}

}
