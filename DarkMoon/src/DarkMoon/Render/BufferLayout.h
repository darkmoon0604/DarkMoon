#pragma once
#include "pch.h"
#include "DarkMoon/Core/Core.h"

namespace DarkMoon
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool,
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case DarkMoon::ShaderDataType::Float:
			return 4;
		case DarkMoon::ShaderDataType::Float2:
			return 4 * 2;
		case DarkMoon::ShaderDataType::Float3:
			return 4 * 3;
		case DarkMoon::ShaderDataType::Float4:
			return 4 * 4;
		case DarkMoon::ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case DarkMoon::ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case DarkMoon::ShaderDataType::Int:
			return 4;
		case DarkMoon::ShaderDataType::Int2:
			return 4 * 2;
		case DarkMoon::ShaderDataType::Int3:
			return 4 * 3;
		case DarkMoon::ShaderDataType::Int4:
			return 4 * 4;
		case DarkMoon::ShaderDataType::Bool:
			return 1;
		}

		DM_CORE_ASSERT(false, "Unknown shader data type");
		return 0;
	}

	struct BufferElement
	{
		std::string m_Name;
		ShaderDataType m_Type;
		uint32_t m_Size;
		uint32_t m_Offset;
		bool m_Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) 
			: m_Name(name), m_Type(type), m_Size(ShaderDataTypeSize(type)), m_Offset(0), m_Normalized(normalized)
		{

		}

		uint32_t GetComponentCount() const
		{
			switch (m_Type)
			{
			case DarkMoon::ShaderDataType::Float:
				return 1;
			case DarkMoon::ShaderDataType::Float2:
				return 2;
			case DarkMoon::ShaderDataType::Float3:
				return 3;
			case DarkMoon::ShaderDataType::Float4:
				return 4;
			case DarkMoon::ShaderDataType::Mat3:
				return 3 * 3;
			case DarkMoon::ShaderDataType::Mat4:
				return 4 * 4;
			case DarkMoon::ShaderDataType::Int:
				return 1;
			case DarkMoon::ShaderDataType::Int2:
				return 2;
			case DarkMoon::ShaderDataType::Int3:
				return 3;
			case DarkMoon::ShaderDataType::Int4:
				return 4;
			case DarkMoon::ShaderDataType::Bool:
				return 1;
			}

			DM_CORE_ASSERT(false, "Unknown shader data type");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const
		{
			return m_Stride;
		}

		inline const std::vector<BufferElement>& GetElements() const
		{
			return m_Elements;
		}

		std::vector<BufferElement>::iterator begin()
		{
			return m_Elements.begin();
		}

		std::vector<BufferElement>::iterator end()
		{
			return m_Elements.end();
		}

		std::vector<BufferElement>::const_iterator begin() const
		{
			return m_Elements.begin();
		}

		std::vector<BufferElement>::const_iterator end() const
		{
			return m_Elements.end();
		}
	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.m_Offset = offset;
				offset += element.m_Size;
				m_Stride += element.m_Size;
			}
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};
}