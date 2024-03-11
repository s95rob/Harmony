#type vertex
#version 450
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_UV;

layout (std140, binding = 0) uniform MVPData {
    mat4 Model;
    mat4 View;
    mat4 Projection;
};

layout (std140, binding = 1) uniform MaterialData {
    vec4 BaseColor;
};

layout (std140, binding = 2) uniform LightingData {
    vec4 AmbientColor;
    vec3 LightDirection;
};

out VS_OUT {
    vec4 VertexColor;
    vec2 VertexUV;
} vs_out;

void main() {
    gl_Position = Projection * View * Model * vec4(a_Position, 1.0f);

    // Calculate lighting
    vec3 worldNormal = mat3(transpose(inverse(Model))) * a_Normal;
    vec3 diffuseColor = vec3(max(dot(normalize(worldNormal), normalize(-LightDirection)), 0.0f));

    vs_out.VertexColor = vec4((AmbientColor.rgb * AmbientColor.w) + diffuseColor, 1.0f) * 
    vec4(BaseColor.rgb * AmbientColor.w, BaseColor.a);
    vs_out.VertexUV = a_UV;
}


#type fragment
#version 450
out vec4 FragColor;

in VS_OUT {
    vec4 VertexColor;
    vec2 VertexUV;
} fs_in;

uniform sampler2D texture0;

void main() {
    FragColor = fs_in.VertexColor * texture(texture0, fs_in.VertexUV);
}