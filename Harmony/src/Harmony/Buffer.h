#pragma once

#include "Harmony/Base.h"

#include <cstdint>
#include <vector>

namespace Harmony {

	enum class Semantic {
		None = 0,
		Position, 
		Normal, 

		TexCoord0,
		TexCoord1,
		TexCoord2,
		TexCoord3,
		TexCoord4,
		TexCoord5,
		TexCoord6,
		TexCoord7,

		Color0,
		Color1,
		Color2,
		Color3,
		Color4,
		Color5,
		Color6,
		Color7,
	};

	enum class DataType {
		None = 0,
		Int,
		Uint,
		Float
	};

	struct VertexAttrib {
		friend class VertexLayout;

		Semantic SemanticName;
		DataType Type;
		bool Normalized = false;

		VertexAttrib() = default;
		VertexAttrib(Semantic semantic, DataType dataType, unsigned int count = 1, bool normalized = false)
			: SemanticName(semantic), Type(dataType), m_ComponentCount(count), Normalized(normalized)  {}

		size_t Size() const {
			switch (Type) {
			case DataType::Float: return sizeof(float) * m_ComponentCount;
			case DataType::Uint: return sizeof(int) * m_ComponentCount;
			case DataType::Int: return sizeof(int) * m_ComponentCount;
			}

			return 0;
		}

		unsigned int GetIndex() const { return m_Index; }
		unsigned int GetComponentCount() const { return m_ComponentCount; }
		size_t GetOffset() const { return m_Offset; }

	private:
		unsigned int m_ComponentCount;
		unsigned int m_Index = 0;
		size_t m_Offset = 0;
	};

	class VertexLayout {
	public:
		VertexLayout() = default;

		void AddAttrib(Semantic semantic, DataType type, unsigned int count, bool normalized = false) {
			this->AddAttrib(VertexAttrib(semantic, type, count, normalized));
		}

		void AddAttrib(VertexAttrib& attrib) {
			attrib.m_Offset += m_Stride;
			attrib.m_Index = m_Attribs.size();
			m_Attribs.push_back(attrib);
			m_Stride += attrib.Size();
		}

		const std::vector<VertexAttrib>& GetAttribs() const { return m_Attribs; }
		unsigned int GetAttribCount() const { return m_Attribs.size(); }
		unsigned int GetStride() const { return m_Stride; }

		std::vector<VertexAttrib>::iterator begin() { return m_Attribs.begin(); }
		std::vector<VertexAttrib>::iterator end() { return m_Attribs.end(); }
		std::vector<VertexAttrib>::const_iterator begin() const { return m_Attribs.begin(); }
		std::vector<VertexAttrib>::const_iterator end() const { return m_Attribs.end(); }

		const VertexAttrib& operator[](int index) const {
			return m_Attribs[index];
		}

	private:
		std::vector<VertexAttrib> m_Attribs;
		unsigned int m_Stride = 0;
	};

	interface class CastableBuffer {
	public:
		virtual ~CastableBuffer() = default;

		template <typename T>
		T Cast() { return reinterpret_cast<T>(*this); }
	};

	interface class BindableBuffer : public CastableBuffer {
	public:
		virtual ~BindableBuffer() = default;

		virtual void Bind() {}
		virtual void Bind(uint32_t) {}
		virtual void Unbind() {}

		virtual void Release() {}
	};

	enum class BufferUsage {
		Static = 0, Dynamic
	};
	
	struct VertexBufferInfo {
		float* Data = nullptr;
		size_t Size = 0;
		BufferUsage Usage = BufferUsage::Static;
	};

	interface class VertexBuffer : public BindableBuffer {
	public:
		virtual ~VertexBuffer() = default;

		virtual void SetData(float* data, size_t size, size_t startOffset = 0) = 0;

		virtual void SetLayout(const VertexLayout& layout) { m_Layout = layout; }
		const VertexLayout& GetLayout() const { return m_Layout; }

		static Ref<VertexBuffer> Create(const VertexBufferInfo& vertexBufferInfo);

	protected:
		VertexLayout m_Layout;
	};

	struct IndexBufferInfo {
		uint32_t* Data = nullptr;
		unsigned int Count = 0;
		BufferUsage Usage = BufferUsage::Static;
	};

	interface class IndexBuffer : public BindableBuffer {
	public:
		virtual ~IndexBuffer() = default;

		virtual void SetData(uint32_t* data, size_t size, size_t startOffset = 0) = 0;

		unsigned int GetCount() const { return m_Count; }

		static Ref<IndexBuffer> Create(const IndexBufferInfo& indexBufferInfo);

	protected:
		unsigned int m_Count = 0;
	};

	interface class UniformBuffer : public BindableBuffer {
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetData(void* data, size_t size, size_t offset = 0) = 0;

		virtual void Bind(unsigned int slot) = 0;
		virtual void Unbind() = 0;

		static Ref<UniformBuffer> Create(void* data, size_t size);

	protected:
		unsigned int m_Slot = 0;
	};

	using ConstantBuffer = UniformBuffer;
}