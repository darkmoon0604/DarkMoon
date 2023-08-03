#pragma once

#include "DarkMoon/Camera/OrthographicCamera.h"
#include "DarkMoon/Render/Texture.h"

namespace DarkMoon
{
	class DARKMOON_API Render2D
	{
	public: 
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture);
	};
}