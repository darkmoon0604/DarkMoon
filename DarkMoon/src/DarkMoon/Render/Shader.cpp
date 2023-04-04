#include "pch.h"
#include "Shader.h"
#include "Render.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace DarkMoon
{
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Render::GetAPI())
		{
		case  RendererAPI::API::None:
			DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(vertexSource, fragmentSource);
		case  RendererAPI::API::DirectX:
			DM_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
			return nullptr;
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}