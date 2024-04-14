#pragma once

#include "Harmony/Buffer.h"
#include "Backend/OpenGL/OpenGLRendererAPI.h"

namespace Harmony {
	
	interface class OpenGLBuffer : public BindableBuffer {
	public:
		virtual ~OpenGLBuffer() = default;

		virtual void Release() override;

		unsigned int GetGPUID() const { return m_GPUID; }

		operator unsigned int() const { return m_GPUID; }

	protected:
		unsigned int m_GPUID = 0;
	};

	class OpenGLVertexBuffer final : public VertexBuffer, public OpenGLBuffer {
	public:
		OpenGLVertexBuffer(const VertexBufferInfo& vertexBufferInfo);
		~OpenGLVertexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetData(float* data, size_t size, size_t startOffset) override;
	};

	class OpenGLIndexBuffer final : public IndexBuffer, public OpenGLBuffer {
	public:
		OpenGLIndexBuffer(const IndexBufferInfo& indexBufferInfo);
		~OpenGLIndexBuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetData(uint32_t* data, size_t size, size_t startOffset);
	};

	class OpenGLUniformBuffer final : public UniformBuffer, public OpenGLBuffer {
	public:
		OpenGLUniformBuffer(void* data, size_t size);
		~OpenGLUniformBuffer();

		virtual void Bind(unsigned int slot) override;
		virtual void Unbind() override;

		void SetData(void* data, size_t size, size_t offset = 0);

	private:
		size_t m_DataSize = 0;
	};
}