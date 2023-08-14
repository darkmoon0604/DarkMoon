#pragma once

#include "DarkMoon/Core/Core.h"
#include "RenderCommand.h"
#include "DarkMoon/Camera/OrthographicCamera.h"
#include "DarkMoon/Camera/PerspectiveCamera.h"
#include "Shader.h"

namespace DarkMoon
{
	class DARKMOON_API Render
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint16_t width, uint32_t height);
		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, 
			const Ref<VertexArray>& vertexArray,
			const glm::mat4& transform = glm::mat4(1.0f));

		static void Submit(const Ref<Shader>& shader,
			const Ref<VertexArray>& vertexArray,
			unsigned int count,
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

		static Scope<SceneData> s_SceneData;
	};
}

