#version 460 core
layout (location = 0) out vec4 fragColor;
layout (location = 0) in vec2 texCoord;
layout (location = 0) uniform sampler2D tex;

vec3 toneMapping(vec3 color, float limit) {
    float luminance = 0.3 * color.x + 0.6 * color.y + 0.1 * color.z;
    return color * 1.0 / (1.0 +  luminance / limit);
}

void main()
{
    vec3 color = texture(tex, texCoord).xyz;
    color = toneMapping(color, 1.5);
    color = pow(color, vec3(1.0 / 2.2));
    fragColor = vec4(color, 1.0);
}