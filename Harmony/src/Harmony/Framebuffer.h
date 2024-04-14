#pragma once

#include "Harmony/Buffer.h"

namespace Harmony {

	struct FramebufferInfo {
		int Width = 0;
		int Height = 0;
	};

	interface class Framebuffer : public BindableBuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void Resize(int width, int height) = 0;
		virtual void BindTexture() = 0;

		static Ref<Framebuffer> Create(const FramebufferInfo& framebufferInfo);
	};

}