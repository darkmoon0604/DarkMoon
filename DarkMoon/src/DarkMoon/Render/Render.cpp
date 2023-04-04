#include "pch.h"
#include "Render.h"

namespace DarkMoon
{
	Render::SceneData* Render::m_SceneData = new Render::SceneData;

	void Render::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->m_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Render::EndScene()
	{

	}

	void Render::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Use();
		shader->UpLoadUniformMat4("u_ViewProjection", m_SceneData->m_ViewProjectionMatrix);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}