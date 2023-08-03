#include "pch.h"
#include "Texture.h"

#include "Render.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace DarkMoon
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Render::GetAPI())
		{
		case RendererAPI::API::None:
			DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(path);
		case RendererAPI::API::DirectX:
			DM_CORE_ASSERT(false, "RendererAPI::DirectX is currently not supported!");
			return nullptr;
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	DarkMoon::Ref<DarkMoon::Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Render::GetAPI())
		{
		case RendererAPI::API::None:
			DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLTexture2D>(width, height);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}