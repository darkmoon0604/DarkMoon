#include "DarkMoon.h"
#include "DarkMoon/Core/EntryPoint.h"

#include "Imgui/imgui.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SandBox2D.h"
#include "SandBox3D.h"

class ExampleLayer : public DarkMoon::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"), m_CameraController(1280.0f / 720.0f, false)
	{
		float vertices[3 * 7]
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		m_VertexArray = DarkMoon::VertexArray::Create();
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
		m_SquareVertexArray = DarkMoon::VertexArray::Create();
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

		m_Shader = m_ShaderLibary.Load("assets/shaders/triangle.glsl");
		m_BlueShader = m_ShaderLibary.Load("assets/shaders/square.glsl");
		m_TextureShader = m_ShaderLibary.Load("assets/shaders/texture.glsl");

		m_Texture2D = DarkMoon::Texture2D::Create("assets/textures/Checkerboard.png");
		m_Texture2D1 = DarkMoon::Texture2D::Create("assets/textures/awesomeface.png");

		std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_TextureShader)->Use();
		std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_TextureShader)->UploadUniformInt("uTexture", 0);
	}

	void OnUpdate(DarkMoon::TimeStep timeStep) override
	{
		// camera
		m_CameraController.OnUpdate(timeStep);

		// render
		DarkMoon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		DarkMoon::RenderCommand::Clear();

		DarkMoon::Render::BeginScene(m_CameraController.GetCamera());
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
		m_CameraController.OnEvent(e);
	}

	void OnImguiRender() override
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}
private:
	DarkMoon::ShaderLibrary m_ShaderLibary;
	DarkMoon::Ref<DarkMoon::Shader> m_Shader;
	DarkMoon::Ref<DarkMoon::VertexArray> m_VertexArray;

	DarkMoon::Ref<DarkMoon::Shader> m_BlueShader, m_TextureShader;
	DarkMoon::Ref<DarkMoon::VertexArray> m_SquareVertexArray;

	DarkMoon::Ref<DarkMoon::Texture2D> m_Texture2D, m_Texture2D1;

	DarkMoon::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class SandBox : public DarkMoon::Application
{
public:
	SandBox()
	{
		GetWindow().IsShowCursor(true);
		//PushLayer(new ExampleLayer());
		//PushLayer(new SandBox2D());
		PushLayer(new SandBox3D());
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