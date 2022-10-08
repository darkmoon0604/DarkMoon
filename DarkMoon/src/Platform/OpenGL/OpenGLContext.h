#pragma once

#include "DarkMoon/Render/GraphicsContext.h"

struct GLFWwindow;

namespace DarkMoon
{
	class DARKMOON_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffer() override;
	private:
		GLFWwindow* m_Window;
	};
}

