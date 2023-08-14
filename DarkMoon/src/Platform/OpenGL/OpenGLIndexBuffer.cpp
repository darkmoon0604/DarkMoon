#include "pch.h"
#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

namespace DarkMoon
{

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		DM_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_BufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		DM_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_BufferId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		DM_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		DM_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}