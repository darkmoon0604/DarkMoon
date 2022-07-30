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

#ifdef DM_ENABLE_ASSERTS
	#define DM_ASSERT(x, ...) { if (!(x)) { DM_LOG_ERROR("Assert failed : {0}", __VA_ARGS__); __debugbreak(); } }
	#define DM_CORE_ASSERT(x, ...) { if (!(x)) { DM_LOG_CORE_ERROR("Assert failed : {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define DM_ASSERT(x, ...)
	#define DM_CORE_ASSERT(x, ...)
#endif // DM_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define DM_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)
