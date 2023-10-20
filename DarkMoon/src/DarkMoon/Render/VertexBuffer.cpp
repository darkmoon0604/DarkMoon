#include "pch.h"
#include "VertexBuffer.h"
#include "Render.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace DarkMoon
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Render::GetAPI())
		{
		case RendererAPI::API::None:
			DM_CORE_ASSERT(false, "None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
		case RendererAPI::API::DirectX:
			DM_CORE_ASSERT(false, "DirectX is not supported!");
			return nullptr;
		}

		DM_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Render::GetAPI())
		{
		case RendererAPI::API::None:
			DM_CORE_ASSERT(false, "None is not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		case RendererAPI::API::DirectX:
			DM_CORE_ASSERT(false, "DirectX is not supported!");
			return nullptr;
		}

		DM_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}