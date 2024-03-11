#type vertex
#version 450
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_UV;

layout (std140, binding = 0) uniform MVPData {
	mat4 Model;
	mat4 View;
	mat4 Projection;
};

out vec2 v_UV;

void main() {
	gl_Position = Projection * View * Model * vec4(a_Position, 0.0f, 1.0f);
	v_UV = a_UV;
}

#type fragment
#version 450
out vec4 FragColor;
in vec2 v_UV;

uniform sampler2D v_Texture;

void main() {

	// Lazy SDF emulation
	vec4 color = texture(v_Texture, v_UV);
	if (color.a < 1.0f)
		color.a = log(color.a * 5.0f);
	else if (color.a < 0.1f)
		discard;

	FragColor = color; 
}