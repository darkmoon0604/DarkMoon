#pragma once

#include "DarkMoon/Core.h"

#include <string>

namespace DarkMoon
{
	class DARKMOON_API Shader
	{
	public:
		Shader(const std::string vertexSource, const std::string fragmentSource);
		~Shader();

		void Use() const;
		void UnUse() const;
	private:
		uint32_t m_ShaderId;
	};
}