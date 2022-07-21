#include "pch.h"
#include "ImguiLayer.h"
#include "DarkMoon/Application.h"
#include "Platform/OpenGL/imguiOpenGLRender.h"

#include <imgui.h>
#include <GLFW/glfw3.h>

namespace DarkMoon
{
	ImguiLayer::ImguiLayer()
		: Layer("ImguiLayer")
	{

	}

	ImguiLayer::~ImguiLayer()
	{

	}

	void ImguiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImguiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
	}

	void ImguiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
		
		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;
	
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool isShow = true;
		ImGui::ShowDemoWindow(&isShow);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImguiLayer::OnEvent(Event& e)
	{

	}
}