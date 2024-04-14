#include "Backend/OpenGL/OpenGLTexture.h"

#include <glad/glad.h>

namespace Harmony {

	int GLEnumFromTextureFilter(TextureFilter filter);
	int GLEnumFromTextureWrap(TextureWrap wrap);
	int GLEnumFromChannels(int channels);

	OpenGLTexture::OpenGLTexture(const TextureInfo& textureInfo) {
		glGenTextures(1, &m_GPUID);
		glBindTexture(GL_TEXTURE_2D, m_GPUID);

		int wrapEnum = GLEnumFromTextureWrap(textureInfo.Wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapEnum);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapEnum);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLEnumFromTextureFilter(textureInfo.FarFilter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLEnumFromTextureFilter(textureInfo.NearFilter));

		int channelsEnum = GLEnumFromChannels(textureInfo.Channels);
		glTexImage2D(GL_TEXTURE_2D, 0, channelsEnum,
			textureInfo.Width, textureInfo.Height, 0, channelsEnum,
			GL_UNSIGNED_BYTE, textureInfo.Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, GL_NONE);

		m_Width = textureInfo.Width;
		m_Height = textureInfo.Height;
		m_ID = s_CurrentID++;
	}

	OpenGLTexture::~OpenGLTexture() {
		glDeleteTextures(1, &m_GPUID);
	}

	void OpenGLTexture::Bind(unsigned int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_GPUID);
		m_Slot = slot;
	}

	void OpenGLTexture::Unbind() {
		glActiveTexture(GL_TEXTURE0 + m_Slot);
		glBindTexture(GL_TEXTURE_2D, GL_NONE);
	}

	int GLEnumFromTextureFilter(TextureFilter filter) {
		switch (filter) {
		case TextureFilter::Nearest: return GL_NEAREST;
		case TextureFilter::Linear: return GL_LINEAR;
		}

		return GL_NONE;
	}

	int GLEnumFromTextureWrap(TextureWrap wrap) {
		switch (wrap) {
		case TextureWrap::Repeat: return GL_REPEAT;
		case TextureWrap::Clamp: return GL_CLAMP_TO_EDGE;
		}

		return GL_NONE;
	}

	int GLEnumFromChannels(int channels) {
		switch (channels) {
		case 1: return GL_R;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		}

		return GL_NONE;
	}
}