#include "pch.h"
#include "OpenGLVertexBuffer.h"

#include <glad/glad.h>

namespace DarkMoon
{

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		DM_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_BufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		DM_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_BufferId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		DM_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		DM_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
