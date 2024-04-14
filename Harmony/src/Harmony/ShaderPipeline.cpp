#include "Harmony/ShaderPipeline.h"

#include "spirv_glsl.hpp"
#include "spirv_hlsl.hpp"

namespace Harmony {

	std::string ShaderPipeline::GenerateGLSLSource(uint32_t* binary, size_t size) {
		spirv_cross::CompilerGLSL glsl(binary, size);

		spirv_cross::CompilerGLSL::Options glslOptions;
		glslOptions.version = 330;
		glslOptions.es = false;
		glsl.set_common_options(glslOptions);

		std::string source = glsl.compile();
		return source;
	}

	std::string ShaderPipeline::GenerateHLSLSource(uint32_t* binary, size_t size) {
		spirv_cross::CompilerHLSL hlsl(binary, size);

		spirv_cross::CompilerHLSL::Options hlslOptions;
		for (int i = 0; i < 10; i++) {
			spirv_cross::HLSLVertexAttributeRemap remap = { i, "VERTEX_ATTRIB" + std::to_string(i) };
			hlsl.add_vertex_attribute_remap(remap);
		}
		hlslOptions.shader_model = 40;
		hlsl.set_hlsl_options(hlslOptions);

		std::string source = hlsl.compile();
		return source;
	}
}