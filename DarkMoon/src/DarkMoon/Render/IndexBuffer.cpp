#include "pch.h"
#include "IndexBuffer.h"
#include "Render.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace DarkMoon
{
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Render::GetAPI())
		{
		case RendererAPI::API::None:
			DM_CORE_ASSERT(false, "None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		case RendererAPI::API::DirectX:
			DM_CORE_ASSERT(false, "DirectX is not supported!");
			return nullptr;
		}

		DM_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}