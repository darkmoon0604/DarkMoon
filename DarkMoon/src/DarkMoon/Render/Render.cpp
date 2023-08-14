#include "pch.h"
#include "Render.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Render2D.h"

namespace DarkMoon
{
	Scope<Render::SceneData> Render::s_SceneData = CreateScope<Render::SceneData>();

	void Render::Init()
	{
		DM_PROFILE_FUNCTION();
		RenderCommand::Init();
		Render2D::Init();
	}

	void Render::Shutdown()
	{
		Render2D::Shutdown();
	}

	void Render::OnWindowResize(uint16_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Render::BeginScene(Camera& camera)
	{
		s_SceneData->m_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Render::EndScene()
	{

	}

	void Render::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Use();
		shader->SetMat4("uViewProjection", s_SceneData->m_ViewProjectionMatrix);
		shader->SetMat4("uTransform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Render::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, unsigned int count, const glm::mat4& transform /*= glm::mat4(1.0f)*/)
	{
		shader->Use();
		shader->SetMat4("uViewProjection", s_SceneData->m_ViewProjectionMatrix);
		shader->SetMat4("uTransform", transform);
		vertexArray->Bind();
		RenderCommand::DrawArrays(count);
	}

}