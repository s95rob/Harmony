#include "../Common.h"
#include "HarmonyAPI.h"
using namespace Harmony;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static Ref<Shader> shader;
static Ref<UniformBuffer> mvpBuffer;
static Ref<Texture> texture;
static Ref<VertexBuffer> vertexBuffer;

static MVPData mvpData;

static float quadVertices[] = {
//	POSITION		UV
// --------------------------------
	-1.0f, 1.0f,	0.0f, 0.0f,
	-1.0f, -1.0f,	0.0f, 1.0f,
	1.0f, 1.0f,		1.0f, 0.0f,

	-1.0f, -1.0f,	0.0f, 1.0f,
	1.0f, 1.0f,		1.0f, 0.0f,
	1.0f, -1.0f,	1.0f, 1.0f
};

EXAMPLE_DECL(Textures);

EXAMPLE_INIT(Textures) {
	Harmony::InitInfo initInfo = {};
	initInfo.Width = APP_WINDOW_WIDTH;
	initInfo.Height = APP_WINDOW_HEIGHT;
	RenderCommand::Init(initInfo);
	RenderCommand::SetViewport(0, 0, APP_WINDOW_SIZE);

	FileReader vertexShader("Textures.vert.glsl");
	FileReader fragmentShader("Textures.frag.glsl");

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
	mvpBuffer = UniformBuffer::Create(&mvpData, sizeof(MVPData));
	mvpBuffer->Bind(0);

	TextureInfo textureInfo;
	uint8_t* data = stbi_load("Logo.png", &textureInfo.Width, &textureInfo.Height, nullptr, 4);
	textureInfo.Data = data;
	textureInfo.Channels = 4;
	textureInfo.NearFilter = TextureFilter::Linear;
	textureInfo.FarFilter = TextureFilter::Linear;
	texture = Texture::Create(textureInfo);
	texture->Bind(0);
	free(data);
	

	VertexBufferInfo vertexBufferInfo;
	vertexBufferInfo.Data = quadVertices;
	vertexBufferInfo.Size = sizeof(quadVertices);
	vertexBuffer = VertexBuffer::Create(vertexBufferInfo);

	VertexLayout layout;
	layout.AddAttrib(Semantic::Position, DataType::Float, 2);
	layout.AddAttrib(Semantic::TexCoord0, DataType::Float, 2);
	vertexBuffer->SetLayout(layout);
	vertexBuffer->Bind();
}

EXAMPLE_UPDATE(Textures) {
	RenderCommand::SetClearColor(0.1f, 0.2f, 0.3f, 1.0f);
	RenderCommand::Clear();

	mvpData.Model = glm::translate(Matrix4(1.0f), Vector3(0.0f, 0.0f, -20.0f))
		* glm::rotate(Matrix4(1.0f), (float)glfwGetTime(), Vector3(0.0f, 1.0f, 0.0f))
		* glm::scale(Matrix4(1.0f), Vector3(10.0f, 5.0f, 1.0f));
	mvpBuffer->SetData(&mvpData, sizeof(MVPData));

	RenderCommand::Draw(DrawMode::Triangles, 6);

	RenderCommand::Present();
}

EXAMPLE_EVENT(Textures, e) {

}