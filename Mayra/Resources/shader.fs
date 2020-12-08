#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform bool showTexture1;
uniform bool showTexture2;
uniform bool flipSmile;

uniform float mixPercentage;

void main()
{
    vec2 smileTexCoord;
    if (flipSmile) {
        smileTexCoord = vec2(1.0 - TexCoord.x, TexCoord.y);
    } else {
        smileTexCoord = TexCoord;
    }

    if (showTexture1 && showTexture2)
    {
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, smileTexCoord), mixPercentage);
    }
    else if (showTexture1 && !showTexture2)
    {
        FragColor = texture(texture1, TexCoord);
    }
    else if (!showTexture1 && showTexture2)
    {
        FragColor = texture(texture2, smileTexCoord);
    }
    else
    {
        FragColor = vec4(ourColor, 1.0);
    }
}
