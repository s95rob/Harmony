cmake_minimum_required(VERSION 3.18)

add_subdirectory("extern/glfw")

add_library(ExampleDependencies INTERFACE)
target_link_libraries(ExampleDependencies
INTERFACE
	glfw

	HarmonyAPI
	HarmonyRenderer
)
target_include_directories(ExampleDependencies 
INTERFACE
	"${HARMONY_ROOT_DIR}/HarmonyRenderer/extern/stb"
)

function(DeclareExample ExampleName)
	cmake_parse_arguments("" "" "" "" ${ARGN})

	file(GLOB_RECURSE HARMONY_EXAMPLE_SRCS "*.cpp")
	add_executable(${ExampleName} ${HARMONY_EXAMPLE_SRCS} "../Common/ExampleApp.cpp")
	target_link_libraries(${ExampleName} PUBLIC ExampleDependencies)

	set_target_properties(${ExampleName} 
	PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${ExampleName}>"
	)
	foreach(res ${ARGN})
		add_custom_command(TARGET ${ExampleName} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${res}" "$<TARGET_FILE_DIR:${ExampleName}>/${res}"
		)
	endforeach()

	CompileLocalShaders(${ExampleName})
endfunction()

add_subdirectory("src/01-Cubes" "01-Cubes")
add_subdirectory("src/02-Textures" "02-Textures")