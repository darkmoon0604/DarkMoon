#pragma once
#include "DarkMoon.h"

class SandBox2D : public DarkMoon::Layer
{
public: 
	SandBox2D();
	virtual ~SandBox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(DarkMoon::TimeStep timeStep) override;
	virtual void OnImguiRender() override;
	void OnEvent(DarkMoon::Event& e) override;
private:
	DarkMoon::OrthographicCameraController m_CameraController;

	DarkMoon::Ref<DarkMoon::VertexArray> m_SquareVA;
	DarkMoon::Ref<DarkMoon::Shader> m_FlatColorShader;
	DarkMoon::Ref<DarkMoon::Texture2D> m_CheckerboardTexture;

	glm::vec4 m_SquareColor = { 0.9f, 0.3f, 0.8f, 1.0f };
	glm::vec4 m_BackgroundColor = { 0.1f, 0.1f, 0.1f, 1.0f };
	float m_SizeX = 0.2f, m_SizeY = 0.1f;
	float m_PosX = -1.0f, m_PosY = 0.5f;
};

