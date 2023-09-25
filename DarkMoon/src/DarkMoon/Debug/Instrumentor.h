#pragma once

#include "DarkMoon/Core/Core.h"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>
#include <thread>
#include <iomanip>

namespace DarkMoon
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string m_Name;
		FloatingPointMicroseconds m_Start;
		std::chrono::microseconds m_ElapsedTime;
		std::thread::id m_ThreadId;
	};

	struct InstrumentationSession
	{
		std::string m_Name;
	};

	class DARKMOON_API Instrumentor
	{
	private:
		std::mutex m_Mutex; //线程锁
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	public:
		Instrumentor()
			: m_CurrentSession(nullptr)
		{

		}

		void BeginSession(const std::string& name, const std::string& filePath = "debug.json")
		{
			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				if (Log::GetCoreLogger())
				{
					DM_LOG_CORE_ERROR("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->m_Name);
					InternalEndSession();
				}
			}
			m_OutputStream.open(filePath);
			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession{ name };
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger())
				{
					DM_LOG_CORE_ERROR("Instrumentor could not open results file '{0}.'", filePath);
				}
			}
		}

		void EndSession()
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			std::string name = result.m_Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			json << std::setprecision(3) << std::fixed; //格式化输出为小数点3位小数
			json << ", {";
			json << "\"cat\":\"function\", ";
			json << "\"dur\":" << (result.m_ElapsedTime.count()) << ", ";
			json << "\"name\":\"" << name << "\", ";
			json << "\"ph\":\"x\", ";
			json << "\"pid\":0, ";
			json << "\"tid\":" << result.m_ThreadId << ", ";
			json << "\"ts\":" << result.m_Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\":{},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
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
			m_StartTimePoint = std::chrono::steady_clock::now();
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
			auto endTimePoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimePoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch();
			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;
		bool m_Stopped;
	};
}

#define DM_PROFILE TRUE
#if DM_PROFILE

#if defined(__GNUC__) || defined(__MWERKS__) && (__MWERKS__ >= 0x30000) || defined(__ICC) && (__ICC >= 600) || defined(__ghs__)
#define DM_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define DM_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#define DM_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define DM_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define DM_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define DM_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define DM_FUNC_SIG __func__
#else
#define DM_FUNC_SIG "DM_FUNC_SIG unknown!"
#endif

#define DM_PROFILE_BEGIN_SESSION(name, filePath) ::DarkMoon::Instrumentor::Get().BeginSession(name, filePath)
#define DM_PROFILE_END_SESSION() ::DarkMoon::Instrumentor::Get().EndSession()
#define DM_PROFILE_SCOPE(name) ::DarkMoon::InstrumentationTimer timer##__LINE__(name);
#define DM_PROFILE_FUNCTION() DM_PROFILE_SCOPE(DM_FUNC_SIG)
#else
#define DM_PROFILE_BEGIN_SESSION(name, filePath)
#define DM_PROFILE_END_SESSION()
#define DM_PROFILE_SCOPE(name)
#define DM_PROFILE_FUNCTION()
#endif