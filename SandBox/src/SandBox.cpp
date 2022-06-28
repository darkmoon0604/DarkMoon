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
	return new SandBox();
}