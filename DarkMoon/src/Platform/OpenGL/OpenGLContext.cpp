#include "pch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
//#include <gl/GL.h>
#include <glad/glad.h>

namespace DarkMoon
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_Window (windowHandle)
	{
		DM_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		DM_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DM_CORE_ASSERT(status, "Initialize glad failed!");

		DM_LOG_CORE_INFO("OpenGL Info:");
		DM_LOG_CORE_INFO("	Vendor : {0}", glGetString(GL_VENDOR));
		DM_LOG_CORE_INFO("	Version : {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffer()
	{
		DM_PROFILE_FUNCTION();
		glfwSwapBuffers(m_Window);
	}
}