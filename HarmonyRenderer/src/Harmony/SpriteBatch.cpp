#include "Harmony/SpriteBatch.h"

#include "Harmony/Buffer.h"
#include "Harmony/RenderCommand.h"
#include "Harmony/Texture.h"

#include <unordered_map>

namespace Harmony {

	constexpr size_t MaxSpriteBatchQuads = 1000;
	constexpr size_t MaxSpriteBatchVertices = MaxSpriteBatchQuads * 4;
	constexpr size_t MaxSpriteBatchIndices = MaxSpriteBatchQuads * 6;
	constexpr uint8_t MaxTextureSlots = 16;

	struct SpriteBatchVertex {
		Vector2 Position;
		Vector4 Color;
		Vector2 UV;
		float TextureSlot;
	};

	struct SpriteBatchData {
		SpriteBatchVertex* Vertices = nullptr;
		SpriteBatchVertex* TargetVertex = nullptr;
		uint32_t* Indices = nullptr;
		size_t IndexCount = 0;

		Ref<VertexBuffer> VertexBuffer = nullptr;
		Ref<IndexBuffer> IndexBuffer = nullptr;

		Ref<Texture> DefaultTexture = nullptr;
		std::unordered_map<unsigned int, std::pair<Ref<Texture>, uint32_t>> TextureSlots; // < TextureID < Texture, TextureSlot > >
		uint32_t TextureSlotIndex = 1;
	};

	static SpriteBatchData spriteBatchData;


	void SpriteBatch::Init() {
		spriteBatchData.Vertices = new SpriteBatchVertex[MaxSpriteBatchVertices];
		spriteBatchData.Indices = new uint32_t[MaxSpriteBatchIndices];

		size_t indexOffset = 0;
		for (int i = 0; i < MaxSpriteBatchIndices; i += 6) {
			spriteBatchData.Indices[i + 0] = 3 + indexOffset;
			spriteBatchData.Indices[i + 1] = 2 + indexOffset;
			spriteBatchData.Indices[i + 2] = 0 + indexOffset;

			spriteBatchData.Indices[i + 3] = 2 + indexOffset;
			spriteBatchData.Indices[i + 4] = 1 + indexOffset;
			spriteBatchData.Indices[i + 5] = 0 + indexOffset;

			indexOffset += 4;
		}

		VertexBufferInfo spriteBatchVertexInfo;
		spriteBatchVertexInfo.Data = nullptr;
		spriteBatchVertexInfo.Size = MaxSpriteBatchVertices * sizeof(SpriteBatchVertex);
		spriteBatchVertexInfo.Usage = BufferUsage::Dynamic;
		spriteBatchData.VertexBuffer = VertexBuffer::Create(spriteBatchVertexInfo);

		VertexLayout spriteBatchVertexLayout;
		spriteBatchVertexLayout.AddAttrib(Semantic::None, DataType::Float, 2);		// Position
		spriteBatchVertexLayout.AddAttrib(Semantic::Color0, DataType::Float, 4);	// Color
		spriteBatchVertexLayout.AddAttrib(Semantic::TexCoord0, DataType::Float, 2);	// UV
		spriteBatchVertexLayout.AddAttrib(Semantic::None, DataType::Float, 1);		// TextureSlot
		spriteBatchData.VertexBuffer->SetLayout(spriteBatchVertexLayout);

		IndexBufferInfo spriteBatchIndexInfo;
		spriteBatchIndexInfo.Data = spriteBatchData.Indices;
		spriteBatchIndexInfo.Count = MaxSpriteBatchIndices;
		spriteBatchData.IndexBuffer = IndexBuffer::Create(spriteBatchIndexInfo);

		uint32_t defaultTexture = 0xFFFFFFFF;
		TextureInfo spriteBatchDefaultTextureInfo;
		spriteBatchDefaultTextureInfo.Data = &defaultTexture;
		spriteBatchDefaultTextureInfo.Width = 1;
		spriteBatchDefaultTextureInfo.Height = 1;
		spriteBatchDefaultTextureInfo.Channels = 1;
		spriteBatchData.DefaultTexture = Texture::Create(spriteBatchDefaultTextureInfo);
	}

	void SpriteBatch::Close() {
		delete[] spriteBatchData.Vertices, spriteBatchData.Indices;
		spriteBatchData.VertexBuffer->Release();
		spriteBatchData.IndexBuffer->Release();
		spriteBatchData.DefaultTexture->Release();
	}

	void SpriteBatch::Begin() {
		std::memset(spriteBatchData.Vertices, 0, MaxSpriteBatchVertices);
		spriteBatchData.TargetVertex = spriteBatchData.Vertices;
		spriteBatchData.IndexCount = 0;

		spriteBatchData.TextureSlotIndex = 1;
	}

	void SpriteBatch::End() {
		Flush();
	}

	void SpriteBatch::Flush() {
		spriteBatchData.VertexBuffer->Bind();
		spriteBatchData.VertexBuffer->SetData((float*)spriteBatchData.Vertices, MaxSpriteBatchVertices * sizeof(float));

		spriteBatchData.DefaultTexture->Bind(0);
		for (auto& pair : spriteBatchData.TextureSlots) {
			pair.second.first->Bind(pair.second.second);
		}

		RenderCommand::DrawIndexed(DrawMode::Triangles, MaxSpriteBatchIndices);

		Begin();
	}

	void SpriteBatch::Draw(const SpriteBatchDrawInfo& drawInfo) {
		if (spriteBatchData.IndexCount >= MaxSpriteBatchIndices ||
			(drawInfo.Texture && spriteBatchData.TextureSlotIndex >= MaxTextureSlots))
			Flush();

		uint8_t textureSlot = 0;
		if (drawInfo.Texture) {
			if (spriteBatchData.TextureSlots.find(drawInfo.Texture->GetID()) != spriteBatchData.TextureSlots.end())
				textureSlot = spriteBatchData.TextureSlots[drawInfo.Texture->GetID()].second;
			else {
				spriteBatchData.TextureSlots[drawInfo.Texture->GetID()] = std::make_pair(drawInfo.Texture, spriteBatchData.TextureSlotIndex);
				textureSlot = spriteBatchData.TextureSlotIndex++;
			}
		}

		Vector2 textureSize = { 1.0f, 1.0f };
		if (!drawInfo.UVNormalized)
			textureSize = { drawInfo.Texture->GetWidth(), drawInfo.Texture->GetHeight() };

		// Top left
		spriteBatchData.TargetVertex->Position = { drawInfo.Transform.x, drawInfo.Transform.y };
		spriteBatchData.TargetVertex->Color = { drawInfo.Color.r, drawInfo.Color.g, drawInfo.Color.b, drawInfo.Color.a };
		spriteBatchData.TargetVertex->UV = { drawInfo.UVCoordinates.x / textureSize.x, drawInfo.UVCoordinates.y / textureSize.y };
		spriteBatchData.TargetVertex->TextureSlot = textureSlot;
		spriteBatchData.TargetVertex++;

		// Top right
		spriteBatchData.TargetVertex->Position = { drawInfo.Transform.x + drawInfo.Transform.z, drawInfo.Transform.y };
		spriteBatchData.TargetVertex->Color = { drawInfo.Color.r, drawInfo.Color.g, drawInfo.Color.b, drawInfo.Color.a };
		spriteBatchData.TargetVertex->UV = { (drawInfo.UVCoordinates.x + drawInfo.UVCoordinates.z) / textureSize.x, drawInfo.UVCoordinates.y / textureSize.y };
		spriteBatchData.TargetVertex->TextureSlot = textureSlot;
		spriteBatchData.TargetVertex++;

		// Bottom right
		spriteBatchData.TargetVertex->Position = { drawInfo.Transform.x + drawInfo.Transform.z, drawInfo.Transform.y + drawInfo.Transform.w };
		spriteBatchData.TargetVertex->Color = { drawInfo.Color.r, drawInfo.Color.g, drawInfo.Color.b, drawInfo.Color.a };
		spriteBatchData.TargetVertex->UV = { (drawInfo.UVCoordinates.x + drawInfo.UVCoordinates.z) / textureSize.x, (drawInfo.UVCoordinates.y + drawInfo.UVCoordinates.w) / textureSize.y };
		spriteBatchData.TargetVertex->TextureSlot = textureSlot;
		spriteBatchData.TargetVertex++;

		// Bottom left
		spriteBatchData.TargetVertex->Position = { drawInfo.Transform.x, drawInfo.Transform.y + drawInfo.Transform.w };
		spriteBatchData.TargetVertex->Color = { drawInfo.Color.r, drawInfo.Color.g, drawInfo.Color.b, drawInfo.Color.a };
		spriteBatchData.TargetVertex->UV = { drawInfo.UVCoordinates.x / textureSize.x, (drawInfo.UVCoordinates.y + drawInfo.UVCoordinates.w) / textureSize.y };
		spriteBatchData.TargetVertex->TextureSlot = textureSlot;
		spriteBatchData.TargetVertex++;

		spriteBatchData.IndexCount += 4;
	}
}