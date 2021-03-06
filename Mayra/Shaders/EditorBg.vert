#version 330 core

const vec3 position[6] = vec3[] (
    vec3(-1.0,-1.0, 0.0),
    vec3(-1.0, 1.0, 0.0),
    vec3( 1.0, 1.0, 0.0),
    vec3(-1.0,-1.0, 0.0),
    vec3( 1.0, 1.0, 0.0),
    vec3( 1.0,-1.0, 0.0)
);

void main()
{
    gl_Position = vec4(position[gl_VertexID], 1.0);
}
