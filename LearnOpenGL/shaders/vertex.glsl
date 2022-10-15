#version 330 core

layout(location = 0) in vec3 position; // 3D position data
layout(location = 1) in vec3 aNormal; // normal vec
layout(location = 2) in vec2 aTexCoords; // ÎÆÀí×ø±ê


out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos =  model * vec4(position, 1.0);
    gl_Position = projection * view * worldPos;
    FragPos = vec3(worldPos);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
}