#version 330 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D u_Image;
uniform vec3 u_Color;

void main()
{
    color = vec4(u_Color, 1.0) * texture(u_Image, TexCoords);
}
