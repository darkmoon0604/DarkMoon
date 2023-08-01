#pragma once

#include "OrthographicCamera.h"

#include "DarkMoon/Core/Core.h"
#include "DarkMoon/Core/TimeStep.h"

#include "DarkMoon/Events/ApplicationEvent.h"
#include "DarkMoon/Events/MouseEvent.h"

namespace DarkMoon
{
	class DARKMOON_API OrthographicCameraController
	{
	public: 
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(TimeStep timeStep);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		OrthographicCamera& GetCamera()
		{
			return m_Camera;
		}

		const OrthographicCamera& GetCamera() const
		{
			return m_Camera;
		}

		float GetZoomLevel() const
		{
			return m_ZoomLevel;
		}

		void SetZoomLevel(float level)
		{
			m_ZoomLevel = level;
		}
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 100.0f;
	};
}