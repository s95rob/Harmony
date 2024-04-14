<h1 style="text-align: center;">
    <p style="font-size:1.44em;">Harmony</p>
    <p style="font-size:0.77em; font-style: italic;">Graphics API Frontend</p>
</h1>

Graphics API frontend library, encouraging a single rendering codebase for multiple target platforms. 

### Supported Backend API's
+ OpenGL 3.1+
+ DirectX 11

## Shaders
Harmony uses [glslang](https://github.com/KhronosGroup/glslang) and [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross) to target multiple rendering backends using SPIRV binaries. This allows interoperability between glsl and DirectX, and vice-versa. To compile your project's shaders along with its source, use the provided CMake macro: `CompileLocalShaders(TARGET_NAME_HERE)`. This will reflect the relative shader filepaths in your project's binary directory. Shader source file names must follow glslang's shader naming convention. 