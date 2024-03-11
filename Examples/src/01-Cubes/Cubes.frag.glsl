#version 450
layout (location = 0) out vec4 FragColor;

layout (location = 0) in VS_OUT {
	vec3 VertexColor;
} fs_in;

void main() {
	FragColor = vec4(fs_in.VertexColor, 1.0f);
}