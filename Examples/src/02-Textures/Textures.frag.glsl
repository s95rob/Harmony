#version 450
layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec2 v_UV;

layout(binding = 0) uniform sampler2D v_Texture;

void main() {

	// Lazy SDF emulation
	vec4 color = texture(v_Texture, v_UV);
	if (color.a < 1.0f)
		color.a = log(color.a * 5.0f);
	else if (color.a < 0.1f)
		discard;

	FragColor = color; 
}