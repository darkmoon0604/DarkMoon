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

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};

