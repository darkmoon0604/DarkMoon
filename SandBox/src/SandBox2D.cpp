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

}

void SandBox2D::OnDetach()
{

}

void SandBox2D::OnUpdate(DarkMoon::TimeStep timeStep)
{
	m_CameraController.OnUpdate(timeStep);

	DarkMoon::RenderCommand::SetClearColor(m_BackgroundColor);
	DarkMoon::RenderCommand::Clear();

	DarkMoon::Render2D::BeginScene(m_CameraController.GetCamera());

	auto pos = glm::vec2(0.0f, 0.0f);
	auto size = glm::vec2(1.0f, 1.0f);
	DarkMoon::Render2D::DrawQuad(pos, size, m_SquareColor);

	DarkMoon::Render2D::EndScene();
}

void SandBox2D::OnImguiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
	ImGui::End();
}

void SandBox2D::OnEvent(DarkMoon::Event& e)
{
	if (e.GetEventType() == DarkMoon::EventType::KeyPressed)
	{
		//DM_LOG_INFO("{0}", e);
	}
	m_CameraController.OnEvent(e);
}