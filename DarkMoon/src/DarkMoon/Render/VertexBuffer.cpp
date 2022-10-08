#include "pch.h"
#include "VertexBuffer.h"
#include "Render.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace DarkMoon
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Render::GetRenderAPI())
		{
		case RenderAPI::None:
			DM_CORE_ASSERT(false, "None is not supported!");
			return nullptr;
		case RenderAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		case RenderAPI::DirectX:
			DM_CORE_ASSERT(false, "DirectX is not supported!");
			return nullptr;
		}

		DM_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}