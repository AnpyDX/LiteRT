#version 330 core

// 单数据块长度
#define DATA_BLOCK_SIZE 6

out vec4 fragColor;

uniform samplerBuffer Data;
uniform int triangle_num;

/* Types definition */
struct Triangle {
    vec3 p1, p2, p3;
    vec3 normal;
};

struct Material {
    vec3 baseColor;
    vec3 emssive;
};


/* Parse data */
Triangle getTriangle(int index)
{
    Triangle tri;
    int offset = index * DATA_BLOCK_SIZE;

    // position
    tri.p1 = texelFetch(Data, offset + 0).xyz;
    tri.p2 = texelFetch(Data, offset + 1).xyz;
    tri.p3 = texelFetch(Data, offset + 2).xyz;

    // normal
    tri.normal = texelFetch(Data, offset + 3).xyz;

    return tri;
}

Material getMaterial(int index)
{
    Material mat;
    int offset = index * DATA_BLOCK_SIZE;

    mat.baseColor = texelFetch(Data, offset + 4).xyz;
    mat.emssive = texelFetch(Data, offset + 5).xyz;

    return mat;
}


void main()
{
    Triangle tri = getTriangle(10);
    Material mat = getMaterial(10);

    fragColor = vec4(tri.p1, 1.0);
}