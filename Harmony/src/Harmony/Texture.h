#pragma once

#include "Harmony/Buffer.h"

namespace Harmony {

	enum class TextureFilter {
		Nearest = 0, Linear
	};

	enum class TextureWrap {
		Repeat = 0, Clamp
	};

	struct TextureInfo {
		TextureFilter NearFilter = TextureFilter::Nearest;
		TextureFilter FarFilter = TextureFilter::Linear;
		TextureWrap Wrap = TextureWrap::Clamp;

		void* Data = nullptr;
		size_t Size = 0;

		int Width = 0,
			Height = 0,
			Channels = 0;

	};

	interface class Texture : public BindableBuffer {
	public:
		virtual ~Texture() = default;

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		unsigned int GetID() const { return m_ID; }

		static Ref<Texture> Create(const TextureInfo& info);

	protected:
		unsigned int m_Slot = 0,
			m_Width = 0,
			m_Height = 0,
			m_ID = 0;

		static unsigned int s_CurrentID;
	};
}