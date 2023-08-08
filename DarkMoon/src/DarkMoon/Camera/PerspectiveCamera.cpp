#include "pch.h"
#include "PerspectiveCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace DarkMoon
{
	PerspectiveCamera::PerspectiveCamera(float aspectRatio, float fov, float cameraNear, float cameraFar)
		: m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, cameraNear, cameraFar)),
		  m_Position({ 0.0f, 0.0f, 3.0f }),
		  m_WorldUp({ 0.0f, 1.0f, 0.0f }),
		  m_Front({ 0.0f, 0.0f, -1.0f }),
		  m_Yaw(-90.f),
		  m_Pitch(0.0f)
	{
		UpdateCameraVectors();
	}

	void PerspectiveCamera::SetProjection(float aspectRatio, float fov, float cameraNear, float cameraFar)
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, cameraNear, cameraFar);
	}

	void PerspectiveCamera::UpdateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);
		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));
	}
}
