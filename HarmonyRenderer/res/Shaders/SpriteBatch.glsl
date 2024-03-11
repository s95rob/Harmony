#type vertex
#version 450
layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_UV;
layout (location = 3) in float a_TextureSlot;

out vec4 v_Color;
out vec2 v_UV;
out float v_TextureSlot;

layout (std140, binding = 0) uniform MVPData {
    mat4 View;
    mat4 Projection;
};

void main() {
    gl_Position = Projection * vec4(a_Position.xy, 0.0f, 1.0f);
    v_Color = a_Color;
    v_UV = a_UV;
    v_TextureSlot = a_TextureSlot;
}

#type fragment
#version 450
out vec4 FragColor;

in vec4 v_Color;
in vec2 v_UV;
in float v_TextureSlot;

layout (binding = 0) uniform sampler2D textures[17];

void main() {
    int textureIndex = int(v_TextureSlot);
    FragColor = texture(textures[textureIndex], v_UV) * v_Color;
}