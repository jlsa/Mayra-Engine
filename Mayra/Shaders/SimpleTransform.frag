#version 330 core
in vec2 v_TexCoord;

out vec4 color;

uniform vec3 u_Color;
uniform sampler2D u_Texture;

void main()
{
    vec4 texColor = vec4(u_Color, 1) * texture(u_Texture, v_TexCoord);
    color = texColor;
}
