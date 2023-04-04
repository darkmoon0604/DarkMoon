#pragma once
#include "DarkMoon/Core/Core.h"

namespace DarkMoon
{
	class DARKMOON_API IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual unsigned int GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}