#include "Backend/DX11/DX11Texture.h"

#include "Backend/DX11/DX11RendererAPI.h"

namespace Harmony {

	DX11Texture::DX11Texture(const TextureInfo& textureInfo) {
		HRESULT result = S_OK;

		// Create the texture
		DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
		switch (textureInfo.Channels) {
		case 1: format = DXGI_FORMAT_R8_UNORM; break;
		case 2: format = DXGI_FORMAT_R8G8_UNORM; break;
		case 3: format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
		case 4: format = DXGI_FORMAT_R8G8B8A8_UNORM; break;
		}

		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = textureInfo.Width;
		textureDesc.Height = textureInfo.Height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = format;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		D3D11_SUBRESOURCE_DATA imageData = {};
		imageData.pSysMem = textureInfo.Data;
		imageData.SysMemPitch = textureInfo.Width * textureInfo.Channels;

		auto* device = DX11RendererAPI::GetDevice();
		result = device->CreateTexture2D(&textureDesc, &imageData, &m_Texture);

		// Create the shader resource view
		result = device->CreateShaderResourceView(m_Texture, nullptr, &m_ShaderResource);

		// Create the sampler
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 1.0f;
		samplerDesc.BorderColor[1] = 1.0f;
		samplerDesc.BorderColor[2] = 1.0f;
		samplerDesc.BorderColor[3] = 1.0f;
		samplerDesc.MinLOD = -FLT_MAX;
		samplerDesc.MaxLOD = FLT_MAX;

		result = device->CreateSamplerState(&samplerDesc, &m_Sampler);
	}

	DX11Texture::~DX11Texture() {
		m_ShaderResource->Release();
		m_Sampler->Release();
		m_Texture->Release();
	}

	void DX11Texture::Bind(unsigned int slot) {
		auto* context = DX11RendererAPI::GetDeviceContext();
		context->PSSetShaderResources(0, 1, &m_ShaderResource);
		context->PSSetSamplers(0, 1, &m_Sampler);
	}

	void DX11Texture::Unbind() {
		auto* context = DX11RendererAPI::GetDeviceContext();
		
		ID3D11ShaderResourceView* nullView = nullptr;
		context->PSSetShaderResources(0, 1, &nullView);
	}
}