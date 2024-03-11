#include "Harmony/Buffer.h"
#include "Harmony/Mesh.h"
#include "Harmony/Shader.h"

#include <vector>

namespace Harmony {

	struct RenderSubmission {
		Ref<Mesh> Mesh = nullptr;
		Matrix4 Transform = Matrix4(1.0f);
	};

	struct LightingData {
		Vector4 AmbientColor = Vector4(Vector3(1.0f), 0.77f);
		Vector3 LightDirection = { 0.77f, -1.0f, -1.0f };
	};

	struct RendererData {
		Ref<Shader> StandardShader;

		Ref<UniformBuffer> MVPUniformBuffer;
		Ref<UniformBuffer> MaterialUniformBuffer;
		Ref<UniformBuffer> LightingUniformBuffer;

		std::vector<RenderSubmission> RenderSubmissions;
	};
}