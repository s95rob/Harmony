#include "Backend/OpenGL/OpenGLBuffer.h"
#include "Backend/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Harmony {

	int GLEnumFromDataType(DataType dataType);
	int GLEnumFromBufferUsage(BufferUsage usage);

	void OpenGLBuffer::Release() {
		glDeleteBuffers(1, &m_GPUID);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const VertexBufferInfo& vertexBufferInfo) {
		glGenBuffers(1, &m_GPUID);
		glBindBuffer(GL_ARRAY_BUFFER, m_GPUID);
		glBufferData(GL_ARRAY_BUFFER, vertexBufferInfo.Size, 
			vertexBufferInfo.Data, GLEnumFromBufferUsage(vertexBufferInfo.Usage));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_GPUID);
	}

	void OpenGLVertexBuffer::Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_GPUID);

		for (auto& attrib : m_Layout) {
			glVertexAttribPointer(attrib.GetIndex(), attrib.GetComponentCount(),
				GLEnumFromDataType(attrib.Type), attrib.Normalized,
				m_Layout.GetStride(), (void*)attrib.GetOffset());
			glEnableVertexAttribArray(attrib.GetIndex());
		}
	}

	void OpenGLVertexBuffer::Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	}

	void OpenGLVertexBuffer::SetData(float* data, size_t size, size_t startOffset) {
		glBindBuffer(GL_ARRAY_BUFFER, m_GPUID);
		glBufferSubData(GL_ARRAY_BUFFER, startOffset, size, data);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const IndexBufferInfo& indexBufferInfo) {
		glGenBuffers(1, &m_GPUID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GPUID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferInfo.Count * sizeof(uint32_t),
			indexBufferInfo.Data, GLEnumFromBufferUsage(indexBufferInfo.Usage));

		m_Count = indexBufferInfo.Count;
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_GPUID);
	}

	void OpenGLIndexBuffer::Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GPUID);
	}

	void OpenGLIndexBuffer::Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
	}

	void OpenGLIndexBuffer::SetData(uint32_t* data, size_t size, size_t startOffset) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GPUID);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, startOffset, size, data);

		m_Count = size / sizeof(uint32_t);
	}

	OpenGLUniformBuffer::OpenGLUniformBuffer(void* data, size_t size) {
		glGenBuffers(1, &m_GPUID);
		glBindBuffer(GL_UNIFORM_BUFFER, m_GPUID);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

		m_DataSize = size;
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &m_GPUID);
	}

	void OpenGLUniformBuffer::Bind(unsigned int slot) {
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_GPUID);
		m_Slot = slot;
	}

	void OpenGLUniformBuffer::Unbind() {
		glBindBufferBase(GL_UNIFORM_BUFFER, m_Slot, GL_NONE);
		m_Slot = 0;
	}

	void OpenGLUniformBuffer::SetData(void* data, size_t size, size_t offset) {
		glBindBuffer(GL_UNIFORM_BUFFER, m_GPUID);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

		glBindBufferBase(GL_UNIFORM_BUFFER, m_Slot, m_GPUID);
	}

	int GLEnumFromDataType(DataType dataType) {
		switch (dataType) {
		case DataType::Int: return GL_INT;
		case DataType::Uint: return GL_UNSIGNED_INT;
		case DataType::Float: return GL_FLOAT;
		}

		return GL_NONE;
	}

	int GLEnumFromBufferUsage(BufferUsage usage) {
		switch (usage) {
		case BufferUsage::Static: return GL_STATIC_DRAW;
		case BufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
		}

		return GL_NONE;
	}
}