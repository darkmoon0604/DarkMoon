#pragma once

#include "DarkMoon/Core.h"
#include "RenderCommand.h"

namespace DarkMoon
{
	class DARKMOON_API Render
	{
	public:
		static void BeginScene();
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI()
		{
			return RendererAPI::GetAPI();
		}
	};
}

