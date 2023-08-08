#include "SandBox3D.h"

#include "imgui/imgui.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

SandBox3D::SandBox3D()
	: Layer("SandBox3D"), m_CameraController(1280.0f, 720.0f)
{

}

void SandBox3D::OnAttach()
{
	m_SquareVA = DarkMoon::VertexArray::Create();

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	DarkMoon::Ref<DarkMoon::VertexBuffer> squareVB;
	squareVB.reset(DarkMoon::VertexBuffer::Create(vertices, sizeof(vertices)));
	squareVB->SetLayout(
		{
			{ DarkMoon::ShaderDataType::Float3, "aPos" }
		}
	);
	m_SquareVA->AddVertexBuffer(squareVB);

	m_FlatColorShader = DarkMoon::Shader::Create("assets/shaders/flatcolor.glsl");

	m_CameraController.GetCamera().SetPosition({ 0.0f, 0.0f,  3.0f });
}

void SandBox3D::OnDetach()
{
	
}

void SandBox3D::OnUpdate(DarkMoon::TimeStep timeStep)
{
	// camera
	m_CameraController.OnUpdate(timeStep);

	DarkMoon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	DarkMoon::RenderCommand::Clear();

	DarkMoon::Render::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_FlatColorShader)->Use();
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
	std::dynamic_pointer_cast<DarkMoon::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("uColor", m_SquareColor);
	DarkMoon::Render::Submit(m_FlatColorShader, m_SquareVA, 36, model);

	DarkMoon::Render::EndScene();
}

void SandBox3D::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));


	for (auto& result : m_ProfileResults)
	{
		ImGui::Text(result.c_str());
	}
	if (m_ProfileResults.size() > 1)
	{
		m_ProfileResults.erase(m_ProfileResults.begin());
	}

	ImGui::End();
}

void SandBox3D::OnEvent(DarkMoon::Event& e)
{
	m_CameraController.OnEvent(e);

	//DM_LOG_INFO("{0}", e);
	if (e.GetEventType() == DarkMoon::EventType::MouseMoved)
		m_ProfileResults.push_back({ e.ToString() });
}
