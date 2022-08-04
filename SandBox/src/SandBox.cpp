#include "DarkMoon.h"

class ExampleLayer : public DarkMoon::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer")
	{
		
	}

	void OnUpdate() override
	{
		DM_LOG_INFO("{0} update", m_DebugName);
	}

	void OnEvent(DarkMoon::Event& e) override
	{
		DM_LOG_INFO("{0}", e);
	}
};

class SandBox : public DarkMoon::Application
{
public:
	SandBox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new DarkMoon::ImguiLayer());
	}
	~SandBox()
	{

	}

private:

};

DarkMoon::Application* DarkMoon::CreateApplication()
{
	DarkMoon::Log::Init("SANBOX");
	DM_LOG_WARN("init");

	return new SandBox();
}