#pragma once

#include "DarkMoon/Core/Core.h"

namespace DarkMoon
{
	class DARKMOON_API Input
	{
	protected:
		Input() = default;
	public:
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		inline static bool IsKeyPressed(int keyCode)
		{
			return s_Instance->IIsKeyPressed(keyCode);
		}

		inline static bool IsMouseButtonPressed(int button)
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
		virtual bool IIsKeyPressed(int keyCode) = 0;

		virtual bool IIsMouseButtonPressed(int button) = 0;
		virtual float IGetMousePositionX() = 0;
		virtual float IGetMousePositionY() = 0;
		virtual std::pair<float, float> IGetMousePosition() = 0;
	private:
		static Input* s_Instance;
	};
}