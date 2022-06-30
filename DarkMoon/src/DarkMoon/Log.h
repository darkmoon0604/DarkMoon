#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace DarkMoon {
	class DARKMOON_API Log
	{
	public:
		static void Init(const char* clientName = "APP");

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() 
		{
			return s_CoreLogger;
		}

		inline static std::shared_ptr<spdlog::logger>& GetClientLogger()
		{
			return s_ClientLogger;
		}
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}


#define DM_LOG_CORE_TRACE(...) ::DarkMoon::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DM_LOG_CORE_INFO(...) ::DarkMoon::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DM_LOG_CORE_WARN(...) ::DarkMoon::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DM_LOG_CORE_ERROR(...) ::DarkMoon::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DM_LOG_CORE_FINAL(...) ::DarkMoon::Log::GetCoreLogger()->final(__VA_ARGS__)

#define DM_LOG_TRACE(...) ::DarkMoon::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DM_LOG_INFO(...) ::DarkMoon::Log::GetClientLogger()->info(__VA_ARGS__)
#define DM_LOG_WARN(...) ::DarkMoon::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DM_LOG_ERROR(...) ::DarkMoon::Log::GetClientLogger()->error(__VA_ARGS__)
#define DM_LOG_FINAL(...) ::DarkMoon::Log::GetClientLogger()->final(__VA_ARGS__)