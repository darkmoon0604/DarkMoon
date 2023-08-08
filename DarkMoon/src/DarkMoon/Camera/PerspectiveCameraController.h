#pragma once

#include "PerspectiveCamera.h"
#include "DarkMoon/Core/Core.h"
#include "DarkMoon/Core/TimeStep.h"
#include "DarkMoon/Events/ApplicationEvent.h"
#include "DarkMoon/Events/MouseEvent.h"

namespace DarkMoon
{
	class DARKMOON_API PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float width, float height, float fov = 45.0f, float cameraNear = 0.1f, float cameraFar = 100.0f);

		void OnUpdate(TimeStep timeStep);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		PerspectiveCamera& GetCamera()
		{
			return m_Camera;
		}

		const PerspectiveCamera& GetCamera() const
		{
			return m_Camera;
		}
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		PerspectiveCamera m_Camera;

		float m_AspectRatio;
		float m_MovementSpeed = 2.5f;
		float m_MouseSensitivity = 0.1f;
		float m_Fov;
		float m_CameraNear;
		float m_CameraFar;
	};
}
