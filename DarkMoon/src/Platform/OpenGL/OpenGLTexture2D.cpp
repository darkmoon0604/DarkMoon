#include "pch.h"

#include "OpenGLTexture2D.h"
#include "stb_image.h"

#include <glad/glad.h>

//#define _TexOld

namespace DarkMoon
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		DM_CORE_ASSERT(data, "Failed to load image" + path);
		m_Width = width;
		m_Height = height;

#ifdef _TexOld
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		glTextureStorage2D(m_ID, 1, GL_RGB8, m_Width, m_Height);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGB8, GL_UNSIGNED_BYTE, data);
#else
		glGenTextures(1, &m_ID);
		Bind(0);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
#endif

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
#ifndef _TexOld
		glBindTexture(GL_TEXTURE_2D, 0);
#endif
		glDeleteTextures(1, &m_ID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot /* = 0 */) const
	{
#ifdef _TexOld
		glBindTextureUnit(slot, m_ID);
#else
		glBindTexture(GL_TEXTURE_2D, m_ID);
#endif
	}
}