#include "ExampleLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

ExampleLayer::ExampleLayer()
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

	std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";
	std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
	m_Shader = DarkMoon::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

	std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

	std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			
			uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

	m_FlatColorShader = DarkMoon::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);
	auto textureShader = m_ShaderLibrary.Load("assets/shaders/texture.glsl");
	m_Texture = DarkMoon::Texture2D::Create("assets/textures/Checkerboard.png");
	m_LogoTexture = DarkMoon::Texture2D::Create("assets/textures/ChernoLogo.png");
	textureShader->Use();
	textureShader->SetInt("u_Texture", 0);
}

void ExampleLayer::OnAttach()
{

}

void ExampleLayer::OnDetach()
{

}

void ExampleLayer::OnUpdate(DarkMoon::TimeStep timeStep)
{
	// Update
	m_CameraController.OnUpdate(timeStep);
	// Render
	DarkMoon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	DarkMoon::RenderCommand::Clear();
	DarkMoon::Render::BeginScene(m_CameraController.GetCamera());
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	m_FlatColorShader->Use();
	m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			DarkMoon::Render::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
		}
	}
	auto textureShader = m_ShaderLibrary.Get("Texture");
	m_Texture->Bind();
	DarkMoon::Render::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	m_LogoTexture->Bind();
	DarkMoon::Render::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	// Triangle
	// DarkMoon::Render::Submit(m_Shader, m_VertexArray);
	DarkMoon::Render::EndScene();
}


void ExampleLayer::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void ExampleLayer::OnEvent(DarkMoon::Event& e)
{
	m_CameraController.OnEvent(e);
}