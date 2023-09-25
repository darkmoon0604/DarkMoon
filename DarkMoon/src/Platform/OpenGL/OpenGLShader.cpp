#include "pch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace DarkMoon
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		
		if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		DM_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		DM_PROFILE_FUNCTION();
		std::string source = ReadFile(filePath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		auto lastSlash = filePath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filePath.rfind('.');
		auto count = lastDot == std::string::npos ? filePath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filePath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) : m_Name(name)
	{
		DM_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		DM_PROFILE_FUNCTION();
		glDeleteProgram(m_RendererID);
		glDeleteShader(m_RendererID);
	}

	void OpenGLShader::Use() const
	{
		DM_PROFILE_FUNCTION();
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::UnUse() const
	{
		DM_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		DM_PROFILE_FUNCTION();
		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		DM_PROFILE_FUNCTION();
		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		DM_PROFILE_FUNCTION();
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		DM_PROFILE_FUNCTION();
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		DM_PROFILE_FUNCTION();
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	std::string OpenGLShader::ReadFile(const std::string& filePath)
	{
		DM_PROFILE_FUNCTION();
		std::string result;
		std::ifstream in(filePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
			else
			{
				DM_LOG_CORE_ERROR("Could not open file '{0}'", filePath);
			}
		}
		else
		{
			DM_LOG_CORE_ERROR("Could not open file '{0}'", filePath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		DM_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		GLenum shaderType;
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			DM_CORE_ASSERT(eol != std::string::npos, "Syntax error.");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			shaderType = ShaderTypeFromString(type);
			DM_CORE_ASSERT(shaderType, "Invalid shader type specified.");
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[shaderType] = source.substr(nextLinePos, pos - (pos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		DM_PROFILE_FUNCTION();
		GLint program = glCreateProgram();
		DM_CORE_ASSERT(shaderSources.size() <= 2, "We onlay support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto &shaderInfo : shaderSources)
		{
			GLenum type = shaderInfo.first;
			const std::string& source = shaderInfo.second;

			GLuint shader = glCreateShader(type);
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shader);

				DM_LOG_CORE_ERROR("{0}", infoLog.data());
				DM_CORE_ASSERT(false, "shder compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_RendererID = program;

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(program);
			
			for (auto id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			DM_LOG_CORE_ERROR("{0}", infoLog.data());
			DM_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
		}
	}
}