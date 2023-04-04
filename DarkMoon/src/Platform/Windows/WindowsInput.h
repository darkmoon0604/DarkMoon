#pragma once

#include "DarkMoon/Core/Input.h"

namespace DarkMoon
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IIsKeyPressed(int keyCode) override;

		virtual bool IIsMouseButtonPressed(int button) override;
		virtual float IGetMousePositionX() override;
		virtual float IGetMousePositionY() override;
		virtual std::pair<float, float> IGetMousePosition() override;
	};
}

