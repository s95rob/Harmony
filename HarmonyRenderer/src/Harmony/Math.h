#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Harmony {
	typedef glm::vec2 Vector2;
	typedef glm::vec3 Vector3;
	typedef glm::vec4 Vector4;

	typedef glm::mat4 Matrix4;

	typedef glm::quat Quaternion;


	Vector4 UVFromPixelCoords(const Vector4& pixelCoords, const Vector2& textureSize);
}