#pragma once

#include "DarkMoon/Core/Core.h"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

namespace DarkMoon
{
	struct ProfileResult
	{
		std::string m_Name;
		long long m_Start, m_End;
		uint32_t m_ThreadId;
	};

	struct InstrumentationSession
	{
		std::string m_Name;
	};

	class DARKMOON_API Instrumentor
	{
	private:
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	public:
		Instrumentor()
			: m_CurrentSession(nullptr), m_ProfileCount(0)
		{

		}

		void BeginSession(const std::string& name, const std::string& filePath = "debug.json")
		{
			m_OutputStream.open(filePath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ name };
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (m_ProfileCount++ > 0)
			{
				m_OutputStream << ",";
			}

			std::string name = result.m_Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.m_End - result.m_Start) << ",";
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"x\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.m_ThreadId << ",";
			m_OutputStream << "\"ts\":" << result.m_Start;
			m_OutputStream << "}";
			m_OutputStream.flush();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\":{},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class DARKMOON_API InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimePoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
			{
				Stop();
			}
		}

		void Stop()
		{
			auto endTimePoint = std::chrono::high_resolution_clock::now();
			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
			uint32_t threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadId });
			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
		bool m_Stopped;
	};
}

#define DM_PROFILE TRUE
#if DM_PROFILE
#define DM_PROFILE_BEGIN_SESSION(name, filePath) ::DarkMoon::Instrumentor::Get().BeginSession(name, filePath)
#define DM_PROFILE_END_SESSION() ::DarkMoon::Instrumentor::Get().EndSession()
#define DM_PROFILE_SCOPE(name) ::DarkMoon::InstrumentationTimer timer##__LINE__(name);
#define DM_PROFILE_FUNCTION() DM_PROFILE_SCOPE(__FUNCSIG__)
#else
#define DM_PROFILE_BEGIN_SESSION(name, filePath)
#define DM_PROFILE_END_SESSION()
#define DM_PROFILE_SCOPE(name)
#define DM_PROFILE_FUNCTION()
#endif