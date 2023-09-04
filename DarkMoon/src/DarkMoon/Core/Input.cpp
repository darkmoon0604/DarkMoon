#include "pch.h"
#include "DarkMoon/Core/Input.h"

#ifdef DM_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif

namespace DarkMoon
{
	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create()
	{
#ifdef DM_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
#else
		DM_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}