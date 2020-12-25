#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;
uniform vec3 u_Position;

void main()
{
    v_TexCoord = texCoord;
    gl_Position = u_MVP * vec4(vertexPosition_modelspace, 1.0);
}
