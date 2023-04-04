#include "pch.h"
#include "Application.h"

#include "DarkMoon/Render/Render.h"

namespace DarkMoon {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		DM_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		//WindowProperty wp = WindowProperty("DarkMoon Engine", 1920, 1080);
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallBack(DM_BIND_EVENT_FUNC(Application::OnEvent));

		m_ImguiLayer = new ImguiLayer();
		PushOverlay(m_ImguiLayer);

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aColor;
			
			uniform mat4 u_ViewProjection;

			out vec3 vPos;
			out vec4 vColor;

			void main()
			{
				vPos = aPos;
				vColor = aColor;
				gl_Position = u_ViewProjection * vec4(aPos, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			layout(location = 0) out vec4 fColor;
			
			in vec3 vPos;
			in vec4 vColor;

			void main()
			{
				fColor = vec4(vPos * 0.5 + 0.5, 1.0);
				fColor = vColor;
			}
		)";

		m_Shader.reset(new Shader(vertexSource, fragmentSource));

		float vertices[3 * 7]
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		m_VertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float4, "aColor" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};
		m_SquareVertexArray.reset(VertexArray::Create());
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "aPos" }
			});
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		std::string blueShaderVertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPos;

			uniform mat4 u_ViewProjection;			

			out vec3 vPos;

			void main()
			{
				vPos = aPos;
				gl_Position = u_ViewProjection * vec4(aPos, 1.0);
			}
		)";

		std::string blueShaderFragmentSource = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		in vec3 vPos;

		void main()
		{
			color = vec4(0.2, 0.3, 0.8, 1.0);
		}
		)";
		m_BlueShader.reset(new Shader(blueShaderVertexSource, blueShaderFragmentSource));
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_LINE
	}

	void Application::Run()
	{
		while (m_isRuning)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera.SetPosition(45.0f);

			Render::BeginScene(m_Camera);
			Render::Submit(m_BlueShader, m_SquareVertexArray);
			Render::Submit(m_Shader, m_VertexArray);
			Render::EndScene();

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}

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
