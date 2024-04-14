#include "Harmony/Buffer.h"
#include "Harmony/RendererInfo.h"

#include "Backend/OpenGL/OpenGLBuffer.h"
#include "Backend/DX11/DX11Buffer.h"

namespace Harmony {

	Ref<VertexBuffer> VertexBuffer::Create(const VertexBufferInfo& vertexBufferInfo) {
		switch (RendererInfo::GetAPI()) {
		case API::OpenGL: return MakeRef<OpenGLVertexBuffer>(vertexBufferInfo);
		case API::DX11: return MakeRef<DX11VertexBuffer>(vertexBufferInfo);
		}

		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const IndexBufferInfo& indexBufferInfo) {
		switch (RendererInfo::GetAPI()) {
		case API::OpenGL: return MakeRef<OpenGLIndexBuffer>(indexBufferInfo);
		case API::DX11: return MakeRef<DX11IndexBuffer>(indexBufferInfo);
		}

		return nullptr;
	}

	Ref<UniformBuffer> UniformBuffer::Create(void* data, size_t size) {
		switch (RendererInfo::GetAPI()) {
		case API::OpenGL: return MakeRef<OpenGLUniformBuffer>(data, size);
		case API::DX11: return MakeRef<DX11ConstantBuffer>(data, size);
		}

		return nullptr;
	}
}