#pragma once

#include "Harmony/Math.h"

namespace Harmony {

	class Camera {
	public:
		Camera() = default;
		virtual ~Camera() = default;

		Matrix4 Projection = Matrix4(1.0f);
		Matrix4 Transform = Matrix4(1.0f);
	};

}