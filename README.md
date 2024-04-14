<h1 style="text-align: center;">
    <p style="font-size:1.44em;">Harmony</p>
    <p style="font-size:0.77em; font-style: italic;">Graphics API Frontend + Renderer</p>
</h1>

C++11 graphics API frontend library, encouraging a single rendering codebase for multiple target platforms. 

### Supported Backend API's
+ OpenGL 3.1+
+ DirectX 11

## Usage
Clone the repository (with submodules) and let CMake configure the build system:
```bash
git clone --recursive https://github.com/s95rob/Harmony
cd Harmony
cmake -S . -B build
```

From here you may build the examples, or include Harmony in your project (using CMake):

```cmake
add_subdirectory(".../Harmony")
```

### CMake Options
There are a few options available to set during configuration:

`HARMONY_API_ONLY` : Build only the frontend API. Setting this `TRUE` will omit building the renderer and examples. 
`HARMONY_BUILD_EXAMPLES` : Build the examples. Settings this `FALSE` omits the examples from being built. 

## Shaders
Harmony uses [glslang](https://github.com/KhronosGroup/glslang) and [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross) to target multiple rendering backends using SPIRV binaries. This allows interoperability between glsl and DirectX, and vice-versa. To compile your project's shaders along with its source, use the provided CMake macro: `CompileLocalShaders(TARGET_NAME_HERE)`. This will reflect the shader names (and relative paths) in your project's binary directory, for convenience. Text shaders must follow glslang's shader naming convention. 