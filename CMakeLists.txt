cmake_minimum_required(VERSION 3.18)

add_subdirectory("tools/glslang")

option(HARMONY_API_ONLY "Build HarmonyAPI, nothing else" OFF)
option(HARMONY_BUILD_EXAMPLES "Build Harmony examples" ON)

set(HARMONY_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HARMONY_API_INCLUDE_DIR "${HARMONY_ROOT_DIR}/HarmonyAPI/src")


project(Harmony)
add_subdirectory("HarmonyAPI")

if (NOT HARMONY_API_ONLY)
	set(HARMONY_RENDERER_INCLUDE_DIR "${HARMONY_ROOT_DIR}/HarmonyRenderer/src")
	add_subdirectory("HarmonyRenderer")

	if (HARMONY_BUILD_EXAMPLES)
		add_subdirectory("Examples")
	endif()
endif()
