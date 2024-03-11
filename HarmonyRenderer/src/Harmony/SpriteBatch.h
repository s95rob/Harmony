#pragma once

#include "Harmony/Math.h"
#include "Harmony/Texture.h"

namespace Harmony {

	struct SpriteBatchDrawInfo {
		Vector4 Transform = { 0.0f, 0.0f, 1.0f, 1.0f };
		Vector4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture> Texture = nullptr;
		Vector4 UVCoordinates = { 0.0f, 0.0f, 1.0f, 1.0f };
		bool UVNormalized = false;
	};

	class SpriteBatch {
	public:
		friend class Renderer;

		static void Init();
		static void Close();

		static void Begin();
		static void Flush();
		static void End();

		static void Draw(const SpriteBatchDrawInfo& drawInfo);
	};
}