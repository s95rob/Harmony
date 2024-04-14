#pragma once

#include "Harmony/Texture.h"
#include "Backend/OpenGL/OpenGLBuffer.h"

namespace Harmony {

	class OpenGLTexture : public Texture, public OpenGLBuffer {
	public:
		OpenGLTexture(const TextureInfo& textureInfo);
		~OpenGLTexture();

		virtual void Bind(unsigned int slot = 0) override;
		virtual void Unbind() override;
	};
}