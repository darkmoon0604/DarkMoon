#pragma once

#include <memory>

#ifdef DM_PLATFORM_WINDOWS
	#if DYNAMIC_LINK
		#ifdef DM_BUILD_DLL
			#define DARKMOON_API __declspec(dllexport)
		#else
			#define DARKMOON_API __declspec(dllimport)
		#endif
	#else
		#define DARKMOON_API
	#endif
#elif __APPLE__
	#error Not support macos
#else
	#error Unkown Platform
#endif

#ifdef DM_ENABLE_ASSERTS
	#define DM_ASSERT(x, ...) { if (!(x)) { DM_LOG_ERROR("Assert failed : {0}", __VA_ARGS__); __debugbreak(); } }
	#define DM_CORE_ASSERT(x, ...) { if (!(x)) { DM_LOG_CORE_ERROR("Assert failed : {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define DM_ASSERT(x, ...)
	#define DM_CORE_ASSERT(x, ...)
#endif // DM_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define DM_BIND_EVENT_FUNC(func)\
	[this](auto&&... args) -> decltype(auto) \
	{\
		return this->func(std::forward<decltype(args)>(args)...);\
	}

namespace DarkMoon
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}
