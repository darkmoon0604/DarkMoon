#include "pch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace DarkMoon
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case DarkMoon::ShaderDataType::Float:
		case DarkMoon::ShaderDataType::Float2:
		case DarkMoon::ShaderDataType::Float3:
		case DarkMoon::ShaderDataType::Float4:
		case DarkMoon::ShaderDataType::Mat3:
		case DarkMoon::ShaderDataType::Mat4:
			return GL_FLOAT;
		case DarkMoon::ShaderDataType::Int:
		case DarkMoon::ShaderDataType::Int2:
		case DarkMoon::ShaderDataType::Int3:
		case DarkMoon::ShaderDataType::Int4:
			return GL_INT;
		case DarkMoon::ShaderDataType::Bool:
			return GL_BOOL;
		}

		DM_CORE_ASSERT("false", "Unknown shader data type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		DM_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_RenderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		DM_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &m_RenderID);
	}

	void OpenGLVertexArray::Bind() const
	{
		DM_PROFILE_FUNCTION();
		glBindVertexArray(m_RenderID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		DM_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		DM_PROFILE_FUNCTION();
		DM_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		Bind();
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.m_Type),
				element.m_Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)(intptr_t)element.m_Offset);
			m_VertexBufferIndex++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		DM_PROFILE_FUNCTION();
		Bind();
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}