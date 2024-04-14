#pragma once

#include "Harmony/Shader.h"

#include "Backend/DX11/DX11Include.h"

namespace Harmony {

	class DX11Shader : public Shader {
	public:
		DX11Shader(const ShaderInfo& shaderInfo);
		virtual ~DX11Shader();

		virtual void Bind() override;
		virtual void Unbind() override;

		void* GetVertexShaderData() { return m_VSBlob->GetBufferPointer(); }
		void* GetPixelShaderData() { return m_PSBlob->GetBufferPointer(); }
		size_t GetVertexShaderDataSize() const { return m_VSBlob->GetBufferSize(); }
		size_t GetPixelShaderDataSize() const { return m_PSBlob->GetBufferSize(); }

	protected:
		virtual bool Compile(const ShaderInfo& shaderInfo);

	protected:
		ID3D11VertexShader* m_VertexShader = nullptr;
		ID3D11PixelShader* m_PixelShader = nullptr;

		ID3D10Blob* m_VSBlob = nullptr;
		ID3D10Blob* m_PSBlob = nullptr;
	};
}