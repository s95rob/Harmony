#include "Backend/DX11/DX11Buffer.h"
#include "Backend/DX11/DX11RendererAPI.h"
#include "Backend/DX11/DX11Util.h"
#include "Backend/DX11/DX11Shader.h"

#include <string>

namespace Harmony {

	DX11Buffer::~DX11Buffer() {
		m_Buffer->Release();
	}

	DX11VertexBuffer::DX11VertexBuffer(const VertexBufferInfo& vertexBufferInfo) {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // D3D11UsageFromBufferUsage(vertexBufferInfo.Usage);
		bufferDesc.ByteWidth = vertexBufferInfo.Size;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = vertexBufferInfo.Data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		auto* device = DX11RendererAPI::GetDevice();
		HRESULT result = device->CreateBuffer(&bufferDesc, &bufferData, &m_Buffer);

		SetData(vertexBufferInfo.Data, vertexBufferInfo.Size, 0);
	}

	void DX11VertexBuffer::Bind() {
		DX11Shader* shader = DX11RendererAPI::GetActiveShader();
		if (!shader)
			return;

		auto vs = shader->GetVertexShaderData();
		auto ps = shader->GetPixelShaderData();
		auto vsi = shader->GetVertexShaderDataSize();
		auto psi = shader->GetPixelShaderDataSize();

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements(m_Layout.GetAttribCount());
		std::string semanticName;
		for (int i = 0; i < inputElements.size(); i++) {
			semanticName = "VERTEX_ATTRIB";
			inputElements[i] = {
				semanticName.c_str(),
				(UINT)i,
				DXGIFormatFromAttrib(m_Layout[i]),
				0,
				(UINT)m_Layout[i].GetOffset(),
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			};
		}

		auto* device = DX11RendererAPI::GetDevice();
		HRESULT result = device->CreateInputLayout(inputElements.data(), inputElements.size(),
			shader->GetVertexShaderData(), shader->GetVertexShaderDataSize(),
			&m_InputLayout);

		UINT stride = m_Layout.GetStride();
		UINT offset = 0;
		auto* context = DX11RendererAPI::GetDeviceContext();
		context->IASetInputLayout(m_InputLayout);
		context->IASetVertexBuffers(0, 1, &m_Buffer, &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}

	void DX11VertexBuffer::Unbind() {
		m_InputLayout->Release();
	}

	void DX11VertexBuffer::SetData(float* data, size_t size, size_t offset) {
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		auto* context = DX11RendererAPI::GetDeviceContext();

		HRESULT result = context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		std::memcpy((uint8_t*)mappedResource.pData + offset, data, size);

		context->Unmap(m_Buffer, 0);
	}

	DX11IndexBuffer::DX11IndexBuffer(const IndexBufferInfo& indexBufferInfo) {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // D3D11UsageFromBufferUsage(indexBufferInfo.Usage);
		bufferDesc.ByteWidth = indexBufferInfo.Count * sizeof(uint32_t);
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = indexBufferInfo.Data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		auto* device = DX11RendererAPI::GetDevice();
		HRESULT result = device->CreateBuffer(&bufferDesc, &bufferData, &m_Buffer);

		SetData(indexBufferInfo.Data, indexBufferInfo.Count * sizeof(uint32_t), 0);

		m_Count = indexBufferInfo.Count;
	}

	void DX11IndexBuffer::Bind() {
		auto* context = DX11RendererAPI::GetDeviceContext();
		context->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DX11IndexBuffer::Unbind() {
		auto* context = DX11RendererAPI::GetDeviceContext();
		context->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
	}

	void DX11IndexBuffer::SetData(uint32_t* data, size_t size, size_t offset) {
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		auto* context = DX11RendererAPI::GetDeviceContext();

		context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		std::memcpy((uint8_t*)mappedResource.pData + offset, data, size);

		context->Unmap(m_Buffer, 0);
	}

	DX11ConstantBuffer::DX11ConstantBuffer(void* data, size_t size) {
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = size;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA bufferData = {};
		bufferData.pSysMem = data;
		bufferData.SysMemPitch = 0;
		bufferData.SysMemSlicePitch = 0;

		auto* device = DX11RendererAPI::GetDevice();
		HRESULT result = device->CreateBuffer(&bufferDesc, nullptr, &m_Buffer);

		SetData(data, size, 0);
	}

	void DX11ConstantBuffer::Bind(unsigned int slot) {
		auto* context = DX11RendererAPI::GetDeviceContext();
		context->VSSetConstantBuffers(slot, 1, &m_Buffer);
		m_Slot = slot;
	}

	void DX11ConstantBuffer::Unbind() {
		ID3D11Buffer* nullBuffer = nullptr;
		auto* context = DX11RendererAPI::GetDeviceContext();
		context->VSSetConstantBuffers(m_Slot, 1, &nullBuffer);
	}

	void DX11ConstantBuffer::SetData(void* data, size_t size, size_t offset) {
		D3D11_MAPPED_SUBRESOURCE mappedResource = {};
		auto* context = DX11RendererAPI::GetDeviceContext();

		context->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		std::memcpy((uint8_t*)mappedResource.pData + offset, data, size);

		context->Unmap(m_Buffer, 0);
	}
}