#pragma once

#include "Harmony/RendererData.h"
#include "Harmony/Camera.h"

#include <vector>

namespace Harmony {

	class Renderer {
	public:
		static void Init();
		static void Close();

		static void Begin(Camera& camera);
		static void End();

		static void Submit(const RenderSubmission& renderSubmission);

		static void SetLightingData(const LightingData& lightingData);

	private:
		static void InitData();

	private:
		static RendererData* s_Data;
	};
}