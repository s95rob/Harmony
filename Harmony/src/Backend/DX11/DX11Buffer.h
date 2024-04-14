#pragma once

#include "Harmony/Buffer.h"

#include "Backend/DX11/DX11Include.h"

namespace Harmony {

	class DX11Buffer : public BindableBuffer {
	public:
		virtual ~DX11Buffer();

	protected:
		ID3D11Buffer* m_Buffer = nullptr;
	};

	class DX11VertexBuffer : public VertexBuffer, public DX11Buffer {
	public:
		DX11VertexBuffer(const VertexBufferInfo& vertexBufferInfo);

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetData(float* data, size_t size, size_t startOffset);

	protected:
		ID3D11InputLayout* m_InputLayout = nullptr;
	};

	class DX11IndexBuffer : public IndexBuffer, DX11Buffer {
	public:
		DX11IndexBuffer(const IndexBufferInfo& indexBufferInfo);

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetData(uint32_t* data, size_t size, size_t startOffset);
	};

	class DX11ConstantBuffer : public ConstantBuffer, public DX11Buffer {
	public:
		DX11ConstantBuffer(void* data, size_t size);

		virtual void Bind(unsigned int slot) override;
		virtual void Unbind() override;

		virtual void SetData(void* data, size_t size, size_t offset) override;

	};

}