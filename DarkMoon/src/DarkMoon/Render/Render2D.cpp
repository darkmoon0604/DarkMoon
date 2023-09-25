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
		DM_PROFILE_FUNCTION();
		s_Data = new Render2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float squareVertices[5 * 4] = 
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
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
		Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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
		DM_PROFILE_FUNCTION();
		delete s_Data;
	}

	void Render2D::BeginScene(const OrthographicCamera& camera)
	{
		DM_PROFILE_FUNCTION();
		s_Data->TextureShader->Use();
		s_Data->TextureShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());
	}

	void Render2D::EndScene()
	{
		DM_PROFILE_FUNCTION();
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DM_PROFILE_FUNCTION();
		s_Data->TextureShader->SetFloat4("uColor", color);
		s_Data->TextureShader->SetFloat("uTilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind();

		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("uTransform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor/* = 1.0f*/, const glm::vec4& tintColor/* = glm::vec4(1.0f)*/)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, texture, tilingFactor, tintColor);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor/* = 1.0f*/, const glm::vec4& tintColor/* = glm::vec4(1.0f)*/)
	{
		DM_PROFILE_FUNCTION();
		s_Data->TextureShader->SetFloat4("uColor", tintColor);
		s_Data->TextureShader->SetFloat("uTilingFactor", tilingFactor);
		texture->Bind();
		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("uTransform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Render2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 1.0f }, size, rotation, color);
	}

	void Render2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DM_PROFILE_FUNCTION();
		s_Data->TextureShader->SetFloat4("uColor", color);
		s_Data->TextureShader->SetFloat("uTilingFactor", 1.0f);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("uTransform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Render2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		DrawRotatedQuad({ position.x, position.y, 1.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Render2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		DM_PROFILE_FUNCTION();
		s_Data->TextureShader->SetFloat4("uColor", tintColor);
		s_Data->TextureShader->SetFloat("uTilingFactor", tilingFactor);
		s_Data->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->TextureShader->SetMat4("uTransform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

}