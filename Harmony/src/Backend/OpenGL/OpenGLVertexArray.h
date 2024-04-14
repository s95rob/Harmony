#pragma once

#include "Harmony/VertexArray.h"
#include "Backend/OpenGL/OpenGLBuffer.h"

namespace Harmony {

	class OpenGLVertexArray final : public VertexArray, public OpenGLBuffer {
	public:
		using VertexArray::VertexArray;

		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

	};
}