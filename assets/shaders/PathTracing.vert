#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 0) out vec2 pixCoord;

layout (location = 0) uniform float WIDTH;
layout (location = 1) uniform float HEIGHT;

#define VIEWPORT_HEIGHT 2.0

void main()
{
    gl_Position = vec4(aPos, 1.0);
    float aspect = WIDTH / HEIGHT;
    float viewport_h = VIEWPORT_HEIGHT;
    float viewport_w = viewport_h * aspect;
    pixCoord = vec2(aPos.x * viewport_w, aPos.y * viewport_h);
}