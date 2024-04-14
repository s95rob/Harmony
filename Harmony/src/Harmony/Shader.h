#pragma once

#include "Harmony/Buffer.h"

#include <sstream>

namespace Harmony {

	enum class ShaderLanguage {
		None = 0,
		GLSL, HLSL
	};

	struct ShaderInfo {
		uint8_t* VertexData = nullptr,
			   * FragmentData = nullptr;
		size_t VertexDataSize = 0,
			   FragmentDataSize = 0;
	};

	interface class Shader : public BindableBuffer {
	public:
		virtual ~Shader() = default;

		virtual bool CompileSource(std::stringstream& source) { return true;  }
		bool CompileSourceFromFile(const std::string& filepath);

		static Ref<Shader> Create(const ShaderInfo& shaderInfo);

	protected:
		virtual bool Compile(const ShaderInfo& shaderInfo) = 0;
	};
}