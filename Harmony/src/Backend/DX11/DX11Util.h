#pragma once

#include "Backend/DX11/DX11Include.h"

#include "Harmony/Buffer.h"

namespace Harmony {

	static inline D3D11_USAGE D3D11UsageFromBufferUsage(BufferUsage usage) {
		switch (usage) {
		case BufferUsage::Static: return D3D11_USAGE_IMMUTABLE;
		case BufferUsage::Dynamic: return D3D11_USAGE_DYNAMIC;
		}

		return D3D11_USAGE_DEFAULT;
	}

	static inline DXGI_FORMAT DXGIFormatFromAttrib(const VertexAttrib& attrib) {
		if (attrib.Type == DataType::Float)
			if (attrib.Normalized)
				switch (attrib.GetComponentCount()) {
				case 1: return DXGI_FORMAT_R8_UNORM;
				case 2: return DXGI_FORMAT_R8G8_UNORM;
				case 3: return DXGI_FORMAT_R16G16B16A16_UNORM;
				case 4: return DXGI_FORMAT_R16G16B16A16_UNORM;
				}
			else 
				switch (attrib.GetComponentCount()) {
				case 1: return DXGI_FORMAT_R32_FLOAT;
				case 2: return DXGI_FORMAT_R32G32_FLOAT;
				case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
				case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
			}
		else if (attrib.Type == DataType::Int)
			switch (attrib.GetComponentCount()) {
			case 1: return DXGI_FORMAT_R32_SINT;
			case 2: return DXGI_FORMAT_R32G32_SINT;
			case 3: return DXGI_FORMAT_R32G32B32_SINT;
			case 4: return DXGI_FORMAT_R32G32B32A32_SINT;
			}
		else if (attrib.Type == DataType::Uint)
			switch (attrib.GetComponentCount()) {
			case 1: return DXGI_FORMAT_R32_UINT;
			case 2: return DXGI_FORMAT_R32G32_UINT;
			case 3: return DXGI_FORMAT_R32G32B32_UINT;
			case 4: return DXGI_FORMAT_R32G32B32A32_UINT;
			}
		else if (attrib.Type == DataType::None)
			switch (attrib.GetComponentCount()) {
			case 1: return DXGI_FORMAT_R32_TYPELESS;
			case 2: return DXGI_FORMAT_R32G32_TYPELESS;
			case 3: return DXGI_FORMAT_R32G32B32_TYPELESS;
			case 4: return DXGI_FORMAT_R32G32B32A32_TYPELESS;
			}

		return DXGI_FORMAT_UNKNOWN;
	}

}