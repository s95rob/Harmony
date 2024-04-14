#pragma once

#include "Harmony/RendererAPI.h"

namespace Harmony {

	class OpenGLRendererAPI final : public RendererAPI {
	public:
		virtual bool Init(const InitInfo& initInfo) override;
		virtual void Close() override;

		virtual void Clear() override;
		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void SetViewport(float left, float top, float right, float bottom) override;
		virtual void SetFaceCulling(CullingMode mode) override;
		virtual void SetDepthTest(bool depthTest) override;

		virtual void Draw(DrawMode mode, unsigned int count, unsigned int startOffset);
		virtual void DrawInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset);
		virtual void DrawIndexed(DrawMode mode, unsigned int count, unsigned int startOffset);
		virtual void DrawIndexedInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset);

		virtual void Present() override;
		virtual void BindBackbuffer() override;

		static void BindImmediateVertexArray();

	private:
		static unsigned int s_ImmediateVertexArrayID;
	};
}