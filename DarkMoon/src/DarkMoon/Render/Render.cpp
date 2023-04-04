#include "pch.h"
#include "Render.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace DarkMoon
{
	Render::SceneData* Render::s_SceneData = new Render::SceneData;

	void Render::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->m_ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Render::EndScene()
	{

	}

	void Render::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Use();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->m_ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}