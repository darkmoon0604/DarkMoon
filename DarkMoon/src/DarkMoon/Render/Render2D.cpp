#include "pch.h"
#include "Render2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DarkMoon
{
	struct Render2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Render2DStorage* s_Data;

	void Render2D::Init()
	{
		s_Data = new Render2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = 
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout(
			{
				{
					ShaderDataType::Float3, "aPos"
				},
				{
					ShaderDataType::Float2, "aTexcoord"
				},
			}
		);
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);
	
		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->TextureShader = Shader::Create("assets/shaders/texture.glsl");
		s_Data->TextureShader->Use();
		s_Data->TextureShader->SetInt("uTexture", 0);
	}

	void Render2D::Shutdown()
	{
		delete s_Data;
	}

	void Render2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->TextureShader->Use();
		s_Data->TextureShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());
	}

	void Render2D::EndScene()
	{

	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, glm::vec4& color)
	{
		s_Data->TextureShader->SetFloat4("uColor", color);
		s_Data->WhiteTexture->Bind();

		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("uTransform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, texture);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, Ref<Texture2D>& texture)
	{
		s_Data->TextureShader->SetFloat4("uColor", glm::vec4(1.0f));
		texture->Bind();
		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("uTransform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}