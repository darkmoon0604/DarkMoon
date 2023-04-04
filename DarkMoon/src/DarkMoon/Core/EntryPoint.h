#pragma once

#ifdef DM_PLATFORM_WINDOWS
	extern DarkMoon::Application* DarkMoon::CreateApplication();
	
	int main(int argc, char** argv)
	{
		auto app = DarkMoon::CreateApplication();
		app->Run();
		delete app;
		return 0;
	}
#elif __APPLE__
	#error Not support macos
#else
	#error Unkown Platfomr
#endif