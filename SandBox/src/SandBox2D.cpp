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
	DM_PROFILE_FUNCTION();
	m_CheckerboardTexture = DarkMoon::Texture2D::Create("assets/textures/Checkerboard.png");
}

void SandBox2D::OnDetach()
{
	DM_PROFILE_FUNCTION();
}

void SandBox2D::OnUpdate(DarkMoon::TimeStep timeStep)
{
	DM_PROFILE_FUNCTION();

	m_CameraController.OnUpdate(timeStep);

	{
		DM_PROFILE_SCOPE("Renderer Prep");
		DarkMoon::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		DarkMoon::RenderCommand::Clear();
	}

	{
		static float rotation = 0.0f;
		rotation += timeStep * 50.0f;

		DM_PROFILE_SCOPE("Renderer Draw");
		DarkMoon::Render2D::BeginScene(m_CameraController.GetCamera());

		DarkMoon::Render2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
		DarkMoon::Render2D::DrawQuad(glm::vec2(-1.0f, 0.0f), glm::vec2(0.8f, 0.8f), m_BackgroundColor);
		//DarkMoon::Render2D::DrawQuad(glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.75f), m_BackgroundColor);
		//DarkMoon::Render2D::DrawQuad(glm::vec3(-5.0f, -5.0f, -0.1f), glm::vec2(10.0f, 10.0f), m_CheckerboardTexture, 10.0f);
		//DarkMoon::Render2D::DrawQuad(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 1.0f), m_CheckerboardTexture, 20.0f);
		DarkMoon::Render2D::DrawRotatedQuad({ -2.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, rotation, m_CheckerboardTexture, m_Factor);
		DarkMoon::Render2D::EndScene();
	}
}

void SandBox2D::OnImguiRender()
{
	DM_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::ColorEdit4("Background Color", glm::value_ptr(m_BackgroundColor));
	ImGui::SliderFloat("Square SizeX", &m_SizeX, 0.1f, 1.0f);
	ImGui::SliderFloat("Square SizeY", &m_SizeY, 0.1f, 1.0f);
	ImGui::SliderFloat("Square PosX", &m_PosX, -1.0f, 1.0f);
	ImGui::SliderFloat("Square PosY", &m_PosY, -1.0f, 1.0f);
	ImGui::SliderFloat("Factor", &m_Factor, 0.1f, 10.0f);

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