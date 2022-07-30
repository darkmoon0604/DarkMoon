#include "pch.h"
#include "ImguiLayer.h"
#include "DarkMoon/Application.h"
#include "Platform/OpenGL/imguiOpenGLRender.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

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
		ImGui::DestroyContext();
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
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(DM_BIND_EVENT_FUNC(ImguiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(DM_BIND_EVENT_FUNC(ImguiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(DM_BIND_EVENT_FUNC(ImguiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<MouseMovedEvent>(DM_BIND_EVENT_FUNC(ImguiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(DM_BIND_EVENT_FUNC(ImguiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(DM_BIND_EVENT_FUNC(ImguiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(DM_BIND_EVENT_FUNC(ImguiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(DM_BIND_EVENT_FUNC(ImguiLayer::OnWindowResizedEvent));
	}

	bool ImguiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;
		return false;
	}

	bool ImguiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;
		return false;
	}

	bool ImguiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();
		return false;
	}

	bool ImguiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool ImguiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetCode()] = true;

		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImguiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetCode()] = false;
		return false;
	}

	bool ImguiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetCode();
		if (keycode > 0 && keycode < 0x10000)
		{
			io.AddInputCharacter((unsigned short)keycode);
		}
		return false;
	}

	bool ImguiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0.0f, 0.0f, e.GetWidth(), e.GetHeight());
		return false;
	}

}