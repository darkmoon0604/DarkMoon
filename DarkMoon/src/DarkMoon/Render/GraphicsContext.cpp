#include "pch.h"
#include "GraphicsContext.h"

#include "DarkMoon/Render/Render.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace DarkMoon
{
	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
			DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}