#pragma once

#include <string>

namespace Harmony {

	class ShaderPipeline {
	public:
		static std::string GenerateGLSLSource(uint32_t* binary, size_t size);
		static std::string GenerateHLSLSource(uint32_t* binary, size_t size);
	};
}