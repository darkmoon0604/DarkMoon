#include "SandBox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

SandBox2D::SandBox2D()
	: Layer("SandBox2D"), m_CameraController(1280.0f/ 720.0f)
{

}

void SandBox2D::OnAttach()
{
	m_SquareVA = DarkMoon::VertexArray::Create();

	float squareVertices[5 * 4] = 
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	DarkMoon::Ref<DarkMoon::VertexBuffer> squareVB;
	squareVB.reset(DarkMoon::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	squareVB->SetLayout(
		{
			{ DarkMoon::ShaderDataType::Float3, "aPos" }
		}
	);
	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	DarkMoon::Ref<DarkMoon::IndexBuffer> squareIB;
	squareIB.reset(DarkMoon::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);
	m_FlatColorShader = DarkMoon::Shader::Create("assets/shaders/FlatColor.glsl");
}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(DarkMoon::TimeStep timeStep)
{
	m_CameraController.OnUpdate(timeStep);

	DarkMoon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	DarkMoon::RenderCommand::Clear();

	DarkMoon::Render::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_FlatColorShader)->Use();
	std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("uColor", m_SquareColor);

	DarkMoon::Render::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	DarkMoon::Render::EndScene();
}

void SandBox2D::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void SandBox2D::OnEvent(DarkMoon::Event& e)
{
	m_CameraController.OnEvent(e);
}