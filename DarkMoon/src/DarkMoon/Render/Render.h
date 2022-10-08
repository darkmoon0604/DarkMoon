#pragma once
#include "DarkMoon/Core.h"

#include <string>

namespace DarkMoon
{
	enum class RenderAPI
	{
		None = 0,
		OpenGL = 1,
		DirectX = 2,
	};

	class DARKMOON_API Render
	{
	public:
		inline static RenderAPI GetRenderAPI()
		{
			return s_CurrentRenderAPI;
		}
	private:
		static RenderAPI s_CurrentRenderAPI;
	};
}

