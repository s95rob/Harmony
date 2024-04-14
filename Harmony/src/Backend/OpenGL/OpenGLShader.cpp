#include "Backend/OpenGL/OpenGLShader.h"
#include "Harmony/ShaderPipeline.h"

#include <glad/glad.h>

#define GL_INFO_LOG_SIZE 1024

namespace Harmony {

	OpenGLShader::OpenGLShader(const ShaderInfo& shaderInfo) {
		m_GPUID = glCreateProgram();
		m_Valid = Compile(shaderInfo);
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_GPUID);
	}

	void OpenGLShader::Bind() {
		glUseProgram(m_GPUID);
	}

	void OpenGLShader::Unbind() {
		glUseProgram(GL_NONE);
	}

	bool OpenGLShader::Compile(const ShaderInfo& shaderInfo) {
		unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		int success = 0;
		char infoLog[GL_INFO_LOG_SIZE] = {};

		std::string vertexSource = ShaderPipeline::GenerateGLSLSource((uint32_t*)shaderInfo.VertexData, shaderInfo.VertexDataSize / 4);
		const char* vertexShaderSource = vertexSource.c_str();
		glShaderSource(vertexShaderID, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShaderID);

		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShaderID, GL_INFO_LOG_SIZE, nullptr, infoLog);
			throw std::runtime_error(infoLog);
		}

		std::string fragmentSource = ShaderPipeline::GenerateGLSLSource((uint32_t*)shaderInfo.FragmentData, shaderInfo.FragmentDataSize / 4);
		const char* fragmentShaderSource = fragmentSource.c_str();
		glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShaderID);

		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShaderID, GL_INFO_LOG_SIZE, nullptr, infoLog);
			throw std::runtime_error(infoLog);
		}

		glAttachShader(m_GPUID, vertexShaderID);
		glAttachShader(m_GPUID, fragmentShaderID);
		glLinkProgram(m_GPUID);

		glGetProgramiv(m_GPUID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_GPUID, GL_INFO_LOG_SIZE, nullptr, infoLog);
			throw std::runtime_error(infoLog);
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return success;

		return success;
	}

	bool OpenGLShader::CompileSource(std::stringstream& source) {
		return true;
	}
}