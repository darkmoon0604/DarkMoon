#pragma once

#include "DarkMoon/Core/Core.h"
#include "DarkMoon/Render/Camera.h"

namespace DarkMoon
{
	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera() {}

		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		virtual const glm::vec3& GetPosition() const override
		{
			return m_Position;
		}

		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
		}

		float GetRotation() const
		{
			return m_Rotation;
		}

		void SetRotation(float rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		virtual const glm::mat4& GetProjectionMatrix() const override
		{
			return m_ProjectionMatrix;
		}

		virtual const glm::mat4& GetViewMatrix() const override
		{
			return m_ViewMatrix;
		}

		virtual const glm::mat4& GetViewProjectionMatrix() const override
		{
			return m_ViewProjectionMatrix;
		}
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}


