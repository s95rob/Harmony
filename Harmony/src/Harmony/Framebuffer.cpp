#include "Harmony/Framebuffer.h"
#include "Harmony/RendererInfo.h"

#include "Backend/OpenGL/OpenGLFramebuffer.h"

namespace Harmony {

	Ref<Framebuffer> Framebuffer::Create(const FramebufferInfo& framebufferInfo) {
		switch (RendererInfo::GetAPI()) {
		case API::OpenGL: return MakeRef<OpenGLFramebuffer>(framebufferInfo);
		}

		return nullptr;
	}

}