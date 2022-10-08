#include "pch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace DarkMoon
{

	Shader::Shader(const std::string vertexSource, const std::string fragmentSource)
	{
		// vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const char* source = vertexSource.c_str();
		glShaderSource(vertexShader, 1, &source, nullptr);
		glCompileShader(vertexShader);
		GLint isComplied = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isComplied);
		if (GL_FALSE == isComplied)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> log(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &log[0]);
			glDeleteShader(vertexShader);
			DM_LOG_CORE_ERROR("Vertex shader error : {0}", log.data());
			DM_CORE_ASSERT(false, "Vertex shader compile failed!");
			return;
		}

		// fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		source = fragmentSource.c_str();
		glShaderSource(fragmentShader, 1, &source, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isComplied);
		if (GL_FALSE == isComplied)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> log(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &log[0]);
			glDeleteShader(fragmentShader);
			DM_LOG_CORE_ERROR("Fragment shader error : {0}", log.data());
			DM_CORE_ASSERT(false, "Fragment shader compile failed!");
			return;
		}

		m_ShaderId = glCreateProgram();
		glAttachShader(m_ShaderId, vertexShader);
		glAttachShader(m_ShaderId, fragmentShader);
		glLinkProgram(m_ShaderId);
		GLint isLinked = 0;
		glGetProgramiv(m_ShaderId, GL_LINK_STATUS, (int*)&isLinked);
		if (GL_FALSE == isLinked)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ShaderId, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> log(maxLength);
			glGetProgramInfoLog(m_ShaderId, maxLength, &maxLength, &log[0]);
			glDeleteProgram(m_ShaderId);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			DM_LOG_CORE_ERROR("Program link shader error : {0}", log.data());
			DM_CORE_ASSERT(false, "Program link shader failed!");
			return;
		}

		glDetachShader(m_ShaderId, vertexShader);
		glDetachShader(m_ShaderId, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderId);
		m_ShaderId = 0;
	}

	void Shader::Use() const
	{
		glUseProgram(m_ShaderId);
	}

	void Shader::UnUse() const
	{
		glUseProgram(0);
	}
}