#pragma once

#include "DarkMoon/Layer.h"
#include "DarkMoon/Events/ApplicationEvent.h"
#include "DarkMoon/Events/KeyEvent.h"
#include "DarkMoon/Events/MouseEvent.h"

namespace DarkMoon
{
	class DARKMOON_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImguiRender() override;

		void OnBegin();
		void OnEnd();
	private:
		float m_Time = 0.0f;
	};
}


