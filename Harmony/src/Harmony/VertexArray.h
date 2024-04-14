#pragma once

#include "Harmony/Buffer.h"

#include <vector>

namespace Harmony {

	interface class VertexArray : public BindableBuffer {
	public:
		VertexArray() { m_VertexBuffers.reserve(HARMONY_MAX_INPUT_SLOTS); }
		virtual ~VertexArray() = default;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
		const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

		static Ref<VertexArray> Create();

	protected:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}