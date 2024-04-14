#include "Backend/OpenGL/OpenGLFramebuffer.h"

#include <glad/glad.h>
#include <string>

namespace Harmony {

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferInfo& framebufferInfo) {
		glGenFramebuffers(1, &m_GPUID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_GPUID);

		glGenTextures(1, &m_ColorTargetID);
		glGenRenderbuffers(1, &m_DepthTargetID);
		InitBufferTargets(framebufferInfo.Width, framebufferInfo.Height);

		int result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (result != GL_FRAMEBUFFER_COMPLETE) {
			throw std::runtime_error(std::string("Framebuffer failed, error code ") + std::to_string(result));
		}
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteTextures(1, &m_ColorTargetID);
		glDeleteRenderbuffers(1, &m_DepthTargetID);
		glDeleteFramebuffers(1, &m_GPUID);
	}

	void OpenGLFramebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_GPUID);
	}

	void OpenGLFramebuffer::Unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void OpenGLFramebuffer::Resize(int width, int height) {
		InitBufferTargets(width, height);
	}

	void OpenGLFramebuffer::BindTexture() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ColorTargetID);
	}

	void OpenGLFramebuffer::InitBufferTargets(int width, int height) {
		glBindTexture(GL_TEXTURE_2D, m_ColorTargetID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			m_ColorTargetID, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, m_DepthTargetID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, m_DepthTargetID);

		glBindTexture(GL_TEXTURE_2D, GL_NONE);
		glBindRenderbuffer(GL_RENDERBUFFER, GL_NONE);
	}
}