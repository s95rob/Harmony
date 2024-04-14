#include "Harmony/VertexArray.h"
#include "Harmony/RendererInfo.h"

#include "Backend/OpenGL/OpenGLVertexArray.h"

namespace Harmony {

	Ref<VertexArray> VertexArray::Create() {
		switch (RendererInfo::GetAPI()) {
		case API::OpenGL: return MakeRef<OpenGLVertexArray>();
		}

		return nullptr;
	}
}