#version 450
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_UV;

layout (std140, binding = 0) uniform MVPData {
	mat4 Model;
	mat4 View;
	mat4 Projection;
};

layout (location = 0) out vec2 v_UV;

void main() {
	gl_Position = Projection * View * Model * vec4(a_Position, 0.0f, 1.0f);
	v_UV = a_UV;
}