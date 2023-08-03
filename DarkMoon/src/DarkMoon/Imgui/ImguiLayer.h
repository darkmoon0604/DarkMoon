#pragma once

#include "DarkMoon/Core/Layer.h"
#include "DarkMoon/Events/ApplicationEvent.h"
#include "DarkMoon/Events/KeyEvent.h"
#include "DarkMoon/Events/MouseEvent.h"

namespace DarkMoon
{
	class DARKMOON_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnBegin();
		void OnEnd();
	private:
		float m_Time = 0.0f;
	};
}


