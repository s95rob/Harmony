#ifdef HARMONY_PLATFORM_WINDOWS
	#include <d3d11.h>
	#include <dxgi.h>
	#include <d3dcompiler.h>

	#pragma once
	#pragma comment (lib, "d3d11.lib")
	#pragma comment (lib, "dxgi.lib")
	#pragma comment (lib, "D3DCompiler.lib")
#else
	#error DirectX is not supported on this platform
#endif