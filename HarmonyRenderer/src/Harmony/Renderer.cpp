#include "Harmony/Renderer.h"

#include "Harmony/Buffer.h"
#include "Harmony/RenderCommand.h"
#include "Harmony/SpriteBatch.h"

#include <algorithm>

namespace Harmony {

	RendererData* Renderer::s_Data = nullptr;

	void Renderer::Init() {
		s_Data = new RendererData();
		InitData();
	}

	void Renderer::Close() {
		delete s_Data;
	}

	void Renderer::Begin(Camera& camera) {
		s_Data->MVPUniformBuffer->Bind(0);
		s_Data->MVPUniformBuffer->SetData(&camera.Transform, sizeof(Matrix4), sizeof(Matrix4));
		s_Data->MVPUniformBuffer->SetData(&camera.Projection, sizeof(Matrix4), sizeof(Matrix4) * 2);

		s_Data->RenderSubmissions.clear();
	}

	void Renderer::End() {
		// Sort submissions by Z translation value
		std::sort(s_Data->RenderSubmissions.begin(), s_Data->RenderSubmissions.end(),
			[](const RenderSubmission& lhs, const RenderSubmission& rhs) {
				return lhs.Transform[3][2] > rhs.Transform[3][2];
			});

		// Draw submissions per material
		s_Data->StandardShader->Bind();
		s_Data->LightingUniformBuffer->Bind(2);
		for (auto& submission : s_Data->RenderSubmissions) {
			auto& vertexArray = submission.Mesh->GetVertexArray();

			s_Data->MVPUniformBuffer->SetData(&submission.Transform, sizeof(Matrix4));
			vertexArray->Bind();
			s_Data->MaterialUniformBuffer->Bind(1);

			auto& primitiveData = submission.Mesh->GetPrimitiveData();
			for (auto it = primitiveData.begin(); it != primitiveData.end(); ++it) {
				auto next = it + 1;

				unsigned int count = it->IndicesCount;
				unsigned int offset = it->IndicesStartOffset;

				if (it->Material.Texture) {
					it->Material.Texture->Bind(0);
				}
				s_Data->MaterialUniformBuffer->SetData((void*)&it->Material.BaseColor, sizeof(Vector4));
				RenderCommand::DrawIndexed(DrawMode::Triangles, count, offset);
			}
		}
	}

	void Renderer::Submit(const RenderSubmission& renderSubmission) {
		s_Data->RenderSubmissions.push_back(renderSubmission);
	}

	void Renderer::SetLightingData(const LightingData& lightingData) {
		s_Data->LightingUniformBuffer->SetData((void*)&lightingData, sizeof(LightingData));
	}

	// RendererData

	void Renderer::InitData() {
		//s_Data->StandardShader = Shader::Create();
		s_Data->StandardShader->CompileSourceFromFile("Standard.glsl");

		s_Data->MVPUniformBuffer = UniformBuffer::Create(nullptr, sizeof(Matrix4) * 3);
		s_Data->MaterialUniformBuffer = UniformBuffer::Create(nullptr, sizeof(Vector4));

		LightingData lightingData;
		s_Data->LightingUniformBuffer = UniformBuffer::Create(&lightingData, sizeof(LightingData));
	}
}