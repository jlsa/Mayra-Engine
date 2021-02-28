#version 330 core
out vec4 out_color;

uniform vec3 cam_pos;
uniform vec2 screen_size;
uniform mat4 view;

// // based on https://pastebin.com/ynhtfCey
void main()
{
    // camera position and direction
    const float verticalFieldOfView = 60.0 * 3.1415927 / 180.0;
    vec3 ro = cam_pos;
    vec3 rd = normalize(vec3(gl_FragCoord.xy - screen_size.xy / 2.0, screen_size.y * 0.5 / -tan(verticalFieldOfView * 0.5)));
    rd = mat3(view) * rd;

    // sky with haze
    vec3 col = vec3(0.3, 0.55, 0.8) * (1.0 - 0.8 * rd.y) * 0.9;

    // ground
    if (rd.y < 0.0)
        col = vec3(0.42, 0.39, 0.36);

    // horizon/atmospheric perspective
    col = mix(col, vec3(0.7, 0.75, 0.8), pow(1.0 - max(abs(rd.y), 0.0), 24.0));

    if (rd.y >= -0.05)
    {
        float v = smoothstep(-.05, 0.02, rd.y);
        col = mix(col, pow(col, vec3(0.4545)), v);
    }

    out_color = vec4(col, 1.0);
}
