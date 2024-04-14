#include "Harmony/RendererAPI.h"

#include "Backend/DX11/DX11Include.h"

namespace Harmony {

	class DX11Shader;

	class DX11RendererAPI : public RendererAPI {
	public:
		virtual bool Init(const InitInfo& initInfo) override;

		virtual void Clear();
		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void SetViewport(float left, float top, float right, float bottom) override;
		virtual void SetFaceCulling(CullingMode culling) {}
		virtual void SetDepthTest(bool depthTest) {};

		virtual void Draw(DrawMode mode, unsigned int count, unsigned int startOffset) override;
		virtual void DrawInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset) override {}
		virtual void DrawIndexed(DrawMode mode, unsigned int count, unsigned int startOffset) override;
		virtual void DrawIndexedInstanced(DrawMode mode, unsigned int count, unsigned int instances, unsigned int startOffset) override {}

		virtual void Present() override;
		virtual void BindBackbuffer() override;

		static void SetActiveShader(DX11Shader* shader) { s_ActiveShader = shader; }

		static ID3D11Device* GetDevice() { return s_Device; }
		static ID3D11DeviceContext* GetDeviceContext() { return s_Context; }
		static DX11Shader* GetActiveShader() { return s_ActiveShader; }

	protected:
		bool InitDefaultDepthStencil(uint32_t width, uint32_t height);
		bool InitDefaultRenderTarget(uint32_t width, uint32_t height);

		ID3D11RenderTargetView* GetBackbuffer();

	private:
		static ID3D11Device* s_Device;
		static ID3D11DeviceContext* s_Context;
		static DX11Shader* s_ActiveShader;

		IDXGISwapChain* m_SwapChain = nullptr;

		ID3D11RenderTargetView* m_DefaultRenderTarget = nullptr;
		ID3D11Texture2D* m_DefaultRenderBuffer = nullptr;
		ID3D11DepthStencilView* m_DefaultDepthStencil = nullptr;
		ID3D11Texture2D* m_DefaultDepthBuffer = nullptr;

		ID3D11RasterizerState* m_Rasterizer = nullptr;

		D3D11_VIEWPORT m_Viewport;

		float m_ClearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	};
}