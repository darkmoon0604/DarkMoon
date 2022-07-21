#pragma once

#include "DarkMoon/Layer.h"
#include "DarkMoon/Events/Event.h"

namespace DarkMoon
{
	class DARKMOON_API ImguiLayer : public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& e);
	private:
		float m_Time = 0.0f;
	};
}


