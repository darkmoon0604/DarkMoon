#include "pch.h"
#include "VertexArray.h"

#include "Render.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace DarkMoon
{
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Render::GetAPI())
		{
		case RendererAPI::API::None:
			DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLVertexArray>();
		case RendererAPI::API::DirectX:
			DM_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
			return nullptr;
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}