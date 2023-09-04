#include "pch.h"
#include "DarkMoon/Core/Window.h"

#ifdef DM_PLATFORM_WINDOWS
#include "Platform/Windows/WinWindow.h"
#endif

namespace DarkMoon
{
	Scope<Window> Window::Create(const WindowProperty& props /* = WindowProperty() */)
	{
#ifdef DM_PLATFORM_WINDOWS
		return CreateScope<WinWindow>(props);
#else
		DM_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}
}