#include "pch.h"

#include "Log.h"

namespace DarkMoon
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init(const char* clientName /* = "APP" */)
	{
		spdlog::set_pattern("%^[%T] %n : %v%$");

		s_CoreLogger = spdlog::stdout_color_mt("DARKMOON");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt(clientName);
		s_ClientLogger->set_level(spdlog::level::trace);
	}
}