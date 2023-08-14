#pragma once

#ifdef DM_PLATFORM_WINDOWS
	extern DarkMoon::Application* DarkMoon::CreateApplication();
	
	int main(int argc, char** argv)
	{
		DM_PROFILE_BEGIN_SESSION("Startup", "DM_Profile_Startup.json");
		auto app = DarkMoon::CreateApplication();
		DM_PROFILE_END_SESSION();

		DM_PROFILE_BEGIN_SESSION("Runtime", "DM_Profile_Runtime.json");
		app->Run();
		DM_PROFILE_END_SESSION();

		DM_PROFILE_BEGIN_SESSION("Shutdown", "DM_Profile_Shutdown.json");
		delete app;
		DM_PROFILE_END_SESSION();
		
		return 0;
	}
#elif __APPLE__
	#error Not support macos
#else
	#error Unkown Platfomr
#endif