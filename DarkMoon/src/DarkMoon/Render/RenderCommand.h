#pragma once

#include "DarkMoon/Core/Core.h"
#include "RendererAPI.h"

namespace DarkMoon
{
	class DARKMOON_API RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, count);
		}

		inline static void DrawArrays(unsigned int count)
		{
			s_RendererAPI->DrawArrays(count);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}


