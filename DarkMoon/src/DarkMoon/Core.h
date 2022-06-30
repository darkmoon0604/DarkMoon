#pragma once

#ifdef DM_PLATFORM_WINDOWS
	#ifdef DM_BUILD_DLL
		#define DARKMOON_API __declspec(dllexport)
	#else
		#define DARKMOON_API __declspec(dllimport)
	#endif
#elif __APPLE__
	#error Not support macos
#else
	#error Unkown Platfomr
#endif

#define BIT(x) (1 << x)
