#include "pch.h"
#include "IndexBuffer.h"
#include "Render.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace DarkMoon
{
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Render::GetRenderAPI())
		{
		case RenderAPI::None:
			DM_CORE_ASSERT(false, "None is not supported!");
			return nullptr;
		case RenderAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
		case RenderAPI::DirectX:
			DM_CORE_ASSERT(false, "DirectX is not supported!");
			return nullptr;
		}

		DM_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}