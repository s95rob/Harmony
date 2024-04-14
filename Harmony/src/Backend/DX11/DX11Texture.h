#pragma once

#include "Harmony/Texture.h"

#include "Backend/DX11/DX11Include.h"

namespace Harmony {

	class DX11Texture : public Texture {
	public:
		DX11Texture(const TextureInfo& textureInfo);
		~DX11Texture();

		virtual void Bind(unsigned int slot = 0) override;
		virtual void Unbind() override;

	protected:
		ID3D11Texture2D* m_Texture = nullptr;
		ID3D11SamplerState* m_Sampler = nullptr;
		ID3D11ShaderResourceView* m_ShaderResource = nullptr;
	};

}