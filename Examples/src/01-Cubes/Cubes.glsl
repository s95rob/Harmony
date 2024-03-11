#type vertex
#version 450
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Color;

layout (std140, binding = 0) uniform MVPData {
	mat4 Model;
	mat4 View;
	mat4 Projection;
};

out VS_OUT {
	vec3 VertexColor;
} vs_out;

void main() {
	gl_Position = Projection * View * Model * vec4(a_Position, 1.0f);
	vs_out.VertexColor = a_Color;
}

#type fragment
#version 450
out vec4 FragColor;

in VS_OUT {
	vec3 VertexColor;
} fs_in;

void main() {
	FragColor = vec4(fs_in.VertexColor, 1.0f);
}