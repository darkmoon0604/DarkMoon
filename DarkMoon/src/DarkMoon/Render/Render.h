#pragma once

#include "DarkMoon/Core/Core.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace DarkMoon
{
	class DARKMOON_API Render
	{
	public:
		static void BeginScene(OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, 
			const std::shared_ptr<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI()
		{
			return RendererAPI::GetAPI();
		}
	private:
		struct SceneData
		{
			glm::mat4 m_ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}

