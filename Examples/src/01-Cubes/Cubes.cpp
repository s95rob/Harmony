#include "../Common.h"
#include "HarmonyAPI.h"
using namespace Harmony;

static bool tripoutMode = false;

static Ref<Shader> shader;
static Ref<VertexBuffer> vertexBuffer;
static Ref<IndexBuffer> indexBuffer;
static Ref<UniformBuffer> mvpBuffer;

constexpr int MaxCubes = 100;
constexpr float CubeOffset = 1.0f;

static MVPData mvpData;

static float cubeVertices[] = {
    //  POSITION            COLOR                  VERTEX INDEX
    // --------------------------------------------------------
    -0.5f, 0.5f, -0.5f,     1.0f, 0.0f, 0.0f,   // 0: Left top rear
    0.5f, 0.5f, -0.5f,      0.0f, 0.0f, 0.0f,   // 1: Right top rear
    0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,   // 2: Right top front
    -0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 0.0f,   // 3: Left top front

    -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 1.0f,   // 4: Left bottom rear
    0.5f, -0.5f, -0.5f,     0.0f, 0.0f, 1.0f,   // 5: Right bottom rear
    0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 1.0f,   // 6: Right bottom front
    -0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f    // 7: Left bottom front
};

static uint32_t cubeIndices[] = {
    3, 2, 1, 1, 0, 3,   // Top
    7, 6, 5, 5, 4, 7,   // Bottom
    4, 7, 3, 3, 0, 4,   // Left
    6, 5, 1, 1, 2, 6,   // Right
    7, 6, 2, 2, 3, 7,   // Front
    1, 5, 4, 4, 0, 1    // Back
};

EXAMPLE_DECL(Cubes);

EXAMPLE_INIT(Cubes) {
    Harmony::InitInfo initInfo = {};
    initInfo.Width = APP_WINDOW_WIDTH;
    initInfo.Height = APP_WINDOW_HEIGHT;
	RenderCommand::Init(initInfo);
	RenderCommand::SetViewport(0, 0, APP_WINDOW_SIZE);

    FileReader vertexShader("Cubes.vert.glsl");
    FileReader fragmentShader("Cubes.frag.glsl");

    ShaderInfo shaderInfo;
    shaderInfo.VertexData = vertexShader.Data();
    shaderInfo.VertexDataSize = vertexShader.Size();
    shaderInfo.FragmentData = fragmentShader.Data();
    shaderInfo.FragmentDataSize = fragmentShader.Size();
    try {
        shader = Shader::Create(shaderInfo);
    }
    catch (std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    shader->Bind();

    mvpData.Projection = glm::perspective(glm::radians(60.0f), 1.0f, 0.3f, 1000.0f);
    mvpData.View = glm::translate(Matrix4(1.0f), Vector3(0.0f, 0.0f, 0.0f));
    mvpBuffer = UniformBuffer::Create(&mvpData, sizeof(MVPData));
    mvpBuffer->Bind(0);

    VertexBufferInfo vertexBufferInfo;
    vertexBufferInfo.Data = cubeVertices;
    vertexBufferInfo.Size = sizeof(cubeVertices);
    vertexBufferInfo.Usage = BufferUsage::Static;

    VertexLayout layout;
    layout.AddAttrib(Semantic::Position, DataType::Float, 3);
    layout.AddAttrib(Semantic::Color0, DataType::Float, 3);

    vertexBuffer = VertexBuffer::Create(vertexBufferInfo);
    vertexBuffer->SetLayout(layout);

    vertexBuffer->Bind();

    IndexBufferInfo indexBufferInfo;
    indexBufferInfo.Data = cubeIndices;
    indexBufferInfo.Count = sizeof(cubeIndices) / sizeof(int);
    indexBufferInfo.Usage = BufferUsage::Static;

    indexBuffer = IndexBuffer::Create(indexBufferInfo);

    indexBuffer->Bind();
}

EXAMPLE_UPDATE(Cubes) {
    RenderCommand::SetClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    RenderCommand::Clear();

    float cubeRotation = (float)glfwGetTime();
    float cubeRotationOffset = 0.0f;
    int maxCubesSquared = sqrtf(MaxCubes);
    for (int y = 0; y < maxCubesSquared; y++) {
        for (int x = 0; x < maxCubesSquared; x++) {
            mvpData.Model = glm::translate(Matrix4(1.0f),
                Vector3(x + (x * CubeOffset) - (maxCubesSquared - CubeOffset), y + (y * CubeOffset) - (maxCubesSquared - CubeOffset), -20.0f))
                * glm::rotate(Matrix4(1.0f), cubeRotation, Vector3(cosf(x), sinf(y), tanf(x + y)));

            if (tripoutMode)
                mvpData.Model = glm::transpose(mvpData.Model);

            mvpBuffer->SetData(&mvpData, sizeof(MVPData));
            RenderCommand::DrawIndexed(DrawMode::Triangles, indexBuffer->GetCount());
        }
    }


    RenderCommand::Present();
}

EXAMPLE_EVENT(Cubes, e) {
    if (e.Type == EVENT_KEYPRESS) {
        switch (e.Params[0]) {
        case GLFW_KEY_SPACE: tripoutMode = !tripoutMode; break;
        }
    }
}
