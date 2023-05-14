#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 0) out vec2 pixCoord;
layout (location = 1) out vec2 viewPortCoord;
layout (location = 2) out vec2 texCoord;

layout (location = 1) uniform float WIDTH;
layout (location = 2) uniform float HEIGHT;

#define VIEPORT_HEIGHT 2.0 // FIXME use uniform

void main()
{
    gl_Position = vec4(aPos, 1.0);
    
    float aspect = WIDTH / HEIGHT;
    float VIEPORT_WIDTH = aspect * VIEPORT_HEIGHT;
    viewPortCoord = vec2(aPos.x * VIEPORT_WIDTH / 2, aPos.y * VIEPORT_HEIGHT / 2);

    pixCoord = aPos.xy;
    texCoord = aTexCoord;
}