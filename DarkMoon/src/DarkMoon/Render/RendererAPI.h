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
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		inline static API GetAPI()
		{
			return s_API;
		}
	private:
		static API s_API;
	};
}


