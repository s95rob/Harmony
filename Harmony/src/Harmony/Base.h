#pragma once

#include <memory>
#include <stdexcept>
#include <cassert>
#include <iostream>

#define interface

#define HARMONY_ASSERT(condition) assert(condition)

#define HARMONY_MAX_INPUT_SLOTS 32

#define HARMONY_MEMORY_PADDING(bytes) uint8_t __harmony_memory_padding[bytes]

namespace Harmony {

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T, typename ... Args>
	constexpr Ref<T> MakeRef(Args&& ... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	constexpr int Bit(uint8_t shift) {
		return 1 << shift;
	}



	enum class API {
		None = 0,
		OpenGL, DX11,
		Auto			// Used only during initialization
	};

	struct InitInfo {
		uint32_t Width, Height;
		API API = API::Auto;
	};

}