#pragma once

#include "DarkMoon/Core/Core.h"

namespace DarkMoon
{
	class DARKMOON_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};
}