#pragma once

#include "DarkMoon/Core/Core.h"
#include "DarkMoon/Core/KeyCodes.h"
#include "DarkMoon/Core/MouseCodes.h"

namespace DarkMoon
{
	class DARKMOON_API Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(KeyCode keyCode)
		{
			return s_Instance->IIsKeyPressed(keyCode);
		}

		inline static bool IsMouseButtonPressed(MouseCode button)
		{
			return s_Instance->IIsMouseButtonPressed(button);
		}

		inline static float GetMousePositionX()
		{
			return s_Instance->IGetMousePositionX();
		}

		inline static float GetMousePositionY()
		{
			return s_Instance->IGetMousePositionY();
		}

		inline static std::pair<float, float> GetMousePosition()
		{
			return s_Instance->IGetMousePosition();
		}
	protected:
		/// <summary>
		/// 需要子类实现的接口
		/// </summary>
		/// <param name="keyCode"></param>
		/// <returns></returns>
		virtual bool IIsKeyPressed(KeyCode keyCode) = 0;

		virtual bool IIsMouseButtonPressed(MouseCode button) = 0;
		virtual float IGetMousePositionX() = 0;
		virtual float IGetMousePositionY() = 0;
		virtual std::pair<float, float> IGetMousePosition() = 0;
	private:
		static Scope<Input> s_Instance;
	};
}