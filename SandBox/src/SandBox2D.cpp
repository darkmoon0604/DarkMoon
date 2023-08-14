#include "SandBox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
		m_Stopped = true;
		float duration = (end - start) * 0.001f;
		m_Func({m_Name, duration});
	}
private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

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
		DM_PROFILE_SCOPE("Renderer Draw");
		DarkMoon::Render2D::BeginScene(m_CameraController.GetCamera());

		DarkMoon::Render2D::DrawQuad(glm::vec2(m_PosX, m_PosY), { m_SizeX, m_SizeY }, m_SquareColor);
		DarkMoon::Render2D::DrawQuad(glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.75f), m_BackgroundColor);
		DarkMoon::Render2D::DrawQuad(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(10.0f, 10.0f), m_CheckerboardTexture);

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