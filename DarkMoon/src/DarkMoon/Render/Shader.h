#pragma once

#include "DarkMoon/Core/Core.h"

#include <string>

namespace DarkMoon
{
	class DARKMOON_API Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Use() const = 0;
		virtual void UnUse() const = 0;

		static Shader* Create(const std::string& vertexSource, const std::string& fragmentSource);
	};
}