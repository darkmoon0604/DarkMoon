#pragma once
#include "DarkMoon/Core.h"

namespace DarkMoon
{
	class DARKMOON_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};
}