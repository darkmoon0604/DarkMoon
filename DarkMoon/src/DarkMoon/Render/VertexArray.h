#pragma once
#include "pch.h"
#include "DarkMoon/Core/Core.h"

#include "DarkMoon/Render/VertexBuffer.h"
#include "DarkMoon/Render/IndexBuffer.h"

namespace DarkMoon
{
	class DARKMOON_API VertexArray
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static VertexArray* Create();
	};
}


