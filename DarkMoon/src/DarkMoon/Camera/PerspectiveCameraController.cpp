#include "pch.h"
#include "PerspectiveCameraController.h"

#include "DarkMoon/Core/Input.h"
#include "DarkMoon/Core/KeyCodes.h"

namespace DarkMoon
{
	bool m_IsFirstMouse = true;
	float m_LastPosX, m_LastPosY;

	PerspectiveCameraController::PerspectiveCameraController(float width, float height, float fov /*= 45.0f*/, float cameraNear /*= 0.1f*/, float cameraFar /*= 100.0f*/)
		: m_Camera(width / height, fov, cameraNear, cameraFar), m_AspectRatio(width / height), m_Fov(fov), m_CameraNear(cameraNear), m_CameraFar(cameraFar), m_MovementSpeed(2.5f), m_MouseSensitivity(0.01f)
	{
		m_LastPosX = width / 2.0f;
		m_LastPosY = height / 2.0f;
	}

	void PerspectiveCameraController::OnUpdate(TimeStep timeStep)
	{
		float velocity = m_MovementSpeed * timeStep;
		if (DarkMoon::Input::IsKeyPressed(DM_KEY_A))
		{
			glm::vec3 pos = m_Camera.GetPosition() + m_Camera.GetRight() * velocity;
			m_Camera.SetPosition(pos);
		}
		else if (DarkMoon::Input::IsKeyPressed(DM_KEY_D))
		{
			glm::vec3 pos = m_Camera.GetPosition() - m_Camera.GetRight() * velocity;
			m_Camera.SetPosition(pos);
		}

		if (DarkMoon::Input::IsKeyPressed(DM_KEY_W))
		{
			glm::vec3 pos = m_Camera.GetPosition() - m_Camera.GetFront() * velocity;
			m_Camera.SetPosition(pos);
		}
		else if (DarkMoon::Input::IsKeyPressed(DM_KEY_S))
		{
			glm::vec3 pos = m_Camera.GetPosition() + m_Camera.GetFront() * velocity;
			m_Camera.SetPosition(pos);
		}
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseMovedEvent>(DM_BIND_EVENT_FUNC(PerspectiveCameraController::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(DM_BIND_EVENT_FUNC(PerspectiveCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DM_BIND_EVENT_FUNC(PerspectiveCameraController::OnWindowResized));
	}

	void PerspectiveCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(m_Fov, m_AspectRatio, m_CameraNear, m_CameraFar);
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (m_IsFirstMouse)
		{
			m_LastPosX = e.GetX();
			m_LastPosY = e.GetY();
			m_IsFirstMouse = false;
		}
		float xOffset = (m_LastPosX - e.GetX()) * m_MouseSensitivity;
		float yOffset = (e.GetY() - m_LastPosY) * m_MouseSensitivity;

		m_LastPosX = e.GetX();
		m_LastPosY = e.GetY();

		float yaw = m_Camera.GetYaw() + xOffset;
		float pitch = m_Camera.GetPitch() + yOffset;

		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;

		m_Camera.SetYawAndPitch(yaw, pitch);
		return false;
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_Fov -= e.GetYOffset();
		if (m_Fov < 1.0f) m_Fov = 1.0f;
		if (m_Fov > 90.0f) m_Fov = 90.0f;
		m_Camera.SetProjection(m_AspectRatio, m_Fov, m_CameraNear, m_CameraFar);
		return false;
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}