#pragma once

#include "Harmony/RendererAPI.h"

namespace Harmony {

	class RendererInfo {
	public:
		friend class RenderCommand;

		static API GetAPI() { return s_CurrentAPI; }

	private:
		static API s_CurrentAPI;
	};
}