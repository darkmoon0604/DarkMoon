#pragma once
#include "DarkMoon.h"

class ExampleLayer : public DarkMoon::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(DarkMoon::TimeStep timeStep) override;
	virtual void OnImguiRender() override;
	virtual void OnEvent(DarkMoon::Event& e) override;
private:
	DarkMoon::ShaderLibrary m_ShaderLibrary;
	DarkMoon::Ref<DarkMoon::Shader> m_Shader;
	DarkMoon::Ref<DarkMoon::VertexArray> m_VertexArray;
	
	DarkMoon::Ref<DarkMoon::Shader> m_FlatColorShader;
	DarkMoon::Ref<DarkMoon::VertexArray> m_SquareVertexArray;

	DarkMoon::Ref<DarkMoon::Texture2D> m_Texture, m_LogoTexture;

	DarkMoon::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

