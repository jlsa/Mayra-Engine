#shader vertex
#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 u_MVP;
uniform vec3 u_Position;

void main()
{
    TexCoords = texCoords;
    gl_Position = u_MVP * vec4(vertexPosition_modelspace, 1.0);
}

#shader fragment
#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform vec3 u_Color;
uniform sampler2D image;

void main()
{
    color = vec4(u_Color, 1) * texture(image, TexCoords);
}
