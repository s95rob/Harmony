#include "Harmony/Texture.h"
#include "Harmony/RendererInfo.h"

#include "Backend/OpenGL/OpenGLTexture.h"
#include "Backend/DX11/DX11Texture.h"

namespace Harmony {

	unsigned int Texture::s_CurrentID = 0;

	Ref<Texture> Texture::Create(const TextureInfo& info) {
		HARMONY_ASSERT(info.Data != nullptr);

		switch (RendererInfo::GetAPI()) {
		case API::OpenGL: return MakeRef<OpenGLTexture>(info);
		case API::DX11: return MakeRef<DX11Texture>(info);
		}

		return nullptr;
	}
}