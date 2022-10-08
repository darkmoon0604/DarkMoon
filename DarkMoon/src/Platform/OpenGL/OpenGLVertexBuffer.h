#pragma once
#include "DarkMoon/Render/VertexBuffer.h"

namespace DarkMoon
{
	class DARKMOON_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;
	private:
		uint32_t m_BufferId;
	};
}


