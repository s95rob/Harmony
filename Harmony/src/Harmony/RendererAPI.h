#pragma once

#include "Harmony/Base.h"
#define interface

namespace Harmony {

	enum class CullingMode {
		None = 0,
		Front, Back
	};

	enum class DrawMode {
		Triangles = 0, Lines, Points
	};

	interface class RendererAPI {
	public:
		friend class RenderCommand;

		virtual ~RendererAPI() = default;

		virtual bool Init(const InitInfo& initInfo) = 0;
		virtual void Close() {}

		virtual void Clear() = 0;
		virtual void SetClearColor(float r, float g, float b, float a) = 0;
		virtual void SetViewport(float left, float top, float right, float bottom) = 0;
		virtual void SetFaceCulling(CullingMode mode) = 0;
		virtual void SetDepthTest(bool depthTest) = 0;

		virtual void Draw(DrawMode mode, unsigned int count, unsigned int startOffset) = 0;
		virtual void DrawInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset) = 0;
		virtual void DrawIndexed(DrawMode mode, unsigned int count, unsigned int startOffset) = 0;
		virtual void DrawIndexedInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset) = 0;
		
		virtual void Present() = 0;
		virtual void BindBackbuffer() = 0;

	private:
		static RendererAPI* Create(API api);
	};
}