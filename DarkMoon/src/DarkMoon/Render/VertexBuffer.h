#pragma once
#include "DarkMoon/Core/Core.h"
#include "DarkMoon/Render/BufferLayout.h"

namespace DarkMoon
{
	class DARKMOON_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static DarkMoon::Ref<VertexBuffer> Create(float* vertices, uint32_t size);
	};
}