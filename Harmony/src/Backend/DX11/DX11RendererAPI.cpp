#include "Backend/DX11/DX11RendererAPI.h"

namespace Harmony {

	ID3D11Device* DX11RendererAPI::s_Device = nullptr;
	ID3D11DeviceContext* DX11RendererAPI::s_Context = nullptr;
	DX11Shader* DX11RendererAPI::s_ActiveShader = nullptr;

	bool DX11RendererAPI::Init(const InitInfo& initInfo) {
		HRESULT result = S_OK;

		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = GetActiveWindow(); // This may not work as intended?
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			D3D11_CREATE_DEVICE_DEBUG, nullptr, 0,
			D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &s_Device, nullptr, &s_Context);

		if (result != S_OK)
			return false;

		ID3D11Texture2D* backbuffer = nullptr;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
		s_Device->CreateRenderTargetView(backbuffer, nullptr, &m_DefaultRenderTarget);
		backbuffer->Release();

		InitDefaultDepthStencil(initInfo.Width, initInfo.Height);
		//InitDefaultRenderTarget(initInfo.Width, initInfo.Height);

		s_Context->OMSetRenderTargets(1, &m_DefaultRenderTarget, m_DefaultDepthStencil);


		// Init rasterizer
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = true;

		// Init blendstate
		ID3D11BlendState* blendState = nullptr;
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		result = s_Device->CreateBlendState(&blendDesc, &blendState);
		s_Context->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
		blendState->Release();

		s_Device->CreateRasterizerState(&rasterizerDesc, &m_Rasterizer);
		s_Context->RSSetState(m_Rasterizer);

		return true;
	}

	bool DX11RendererAPI::InitDefaultRenderTarget(uint32_t width, uint32_t height) {
		HRESULT result = S_OK;

		D3D11_TEXTURE2D_DESC bufferDesc = {};
		bufferDesc.Width = width;
		bufferDesc.Height = height;
		bufferDesc.MipLevels = 1;
		bufferDesc.ArraySize = 1;
		bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.SampleDesc.Quality = 0;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		result = s_Device->CreateTexture2D(&bufferDesc, nullptr, &m_DefaultRenderBuffer);

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc = {};
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		result = s_Device->CreateRenderTargetView(m_DefaultRenderBuffer, 
			&renderTargetDesc, &m_DefaultRenderTarget);

		return result == S_OK;
	}

	bool DX11RendererAPI::InitDefaultDepthStencil(uint32_t width, uint32_t height) {
		HRESULT result = S_OK;

		// Setup depth buffer resource
		D3D11_TEXTURE2D_DESC bufferDesc = {};
		bufferDesc.Width = width;
		bufferDesc.Height = height;
		bufferDesc.MipLevels = 1;
		bufferDesc.ArraySize = 1;
		bufferDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		bufferDesc.SampleDesc.Count = 1;
		bufferDesc.SampleDesc.Quality = 0;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		result = s_Device->CreateTexture2D(&bufferDesc, nullptr, &m_DefaultDepthBuffer);

		// Depth test parameters
		ID3D11DepthStencilState* depthStencilState = nullptr;
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		// Stencil test parameters
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		// Front-facing operations
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// Back-facing operations
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		result = s_Device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
		s_Context->OMSetDepthStencilState(depthStencilState, 1);

		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.Flags = 0;
		result = s_Device->CreateDepthStencilView(m_DefaultDepthBuffer, &depthStencilViewDesc,
			&m_DefaultDepthStencil);
		depthStencilState->Release();

		return result == S_OK;
	}

	void DX11RendererAPI::Clear() {
		ID3D11RenderTargetView* currentRenderTarget = nullptr;
		ID3D11DepthStencilView* currentDepthStencil = nullptr;

		s_Context->OMGetRenderTargets(1, &currentRenderTarget, &currentDepthStencil);
		s_Context->ClearRenderTargetView(currentRenderTarget, m_ClearColor);
		s_Context->ClearDepthStencilView(currentDepthStencil,
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void DX11RendererAPI::SetClearColor(float r, float g, float b, float a) {
		m_ClearColor[0] = r;
		m_ClearColor[1] = g;
		m_ClearColor[2] = b;
		m_ClearColor[3] = a;
	}

	void DX11RendererAPI::SetViewport(float left, float top, float right, float bottom) {
		m_Viewport = {};

		m_Viewport.TopLeftX = left;
		m_Viewport.TopLeftY = top;
		m_Viewport.Width = right;
		m_Viewport.Height = bottom;

		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;

		s_Context->RSSetViewports(1, &m_Viewport);
	}

	void DX11RendererAPI::Draw(DrawMode mode, unsigned int count, unsigned int startOffset) {
		s_Context->Draw(count, startOffset);
	}

	void DX11RendererAPI::DrawIndexed(DrawMode mode, unsigned int count, unsigned int startOffset) {
		s_Context->DrawIndexed(count, startOffset, 0);
	}

	void DX11RendererAPI::Present() {
		/*ID3D11Texture2D* backbuffer = nullptr;
		m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer);
		
		ID3D11RenderTargetView* currentRenderTarget = nullptr;
		ID3D11DepthStencilView* currentDepthStencil = nullptr;
		ID3D11Resource* currentRenderTexture = nullptr;

		s_Context->OMGetRenderTargets(1, &currentRenderTarget, &currentDepthStencil);
		currentRenderTarget->GetResource(&currentRenderTexture);

		s_Context->ResolveSubresource(backbuffer, 0, currentRenderTexture, 0,
			DXGI_FORMAT_R8G8B8A8_UNORM);
			*/
		m_SwapChain->Present(1, 0);

		s_Context->OMSetRenderTargets(1, &m_DefaultRenderTarget, m_DefaultDepthStencil);
	}

	void DX11RendererAPI::BindBackbuffer() {

	}

	ID3D11RenderTargetView* DX11RendererAPI::GetBackbuffer() {
		return nullptr;
	}
}