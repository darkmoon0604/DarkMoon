#pragma once

#include "DarkMoon/Core/Core.h"
#include "DarkMoon/Events/Event.h"

namespace DarkMoon
{
	class DARKMOON_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImguiRender() {}

		virtual void OnEvent(Event& e)
		{

		}
		
		inline std::string& GetName()
		{
			return m_DebugName;
		}
	protected:
		std::string m_DebugName;
	};
}

