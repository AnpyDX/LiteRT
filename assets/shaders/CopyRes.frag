#version 460 core
layout (location = 0) out vec4 fragColor;
layout (location = 0) in vec2 texCoord;

layout (location = 0) uniform sampler2D render_res;

void main()
{
    fragColor = vec4(texture(render_res, texCoord).xyz, 1.0);
}