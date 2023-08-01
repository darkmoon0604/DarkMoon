#include "pch.h"
#include "WindowsInput.h"
#include "DarkMoon/Core/Application.h"

#include <GLFW/glfw3.h>

namespace DarkMoon
{
	Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

	bool WindowsInput::IIsKeyPressed(int keyCode)
	{
		auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(win, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IIsMouseButtonPressed(int button)
	{
		auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(win, button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::IGetMousePositionX()
	{
		auto [x, y] = GetMousePosition();	// C++17 结构化绑定语法
		return x;
	}

	float WindowsInput::IGetMousePositionY()
	{
		auto value = GetMousePosition();	// C++17 结构化绑定语法
		return std::get<1>(value);
	}

	std::pair<float, float> WindowsInput::IGetMousePosition()
	{
		auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(win, &x, &y);
		return { (float)x, (float)y };
	}

}