#include "Harmony/Math.h"

namespace Harmony {

	Vector4 UVFromPixelCoords(const Vector4& pixelCoords, const Vector2& textureSize) {
		return {
			pixelCoords.x / textureSize.x,
			pixelCoords.y / textureSize.y,
			(pixelCoords.x + pixelCoords.z) / textureSize.x,
			(pixelCoords.y + pixelCoords.w) / textureSize.y
		};
	}
}