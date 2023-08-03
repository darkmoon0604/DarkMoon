#include "pch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace DarkMoon
{
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
		case DarkMoon::RendererAPI::API::None:
			DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
			return nullptr;
		case DarkMoon::RendererAPI::API::OpenGL:
			return CreateScope<OpenGLRendererAPI>();
		case DarkMoon::RendererAPI::API::DirectX:
			DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!")
			return nullptr;
		default:
			break;
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!")
		return nullptr;
	}
}