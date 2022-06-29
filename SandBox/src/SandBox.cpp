#include "DarkMoon.h"

class SandBox : public DarkMoon::Application
{
public:
	SandBox()
	{
	
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