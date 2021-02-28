#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    vec4 textureColor = texture(texture_diffuse1, TexCoords);
    if (textureColor.a < 0.5)
    {
        discard;
    }
    FragColor = textureColor;//texture(texture_diffuse1, TexCoords);
}
