#pragma once
#include "DarkMoon/Render/IndexBuffer.h"

namespace DarkMoon
{
	class DARKMOON_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;

		uint32_t GetCount() const
		{
			return m_Count;
		}
	private:
		uint32_t m_BufferId;
		uint32_t m_Count;
	};
}