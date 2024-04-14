#include "Harmony/Framebuffer.h"
#include "Backend/OpenGL/OpenGLBuffer.h"

namespace Harmony {

	class OpenGLFramebuffer final : public Framebuffer, public OpenGLBuffer {
	public:
		OpenGLFramebuffer(const FramebufferInfo& framebufferInfo);
		~OpenGLFramebuffer();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(int width, int height) override;
		virtual void BindTexture() override;

	private:
		void InitBufferTargets(int width, int height);

	private:
		unsigned int m_Slot = 0;
		unsigned int m_ColorTargetID = 0;
		unsigned int m_DepthTargetID = 0;
	};
}