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
		glCreateVertexArrays(1, &m_RenderID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RenderID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RenderID);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		DM_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer has no layout!");

		Bind();
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.m_Type),
				element.m_Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.m_Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		Bind();
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}