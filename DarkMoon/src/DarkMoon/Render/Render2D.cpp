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
	struct QuadVertex
	{
		glm::vec3 m_Position;
		glm::vec4 m_Color;
		glm::vec2 m_TexCoord;
		float m_TexIndex;
		float m_TilingFactor;
	};

	struct Renderer2DData
	{
		const uint32_t m_MaxQuads = 10000;
		const uint32_t m_MaxVertices = m_MaxQuads * 4;
		const uint32_t m_MaxIndices = m_MaxQuads * 6;
		static const uint32_t m_MaxTextureSlots = 32;

		Ref<VertexArray> m_QuadVertexArray;
		Ref<VertexBuffer> m_QuadVertexBuffer;
		Ref<Shader> m_TextureShader;
		Ref<Texture2D> m_WhiteTexture;

		uint32_t m_QuadIndexCount = 0;
		QuadVertex* m_QuadVertexBufferBase = nullptr;
		QuadVertex* m_QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, m_MaxTextureSlots> m_TextureSlots;
		uint32_t m_TextureSlotIndex = 1;

		glm::vec4 m_QuadVertexPositions[4];
	};

	static Renderer2DData s_Data;

	void Render2D::Init()
	{
		DM_PROFILE_FUNCTION();
		s_Data.m_QuadVertexArray = VertexArray::Create();
		s_Data.m_QuadVertexBuffer = VertexBuffer::Create(s_Data.m_MaxVertices * sizeof(QuadVertex));
		s_Data.m_QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "aPos"},
			{ ShaderDataType::Float4, "aColor"},
			{ ShaderDataType::Float2, "aTexcoord"},
			{ ShaderDataType::Float, "aTexIndex"},
			{ ShaderDataType::Float, "aTilingFactor"},
		});
		s_Data.m_QuadVertexArray->AddVertexBuffer(s_Data.m_QuadVertexBuffer);
		s_Data.m_QuadVertexBufferBase = new QuadVertex[s_Data.m_MaxVertices];
		uint32_t* quadIndices = new uint32_t[s_Data.m_MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.m_MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.m_MaxIndices);
		s_Data.m_QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		s_Data.m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.m_WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samples[s_Data.m_MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.m_MaxTextureSlots; i++)
		{
			samples[i] = i;
		}

		s_Data.m_TextureShader = Shader::Create("assets/shaders/texture.glsl");
		s_Data.m_TextureShader->Use();
		//s_Data.m_TextureShader->SetInt("uTexture", 0);
		s_Data.m_TextureShader->SetIntArray("uTextures", samples, s_Data.m_MaxTextureSlots);
		// set all texture slots to 0
		s_Data.m_TextureSlots[0] = s_Data.m_WhiteTexture;

		s_Data.m_QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.m_QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Render2D::Shutdown()
	{
		DM_PROFILE_FUNCTION();
	}

	void Render2D::BeginScene(const OrthographicCamera& camera)
	{
		DM_PROFILE_FUNCTION();
		s_Data.m_TextureShader->Use();
		s_Data.m_TextureShader->SetMat4("uViewProjection", camera.GetViewProjectionMatrix());

		s_Data.m_QuadIndexCount = 0;
		s_Data.m_QuadVertexBufferPtr = s_Data.m_QuadVertexBufferBase;

		s_Data.m_TextureSlotIndex = 1;
	}

	void Render2D::EndScene()
	{
		DM_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_Data.m_QuadVertexBufferPtr - (uint8_t*)s_Data.m_QuadVertexBufferBase;
		s_Data.m_QuadVertexBuffer->SetData(s_Data.m_QuadVertexBufferBase, dataSize);
		Flush();
	}

	void Render2D::Flush()
	{
		// bind textures
		for (uint32_t i = 0; i < s_Data.m_TextureSlotIndex; i++)
		{
			s_Data.m_TextureSlots[i]->Bind(i);
		}

		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray, s_Data.m_QuadIndexCount);
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DM_PROFILE_FUNCTION();

		const float texIndex = 0.0f;
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[0];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[1];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 1.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[2];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 1.0f, 1.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[3];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 1.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadIndexCount += 6;
	}

	void Render2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor/* = 1.0f*/, const glm::vec4& tintColor/* = glm::vec4(1.0f)*/)
	{
		DrawQuad({ position.x, position.y, 1.0f }, size, texture, tilingFactor, tintColor);
	}

	void Render2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor/* = 1.0f*/, const glm::vec4& tintColor/* = glm::vec4(1.0f)*/)
	{
		DM_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float texIndex = 0.0f;
		for (uint32_t i = 0; i < s_Data.m_TextureSlotIndex; i++)
		{
			if (*s_Data.m_TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.m_TextureSlotIndex;
			s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = texture;
			s_Data.m_TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[0];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[1];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 1.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[2];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 1.0f, 1.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[3];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 1.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadIndexCount += 6;
#if 0
		s_Data.m_TextureShader->SetFloat4("uColor", tintColor);
		s_Data.m_TextureShader->SetFloat("uTilingFactor", tilingFactor);
		texture->Bind();
		auto transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.m_TextureShader->SetMat4("uTransform", transform);
		s_Data.m_QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);
#endif
	}

	void Render2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 1.0f }, size, rotation, color);
	}

	void Render2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DM_PROFILE_FUNCTION();
		/*s_Data.m_TextureShader->SetFloat4("uColor", color);
		s_Data.m_TextureShader->SetFloat("uTilingFactor", 1.0f);
		s_Data.m_WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.m_TextureShader->SetMat4("uTransform", transform);
		s_Data.m_QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);*/

		const float texIndex = 0.0f; // white texture
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[0];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[1];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[2];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[3];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadIndexCount += 6;
	}

	void Render2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		DrawRotatedQuad({ position.x, position.y, 1.0f }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Render2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor /*= 1.0f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		DM_PROFILE_FUNCTION();
		/*s_Data.m_TextureShader->SetFloat4("uColor", tintColor);
		s_Data.m_TextureShader->SetFloat("uTilingFactor", tilingFactor);
		s_Data.m_WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data.m_TextureShader->SetMat4("uTransform", transform);
		s_Data.m_QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.m_QuadVertexArray);*/

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float texIndex = 0.0f;
		for (uint32_t i = 1; i < s_Data.m_QuadIndexCount; i++)
		{
			if (*s_Data.m_TextureSlots[i].get() == *texture.get())
			{
				texIndex = (float)i;
				break;
			}
		}

		if (texIndex == 0.0f)
		{
			texIndex = (float)s_Data.m_TextureSlotIndex;
			s_Data.m_TextureSlots[s_Data.m_TextureSlotIndex] = texture;
			s_Data.m_TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[0];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[1];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[2];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadVertexBufferPtr->m_Position = transform * s_Data.m_QuadVertexPositions[3];
		s_Data.m_QuadVertexBufferPtr->m_Color = color;
		s_Data.m_QuadVertexBufferPtr->m_TexCoord = { 0.0f, 0.0f };
		s_Data.m_QuadVertexBufferPtr->m_TexIndex = texIndex;
		s_Data.m_QuadVertexBufferPtr->m_TilingFactor = tilingFactor;
		s_Data.m_QuadVertexBufferPtr++;

		s_Data.m_QuadIndexCount += 6;
	}

}