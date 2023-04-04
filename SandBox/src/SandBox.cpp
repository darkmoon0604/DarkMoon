#include "DarkMoon.h"

#include "Imgui/imgui.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public DarkMoon::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{

		std::string vertexSource = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec4 aColor;
			
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 vPos;
			out vec4 vColor;

			void main()
			{
				vPos = aPos;
				vColor = aColor;
				gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
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

		m_Shader.reset(DarkMoon::Shader::Create(vertexSource, fragmentSource));

		float vertices[3 * 7]
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		m_VertexArray.reset(DarkMoon::VertexArray::Create());
		std::shared_ptr<DarkMoon::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(DarkMoon::VertexBuffer::Create(vertices, sizeof(vertices)));
		DarkMoon::BufferLayout layout = {
			{ DarkMoon::ShaderDataType::Float3, "aPos" },
			{ DarkMoon::ShaderDataType::Float4, "aColor" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[] = { 0, 1, 2 };
		std::shared_ptr<DarkMoon::IndexBuffer> indexBuffer;
		indexBuffer.reset(DarkMoon::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		m_SquareVertexArray.reset(DarkMoon::VertexArray::Create());
		std::shared_ptr<DarkMoon::VertexBuffer> squareVB;
		squareVB.reset(DarkMoon::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ DarkMoon::ShaderDataType::Float3, "aPos" }
			});
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<DarkMoon::IndexBuffer> squareIB;
		squareIB.reset(DarkMoon::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		std::string blueShaderVertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPos;

			uniform mat4 u_ViewProjection;		
			uniform mat4 u_Transform;	

			out vec3 vPos;

			void main()
			{
				vPos = aPos;
				gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
			}
		)";

		std::string blueShaderFragmentSource = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		in vec3 vPos;

		uniform vec3 u_Color;

		void main()
		{
			color = vec4(u_Color, 1.0);
		}
		)";
		m_BlueShader.reset(DarkMoon::Shader::Create(blueShaderVertexSource, blueShaderFragmentSource));
	}

	void OnUpdate(DarkMoon::TimeStep timeStep) override
	{
		//DM_LOG_INFO("{0} update", m_DebugName);
		if (DarkMoon::Input::IsKeyPressed(DM_KEY_LEFT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * timeStep;
		}
		else if (DarkMoon::Input::IsKeyPressed(DM_KEY_RIGHT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * timeStep;
		}

		if (DarkMoon::Input::IsKeyPressed(DM_KEY_UP))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * timeStep;
		}
		else if (DarkMoon::Input::IsKeyPressed(DM_KEY_DOWN))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * timeStep;
		}

		if (DarkMoon::Input::IsKeyPressed(DM_KEY_A))
		{
			m_CameraRotation += m_CameraRotationSpeed * timeStep;
		}
		else if (DarkMoon::Input::IsKeyPressed(DM_KEY_D))
		{
			m_CameraRotation -= m_CameraRotationSpeed * timeStep;
		}

		DarkMoon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		DarkMoon::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		DarkMoon::Render::BeginScene(m_Camera);
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_BlueShader)->Use();
		std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				DarkMoon::Render::Submit(m_BlueShader, m_SquareVertexArray, transform);
			}
		}
		DarkMoon::Render::Submit(m_Shader, m_VertexArray);
		DarkMoon::Render::EndScene();
	}

	void OnEvent(DarkMoon::Event& e) override
	{
		//DM_LOG_INFO("{0}", e);
	}

	void OnImguiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	std::shared_ptr<DarkMoon::Shader> m_Shader;
	std::shared_ptr<DarkMoon::VertexArray> m_VertexArray;

	std::shared_ptr<DarkMoon::Shader> m_BlueShader;
	std::shared_ptr<DarkMoon::VertexArray> m_SquareVertexArray;

	DarkMoon::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;

	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 100.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class SandBox : public DarkMoon::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());		
	}
	~SandBox()
	{

	}

private:

};

DarkMoon::Application* DarkMoon::CreateApplication()
{
	DarkMoon::Log::Init("SANBOX");
	DM_LOG_WARN("init");

	return new SandBox();
}