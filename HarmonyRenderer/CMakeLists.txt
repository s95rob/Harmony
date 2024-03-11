cmake_minimum_required(VERSION 3.18)

file(GLOB_RECURSE HARMONY_RENDERER_SRCS "src/*.cpp" "src/*.h")

add_library(HarmonyRenderer STATIC ${HARMONY_RENDERER_SRCS})
target_link_libraries(HarmonyRenderer INTERFACE HarmonyAPI)
target_include_directories(HarmonyRenderer
PUBLIC
	${HARMONY_RENDERER_INCLUDE_DIR}
	${HARMONY_API_INCLUDE_DIR}

	"extern/glm"
	"extern/cgltf"
	"extern/stb"
)

function(InstallRendererShaders TargetName)
add_custom_command(TARGET ${TargetName} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${HARMONY_ROOT_DIR}/HarmonyRenderer/res/Shaders" "$<TARGET_FILE_DIR:${TargetName}>/Shaders"
)
endfunction()