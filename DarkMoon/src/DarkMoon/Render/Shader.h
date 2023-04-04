#pragma once

#include "DarkMoon/Core.h"

#include <string>
#include <glm/glm.hpp>

namespace DarkMoon
{
	class DARKMOON_API Shader
	{
	public:
		Shader(const std::string vertexSource, const std::string fragmentSource);
		~Shader();

		void Use() const;
		void UnUse() const;
		void UpLoadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_ShaderId;
	};
}