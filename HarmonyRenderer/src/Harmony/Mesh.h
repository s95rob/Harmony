#pragma once

#include "Harmony/VertexArray.h"
#include "Harmony/Material.h"

#include <vector>
#include <unordered_map>

namespace Harmony {

	struct Primitive {
		unsigned int IndicesCount = 0;
		unsigned int IndicesStartOffset = 0;
		Material Material;
	};

	struct MeshLoadInfo {
		std::string URI;
	};

	class Mesh {
	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		void Bind() { m_VertexArray->Bind(); }

		const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const auto& GetPrimitiveData() const { return m_PrimitiveData; }

		static Ref<Mesh> LoadGLTF(MeshLoadInfo& meshLoadInfo);

	protected:
		Ref<VertexArray> m_VertexArray;
		std::vector<Primitive> m_PrimitiveData;
	};

	class SkinnedMesh : public Mesh {
	public:
		SkinnedMesh() = default;

		static Ref<SkinnedMesh> LoadGLTF(MeshLoadInfo& meshLoadInfo);
	};

}