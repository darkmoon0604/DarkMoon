#include "pch.h"
#include "Render.h"

namespace DarkMoon
{
	void Render::BeginScene()
	{

	}

	void Render::EndScene()
	{

	}

	void Render::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}