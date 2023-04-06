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
		float vertices[3 * 7]
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		m_VertexArray.reset(DarkMoon::VertexArray::Create());
		DarkMoon::Ref<DarkMoon::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(DarkMoon::VertexBuffer::Create(vertices, sizeof(vertices)));
		DarkMoon::BufferLayout layout = {
			{ DarkMoon::ShaderDataType::Float3, "aPos" },
			{ DarkMoon::ShaderDataType::Float4, "aColor" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[] = { 0, 1, 2 };
		DarkMoon::Ref<DarkMoon::IndexBuffer> indexBuffer;
		indexBuffer.reset(DarkMoon::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		m_SquareVertexArray.reset(DarkMoon::VertexArray::Create());
		DarkMoon::Ref<DarkMoon::VertexBuffer> squareVB;
		squareVB.reset(DarkMoon::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ DarkMoon::ShaderDataType::Float3, "aPos" },
			{ DarkMoon::ShaderDataType::Float2, "aTexcoord" },
			});
		m_SquareVertexArray->AddVertexBuffer(squareVB);

		unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		DarkMoon::Ref<DarkMoon::IndexBuffer> squareIB;
		squareIB.reset(DarkMoon::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(unsigned int)));
		m_SquareVertexArray->SetIndexBuffer(squareIB);

		m_Shader.reset(DarkMoon::Shader::Create("assets/shaders/triangle.glsl"));
		m_BlueShader.reset(DarkMoon::Shader::Create("assets/shaders/square.glsl"));
		m_TextureShader.reset(DarkMoon::Shader::Create("assets/shaders/texture.glsl"));

		m_Texture2D = DarkMoon::Texture2D::Create("assets/textures/Checkerboard.png");
		m_Texture2D1 = DarkMoon::Texture2D::Create("assets/textures/awesomeface.png");

		std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_TextureShader)->Use();
		std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_TextureShader)->UploadUniformInt("uTexture", 0);
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
		std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_BlueShader)->UploadUniformFloat3("uColor", m_SquareColor);
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				DarkMoon::Render::Submit(m_BlueShader, m_SquareVertexArray, transform);
			}
		}
		m_Texture2D->Bind();
		DarkMoon::Render::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_Texture2D1->Bind();
		DarkMoon::Render::Submit(m_TextureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
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
	DarkMoon::Ref<DarkMoon::Shader> m_Shader;
	DarkMoon::Ref<DarkMoon::VertexArray> m_VertexArray;

	DarkMoon::Ref<DarkMoon::Shader> m_BlueShader, m_TextureShader;
	DarkMoon::Ref<DarkMoon::VertexArray> m_SquareVertexArray;

	DarkMoon::Ref<DarkMoon::Texture2D> m_Texture2D, m_Texture2D1;

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