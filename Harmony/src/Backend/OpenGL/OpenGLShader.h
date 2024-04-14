#pragma once

#include "Harmony/Shader.h"

namespace Harmony {

	class OpenGLShader final : public Shader {
	public:
		OpenGLShader(const ShaderInfo& shaderInfo);
		~OpenGLShader();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual bool CompileSource(std::stringstream& source) override;

	protected:
		virtual bool Compile(const ShaderInfo& shaderInfo) override;

	protected:
		bool m_Valid = false;

	private:
		unsigned int m_GPUID;
	};
}