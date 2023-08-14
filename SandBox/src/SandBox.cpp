#include "DarkMoon.h"
#include "DarkMoon/Core/EntryPoint.h"

#include "SandBox2D.h"
#include "SandBox3D.h"
#include "ExampleLayer.h"

class SandBox : public DarkMoon::Application
{
public:
	SandBox()
	{
		GetWindow().IsShowCursor(true);
		//PushLayer(new ExampleLayer());
		PushLayer(new SandBox2D());
		//PushLayer(new SandBox3D());
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