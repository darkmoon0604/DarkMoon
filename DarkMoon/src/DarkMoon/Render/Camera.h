#pragma once

#include "DarkMoon/Core/Core.h"

#include <glm/glm.hpp>

namespace DarkMoon
{
	class DARKMOON_API Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;
		virtual const glm::vec3& GetPosition() const = 0;
		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
	};
}