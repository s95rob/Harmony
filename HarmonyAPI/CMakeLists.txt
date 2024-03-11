cmake_minimum_required(VERSION 3.18)

add_subdirectory("extern/SPIRV-Cross")

set(HARMONY_EXTERN_SRCS "extern/glad/src/glad.c")
set(HARMONY_EXTERN_INCLUDE_DIRS 
    "extern/SPIRV-Cross"    

    "extern/glad/include" 
)
file(GLOB_RECURSE HARMONY_LIB_SRCS "src/*.cpp" "src/*.h")

add_library(HarmonyAPI STATIC ${HARMONY_LIB_SRCS} ${HARMONY_EXTERN_SRCS})
target_include_directories(HarmonyAPI 
PRIVATE
    ${HARMONY_EXTERN_INCLUDE_DIRS}
PUBLIC
    "src/"
)
target_link_libraries(HarmonyAPI
INTERFACE
    spirv-cross-glsl
    spirv-cross-hlsl
)
set_property(TARGET HarmonyAPI PROPERTY CXX_STANDARD 11)
set_property(TARGET HarmonyAPI PROPERTY CXX_STANDARD_REQUIRED true)
add_dependencies(HarmonyAPI
    glslang-standalone
    spirv-cross-glsl
    spirv-cross-hlsl
)

if (WIN32)
    target_compile_definitions(HarmonyAPI PUBLIC HARMONY_PLATFORM_WINDOWS)
endif (WIN32)