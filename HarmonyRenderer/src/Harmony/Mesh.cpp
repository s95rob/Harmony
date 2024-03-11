#include "Harmony/Mesh.h"

#define CGLTF_IMPLEMENTATION
#include "cgltf.h"

#define STB_IMAGE_IMPLENENTATION
#include "stb_image.h"

namespace Harmony {

	struct MeshVertex {
		Vector3 Position;
		Vector3 Normal;
		Vector2 UV;
	};

	Ref<Mesh> Mesh::LoadGLTF(MeshLoadInfo& meshLoadInfo) {
		std::string filepath = meshLoadInfo.URI.substr(0, meshLoadInfo.URI.find_last_of('/'));
		std::string nodeName = meshLoadInfo.URI.substr(meshLoadInfo.URI.find_last_of('/') + 1, meshLoadInfo.URI.size());

		cgltf_options options = {};
		cgltf_data* gltf = nullptr;
		cgltf_result result = cgltf_parse_file(&options, filepath.c_str(), &gltf);
		if (result != cgltf_result_success) {
			throw std::runtime_error("Failed to open GLTF file: " + filepath);
			return nullptr;
		}

		result = cgltf_load_buffers(&options, gltf, filepath.c_str());

		// Find the requested node
		cgltf_node* targetNode = nullptr;
		for (int nodeIndex = 0; nodeIndex < gltf->nodes_count; nodeIndex++) {
			if (gltf->nodes[nodeIndex].name == nodeName)
				targetNode = &gltf->nodes[nodeIndex];
		}
		if (!targetNode) {
			throw std::runtime_error("Failed to find node \"" + nodeName + "\" in GLTF file: " + filepath);
			return nullptr;
		}


		// Load primitives from meshNode
		cgltf_mesh& meshNode = *targetNode->mesh;
		Ref<Mesh> mesh = MakeRef<Mesh>();
		mesh->m_VertexArray = VertexArray::Create();
		VertexLayout vertexLayout;
		vertexLayout.AddAttrib(Semantic::Position, DataType::Float, 3);
		vertexLayout.AddAttrib(Semantic::Normal, DataType::Float, 3);
		vertexLayout.AddAttrib(Semantic::TexCoord0, DataType::Float, 2);

		std::vector<MeshVertex> vertices;
		std::vector<uint32_t> indices;
		uint32_t indicesStartOffset = 0;
		uint32_t indicesVertexOffset = 0;

		for (int primitiveIndex = 0; primitiveIndex < meshNode.primitives_count; primitiveIndex++) {
			cgltf_primitive& primitiveNode = meshNode.primitives[primitiveIndex];
			cgltf_buffer_view* indicesView = primitiveNode.indices->buffer_view;

			cgltf_buffer_view* posView = nullptr;
			cgltf_buffer_view* normView = nullptr;
			cgltf_buffer_view* uvView = nullptr;
			cgltf_buffer_view* targetView = nullptr;

			// Find vertex attributes
			const char* posAttribName = "POSITION";
			const char* normAttribName = "NORMAL";
			const char* uvAttribName = "TEXCOORD_0";
			cgltf_attribute* attribs = primitiveNode.attributes;
			for (int attribIndex = 0; attribIndex < primitiveNode.attributes_count; attribIndex++) {
				targetView = attribs[attribIndex].data->buffer_view;
				const char* targetAttribName = attribs[attribIndex].name;

				if (strncmp(targetAttribName, posAttribName, strlen(posAttribName)) == 0)
					posView = targetView;
				else if (strncmp(targetAttribName, normAttribName, strlen(normAttribName)) == 0)
					normView = targetView;
				else if (strncmp(targetAttribName, uvAttribName, strlen(uvAttribName)) == 0)
					uvView = targetView;
			}

			// Fail if attributes missing
			if (!posView || !normView || !uvView) {
				throw std::runtime_error("Failed to load node \"" + nodeName + "\" in GLTF file: " + filepath + "\nRequired attributes missing!");
				return nullptr;
			}

			// Pack interleaved MeshVertex data
			Vector3* posAddress = (Vector3*)((uint8_t*)posView->buffer->data + posView->offset);
			Vector3* normAddress = (Vector3*)((uint8_t*)normView->buffer->data + normView->offset);
			Vector2* uvAddress = (Vector2*)((uint8_t*)uvView->buffer->data + uvView->offset);
			unsigned int vertexCount = posView->size / sizeof(Vector3);

			for (int vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++) 
				vertices.push_back({ posAddress[vertexIndex], normAddress[vertexIndex], uvAddress[vertexIndex] });

			// Pack indices data
			unsigned short* indicesAddress = (unsigned short*)((uint8_t*)indicesView->buffer->data + indicesView->offset);
			unsigned int indicesCount = primitiveNode.indices->count;

			for (int indicesIndex = 0; indicesIndex < indicesCount; indicesIndex++)
				indices.push_back(indicesAddress[indicesIndex] + indicesVertexOffset);

			// Load material
			Material material;
			cgltf_material* materialNode = primitiveNode.material;
			if (materialNode) {
				float* baseColorNode = materialNode->pbr_metallic_roughness.base_color_factor;
				cgltf_texture* baseTextureNode = materialNode->pbr_metallic_roughness.base_color_texture.texture;

				if (baseTextureNode) {
					cgltf_buffer_view* imageView = baseTextureNode->image->buffer_view;
					TextureInfo textureInfo;
					uint8_t* textureData = stbi_load_from_memory((uint8_t*)imageView->buffer->data + imageView->offset,
						imageView->size, &textureInfo.Width, &textureInfo.Height, nullptr, 4);

					textureInfo.Channels = 4;
					textureInfo.Data = textureData;
					material.Texture = Texture::Create(textureInfo);
					free(textureData);
				}
				if (baseColorNode) {
					material.BaseColor = { baseColorNode[0], baseColorNode[1], baseColorNode[2], baseColorNode[3] };
				}

			}

			mesh->m_PrimitiveData.push_back({ indicesCount, indicesStartOffset, material });

			indicesStartOffset += indicesCount;
			indicesVertexOffset += vertexCount;
		}

		VertexBufferInfo vertexBufferInfo;
		vertexBufferInfo.Data = (float*)vertices.data();
		vertexBufferInfo.Size = vertices.size() * sizeof(MeshVertex);
		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertexBufferInfo);
		vertexBuffer->SetLayout(vertexLayout);
		mesh->m_VertexArray->AddVertexBuffer(vertexBuffer);

		IndexBufferInfo indexBufferInfo;
		indexBufferInfo.Data = indices.data();
		indexBufferInfo.Count = indices.size();
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(indexBufferInfo);
		mesh->m_VertexArray->SetIndexBuffer(indexBuffer);

		cgltf_free(gltf);
		return mesh;
	}

	// todo
	Ref<SkinnedMesh> SkinnedMesh::LoadGLTF(MeshLoadInfo& meshLoadInfo) {
		Ref<SkinnedMesh> skinnedMesh = std::dynamic_pointer_cast<SkinnedMesh>(Mesh::LoadGLTF(meshLoadInfo));

		return skinnedMesh;
	}
} 