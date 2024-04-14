#include "Backend/DX11/DX11Shader.h"
#include "Backend/DX11/DX11RendererAPI.h"

#include "Harmony/ShaderPipeline.h"

namespace Harmony {

	DX11Shader::DX11Shader(const ShaderInfo& shaderInfo) {
		Compile(shaderInfo);
	}

	DX11Shader::~DX11Shader() {
		m_VSBlob->Release();
		m_PSBlob->Release();
		m_VertexShader->Release();
		m_PixelShader->Release();
	}

	bool DX11Shader::Compile(const ShaderInfo& shaderInfo) {
		auto* device = DX11RendererAPI::GetDevice();

		std::string vertexSource = ShaderPipeline::GenerateHLSLSource((uint32_t*)shaderInfo.VertexData, shaderInfo.VertexDataSize / 4);
		std::string fragmentSource = ShaderPipeline::GenerateHLSLSource((uint32_t*)shaderInfo.FragmentData, shaderInfo.FragmentDataSize / 4);
		ID3DBlob* errorMsg = nullptr;

		UINT compileFlags = 0;
		#if _DEBUG
			compileFlags |= D3DCOMPILE_DEBUG;
		#endif

		HRESULT result = S_OK;

		result = D3DCompile(vertexSource.data(), vertexSource.size(), "VertexShader", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", "vs_4_0", compileFlags, 0, &m_VSBlob, &errorMsg);
		if (result != S_OK)
			throw std::runtime_error((char*)errorMsg->GetBufferPointer());
		else
			device->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr, &m_VertexShader);

		result = D3DCompile(fragmentSource.data(), fragmentSource.size(), "PixelShader", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", "ps_4_0", compileFlags, 0, &m_PSBlob, &errorMsg);
		if (result != S_OK)
			throw std::runtime_error((char*)errorMsg->GetBufferPointer());
		else
			device->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr, &m_PixelShader);

		return result == S_OK;
	}

	void DX11Shader::Bind() {
		auto* context = DX11RendererAPI::GetDeviceContext();

		context->VSSetShader(m_VertexShader, nullptr, 0);
		context->PSSetShader(m_PixelShader, nullptr, 0);

		DX11RendererAPI::SetActiveShader(this);
	}

	void DX11Shader::Unbind() {
		auto* context = DX11RendererAPI::GetDeviceContext();

		context->VSSetShader(nullptr, nullptr, 0);
		context->PSSetShader(nullptr, nullptr, 0);

		DX11RendererAPI::SetActiveShader(nullptr);
	}
}