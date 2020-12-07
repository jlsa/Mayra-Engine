#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform bool showTexture1;
uniform bool showTexture2;

void main()
{
    if (showTexture1 && showTexture2)
    {
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.5);
    }
    else if (showTexture1 && !showTexture2)
    {
        FragColor = texture(texture1, TexCoord);
    }
    else if (!showTexture1 && showTexture2)
    {
        FragColor = texture(texture2, TexCoord);
    }
    else
    {
        FragColor = vec4(ourColor, 1.0);
    }
}
