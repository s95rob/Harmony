#include "Harmony/Base.h"

#include "Backend/OpenGL/OpenGLRendererAPI.h"
#include "Backend/DX11/DX11RendererAPI.h"

namespace Harmony {

	RendererAPI* RendererAPI::Create(API api) {
		switch (api) {
		case API::OpenGL: return new OpenGLRendererAPI();
		case API::DX11: return new DX11RendererAPI();
		}

		throw std::runtime_error("Unknown/unsupported API used");
		return nullptr;
	}
}