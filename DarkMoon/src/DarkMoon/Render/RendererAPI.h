#pragma once

#include <glm/glm.hpp>

#include "DarkMoon/Core/Core.h"
#include "VertexArray.h"

namespace DarkMoon
{
	class DARKMOON_API RendererAPI
	{
	public:
		enum class API
		{
			None,
			OpenGL = 1,
			DirectX = 2,
		};
	public:
		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0) = 0;
		virtual void DrawArrays(unsigned int count) = 0;

		inline static API GetAPI()
		{
			return s_API;
		}

		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};
}


