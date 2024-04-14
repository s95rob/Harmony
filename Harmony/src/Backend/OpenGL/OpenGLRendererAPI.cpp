#include "Backend/OpenGL/OpenGLRendererAPI.h"
#include "Harmony/Base.h"

#include <glad/glad.h>

namespace Harmony {

	unsigned int OpenGLRendererAPI::s_ImmediateVertexArrayID = 0;

	int GLEnumFromCullingMode(CullingMode mode);
	int GLEnumFromDrawMode(DrawMode mode);

	bool OpenGLRendererAPI::Init(const InitInfo& initInfo) {
		if (!gladLoadGL()) {
			throw std::runtime_error("Failed to initialize OpenGL extensions");
			return false;
		}

		glEnable(GL_DEPTH_TEST); 
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glGenVertexArrays(1, &s_ImmediateVertexArrayID);
		glBindVertexArray(s_ImmediateVertexArrayID);

		return true;
	}

	void OpenGLRendererAPI::Close() {
		glBindVertexArray(GL_NONE);
		glDeleteVertexArrays(1, &s_ImmediateVertexArrayID);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::SetViewport(float left, float top, float right, float bottom) {
		glViewport(left, top, right, bottom);
	}

	void OpenGLRendererAPI::SetFaceCulling(CullingMode mode) {
		if (mode == CullingMode::None) {
			glDisable(GL_CULL_FACE);
			return;
		}

		glEnable(GL_CULL_FACE);
		glCullFace(GLEnumFromCullingMode(mode));
		glFrontFace(GL_CCW);
	}

	void OpenGLRendererAPI::SetDepthTest(bool depthTest) {
		depthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::Draw(DrawMode mode, unsigned int count, unsigned int startOffset) {
		glDrawArrays(GLEnumFromDrawMode(mode), startOffset, count);
	}

	void OpenGLRendererAPI::DrawInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset) {
		glDrawArraysInstanced(GLEnumFromDrawMode(mode), startOffset, count, instances);
	}


	void OpenGLRendererAPI::DrawIndexed(DrawMode mode, unsigned int count, unsigned int startOffset) {
		glDrawElements(GLEnumFromDrawMode(mode), count, GL_UNSIGNED_INT, (void*)(startOffset * sizeof(int)));
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset) {
		glDrawElementsInstanced(GLEnumFromDrawMode(mode), count, GL_UNSIGNED_INT, 0, instances);
	}

	void OpenGLRendererAPI::Present() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLRendererAPI::BindBackbuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	}

	void OpenGLRendererAPI::BindImmediateVertexArray() {
		glBindVertexArray(s_ImmediateVertexArrayID);
	}


	int GLEnumFromCullingMode(CullingMode mode) {
		switch (mode) {
		case CullingMode::Front: return GL_FRONT;
		case CullingMode::Back: return GL_BACK;
		}

		return GL_NONE;
	}

	int GLEnumFromDrawMode(DrawMode mode) {
		switch (mode) {
		case DrawMode::Triangles: return GL_TRIANGLES;
		case DrawMode::Lines: return GL_LINES;
		case DrawMode::Points: return GL_POINTS;
		}

		return GL_NONE;
	}
}