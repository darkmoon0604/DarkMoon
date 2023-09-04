#include "pch.h"
#include "WindowsInput.h"
#include "DarkMoon/Core/Application.h"

#include <GLFW/glfw3.h>

namespace DarkMoon
{
	bool WindowsInput::IIsKeyPressed(KeyCode keyCode)
	{
		auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(win, static_cast<int32_t>(keyCode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IIsMouseButtonPressed(MouseCode button)
	{
		auto win = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(win, static_cast<int32_t>(button));
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