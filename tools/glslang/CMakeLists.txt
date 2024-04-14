cmake_minimum_required(VERSION 3.20)

project(glslang)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_subdirectory("SPIRV")
add_subdirectory("glslang")
add_subdirectory("StandAlone")

function (CompileLocalShaders TargetName)
    file(GLOB_RECURSE SHADER_SRCS LIST_DIRECTORIES false RELATIVE "" "*.vert" "*.frag" "*.vert.glsl" "*.frag.glsl")
    foreach(SHADER_SRC ${SHADER_SRCS})
        set(RELATIVE_SHADER_SRC "")
        string(REPLACE ${CMAKE_CURRENT_SOURCE_DIR} "" RELATIVE_SHADER_SRC "${SHADER_SRC}")
        add_custom_command(TARGET ${TargetName} POST_BUILD
            COMMAND $<TARGET_FILE:glslang-standalone> "${SHADER_SRC}" -o "$<TARGET_FILE_DIR:${TargetName}>${RELATIVE_SHADER_SRC}" -V
        )
    endforeach()
endfunction()