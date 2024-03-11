#pragma once

#include "Common/ExampleApp.h"
#include "Common/FileReader.h"

#include "HarmonyAPI.h"
using namespace Harmony;

struct MVPData {
	Matrix4 Model = Matrix4(1.0f);
	Matrix4 View = Matrix4(1.0f);
	Matrix4 Projection = Matrix4(1.0f);
};