#pragma once

#include "DarkMoon/Core/Core.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DarkMoon
{
	class DARKMOON_API PerspectiveCamera
	{
	public:
		PerspectiveCamera(float aspectRatio, float fov, float cameraNear, float cameraFar);

		void SetProjection(float aspectRatio, float fov, float cameraNear, float cameraFar);

		const glm::vec3& GetPosition() const
		{
			return m_Position;
		}

		void SetPosition(const glm::vec3& postion)
		{
			m_Position = postion;
		}

		const glm::vec3& GetWorldUp() const
		{
			return m_WorldUp;
		}

		void SetWorldUp(const glm::vec3& worldUp)
		{
			m_WorldUp = worldUp;
			UpdateCameraVectors();
		}

		const float GetYaw() const
		{
			return m_Yaw;
		}

		const float GetPitch() const
		{
			return m_Pitch;
		}

		void SetYaw(float yaw)
		{
			m_Yaw = yaw;
			UpdateCameraVectors();
		}

		void SetPitch(float pitch)
		{
			m_Pitch = pitch;
			UpdateCameraVectors();
		}

		void SetYawAndPitch(float yaw, float pitch)
		{
			m_Yaw = yaw;
			m_Pitch = pitch;
			UpdateCameraVectors();
		}

		const glm::vec3& GetUp() const
		{
			return m_Up;
		}

		const glm::vec3& GetFront() const
		{
			return m_Front;
		}

		const glm::vec3& GetRight() const
		{
			return m_Right;
		}

		glm::mat4& GetViewProjectionMatrix()
		{
			return m_ProjectionMatrix * GetViewMatrix();
		}

		glm::mat4& GetViewMatrix()
		{
			return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		}
	private:
		void UpdateCameraVectors();
	private:
		glm::mat4 m_ProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;

		float m_Yaw;
		float m_Pitch;
	};
}


