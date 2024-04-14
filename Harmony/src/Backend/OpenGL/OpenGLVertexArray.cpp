#include "Backend/OpenGL/OpenGLVertexArray.h"
#include "Backend/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Harmony {

	extern int GLEnumFromDataType(DataType dataType);

	OpenGLVertexArray::OpenGLVertexArray() {
		glGenVertexArrays(1, &m_GPUID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_GPUID);
	}

	void OpenGLVertexArray::Bind() {
		glBindVertexArray(m_GPUID);
	}

	void OpenGLVertexArray::Unbind() {
		OpenGLRendererAPI::BindImmediateVertexArray();
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		glBindVertexArray(m_GPUID);

		glBindBuffer(GL_ARRAY_BUFFER, 
			std::static_pointer_cast<const OpenGLVertexBuffer>(vertexBuffer)->GetGPUID());

		for (auto& attrib : vertexBuffer->GetLayout()) {
			glVertexAttribPointer(attrib.GetIndex(), attrib.GetComponentCount(),
				GLEnumFromDataType(attrib.Type), attrib.Normalized,
				vertexBuffer->GetLayout().GetStride(), (void*)attrib.GetOffset());
			glEnableVertexAttribArray(attrib.GetIndex());
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_GPUID);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			std::static_pointer_cast<const OpenGLIndexBuffer>(indexBuffer)->GetGPUID());

		m_IndexBuffer = indexBuffer;
	}

}