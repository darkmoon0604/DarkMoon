#pragma once

#include "DarkMoon/Core/Core.h"
#include "DarkMoon/Render/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace DarkMoon
{
	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera() { }

		PerspectiveCamera(float aspectRatio, float fov, float cameraNear, float cameraFar);

		void SetProjection(float aspectRatio, float fov, float cameraNear, float cameraFar);

		virtual const glm::vec3& GetPosition() const override
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
		
		virtual const glm::mat4& GetProjectionMatrix() const override
		{
			return m_ProjectionMatrix;
		}

		virtual const glm::mat4& GetViewProjectionMatrix() const override
		{
			return m_ProjectionMatrix * GetViewMatrix();
		}

		virtual const glm::mat4& GetViewMatrix() const override
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


