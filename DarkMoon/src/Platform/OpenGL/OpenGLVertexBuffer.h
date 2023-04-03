#pragma once
#include "DarkMoon/Render/VertexBuffer.h"

namespace DarkMoon
{
	class DARKMOON_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const
		{
			return m_BufferLayout;
		}

		virtual void SetLayout(const BufferLayout& layout) override
		{
			m_BufferLayout = layout;
		}
	private:
		uint32_t m_BufferId;
		BufferLayout m_BufferLayout;
	};
}


