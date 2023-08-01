#include "pch.h"
#include "OrthographicCameraController.h"

#include "DarkMoon/Core/Input.h"
#include "DarkMoon/Core/KeyCodes.h"

namespace DarkMoon
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation /*= false*/)
		: m_AspectRatio(aspectRatio), 
		  m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		  m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(TimeStep timeStep)
	{
		if (DarkMoon::Input::IsKeyPressed(DM_KEY_A))
		{
			//m_CameraPosition.x -= m_CameraTranslationSpeed * timeStep;
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timeStep;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timeStep;
		}
		else if (DarkMoon::Input::IsKeyPressed(DM_KEY_D))
		{
			//m_CameraPosition.x += m_CameraTranslationSpeed * timeStep;
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timeStep;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timeStep;
		}

		if (DarkMoon::Input::IsKeyPressed(DM_KEY_W))
		{
			//m_CameraPosition.y += m_CameraTranslationSpeed * timeStep;
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timeStep;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timeStep;
		}
		else if (DarkMoon::Input::IsKeyPressed(DM_KEY_S))
		{
			//m_CameraPosition.y -= m_CameraTranslationSpeed * timeStep;
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timeStep;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timeStep;
		}

		if (m_Rotation)
		{
			if (DarkMoon::Input::IsKeyPressed(DM_KEY_Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * timeStep;
			}
			else if (DarkMoon::Input::IsKeyPressed(DM_KEY_E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * timeStep;
			}

			if (m_CameraRotation > 180.0f)
			{
				m_CameraRotation -= 360.0f;
			}
			else if (m_CameraRotation <= -180.0f)
			{
				m_CameraRotation += 360.0f;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}
		
		m_Camera.SetPosition(m_CameraPosition);
		
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(DM_BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DM_BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}