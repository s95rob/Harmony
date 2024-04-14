#include "Harmony/RenderCommand.h"
#include "Harmony/RendererInfo.h"

namespace Harmony {

	RendererAPI* RenderCommand::s_API = nullptr;

	API RendererInfo::s_CurrentAPI = API::None;

	bool RenderCommand::Init(InitInfo& initInfo) {

		// Generic platform detection
		if (initInfo.API == API::Auto) {
			#ifdef HARMONY_PLATFORM_WINDOWS
				initInfo.API = API::DX11;
			#else
				initInfo.API = API::OpenGL;
			#endif
		}

		s_API = RendererAPI::Create(initInfo.API);
		bool success = s_API->Init(initInfo);

		if (!success) {
			delete s_API;
			return false;
		}

		RendererInfo::s_CurrentAPI = initInfo.API;
		return true;
	}

	void RenderCommand::Close() {
		if (s_API != nullptr) {
			s_API->Close();
			delete s_API;
		}
	}
}