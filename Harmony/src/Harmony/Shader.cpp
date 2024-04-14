#include "Harmony/Shader.h"
#include "Harmony/RendererInfo.h"

#include "Backend/OpenGL/OpenGLShader.h"
#include "Backend/DX11/DX11Shader.h"

#include <fstream>

namespace Harmony {

	bool Shader::CompileSourceFromFile(const std::string& filepath) {
		std::ifstream shaderFile(filepath, std::ios::binary);

		if (!shaderFile.is_open()) {
			throw std::runtime_error("Bad file");
			return false;
		}

		std::stringstream shaderSource;
		shaderSource << shaderFile.rdbuf();

		return CompileSource(shaderSource);
	}

	Ref<Shader> Shader::Create(const ShaderInfo& shaderInfo) {
		switch (RendererInfo::GetAPI()) {
		case API::OpenGL: return MakeRef<OpenGLShader>(shaderInfo);
		case API::DX11: return MakeRef<DX11Shader>(shaderInfo);
		}

		return nullptr;
	}
}