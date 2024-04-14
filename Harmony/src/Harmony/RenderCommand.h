#pragma once

#include "Harmony/RendererAPI.h"

namespace Harmony {

	class RenderCommand {
	public:
		static bool Init(InitInfo& initInfo);
		static void Close();

		static void Clear() { s_API->Clear(); }
		static void SetClearColor(float r, float g, float b, float a) { s_API->SetClearColor(r, g, b, a); }
		static void SetViewport(float left, float top, float right, float bottom) { s_API->SetViewport(left, top, right, bottom); }
		static void SetFaceCulling(CullingMode mode) { s_API->SetFaceCulling(mode); }
		static void SetDepthTest(bool depthTest) { s_API->SetDepthTest(depthTest); }

		// Draws the currently bound VertexBuffer(s)
		static void Draw(DrawMode mode, unsigned int count, unsigned int startOffset = 0) {
			s_API->Draw(mode, count, startOffset);
		}

		static void DrawInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset = 0) {
			s_API->DrawInstanced(mode, count, instances, startOffset);
		}

		// Draws the currently bound VertexBuffer(s) using an IndexBuffer
		static void DrawIndexed(DrawMode mode, unsigned int count, unsigned int startOffset = 0) { 
			s_API->DrawIndexed(mode, count, startOffset); 
		}

		static void DrawIndexedInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset = 0) { 
			s_API->DrawIndexedInstanced(mode, count, instances, startOffset);
		}

		static void Present() { s_API->Present(); }
		static void BindBackbuffer() { s_API->BindBackbuffer(); }

	private:
		static RendererAPI* s_API;
	};
}