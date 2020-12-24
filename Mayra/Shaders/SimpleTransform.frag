#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform vec3 u_Color;
uniform sampler2D u_Image;

void main()
{
    color = vec4(u_Color, 1) * texture(u_Image, TexCoords);
}
