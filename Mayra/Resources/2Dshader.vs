#version 330 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
out vec3 v_Position;

uniform mat4 u_ViewProjection;

void main()
{
    TexCoords = texCoords;
    gl_Position = u_ViewProjection * vec4(vertex, 1.0);
}
