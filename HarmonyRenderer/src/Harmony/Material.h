#pragma once

#include "Harmony/Math.h"
#include "Harmony/Texture.h"

namespace Harmony {

	struct Material {
		Vector4 BaseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture> Texture = nullptr;
	};

}