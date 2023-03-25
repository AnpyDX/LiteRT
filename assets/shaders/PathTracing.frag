#version 330 core

#define INFINITY 19999999
// 单数据块长度
#define DATA_BLOCK_SIZE 6

out vec4 fragColor;
in vec2 pixCoord;

uniform samplerBuffer Data;
uniform int triangle_num;

/* Types definition */
struct Ray {
    vec3 start;
    vec3 dir;
};

struct Triangle {
    vec3 p1, p2, p3;
    vec3 normal;
};

struct Material {
    vec3 baseColor;
    vec3 emssive;
};

struct HitResult {
    bool is_hit;
    float hit_dis;
    vec3 hit_point;
    vec3 hit_normal;
    Material hit_mat;
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

/* Helper Functions */
HitResult hitTriangle(Triangle tri, Ray ray)
{
    HitResult res;
    res.is_hit = false;
    res.hit_dis = INFINITY;

    vec3 S = ray.start;
    vec3 D = ray.dir;
    vec3 N = normalize(cross(tri.p1 - tri.p3, tri.p2 - tri.p1));
    vec3 n = N;
    if (dot(N, D) > 0.0) N = -N;

    if (abs(dot(N, D)) < 0.00001) return res;

    float t = (dot(N, tri.p1) - dot(S, N)) / dot(D, N);
    if (t < 0.00001) return res;

    vec3 P = S + D * t;

    vec3 c1 = normalize(cross(tri.p2 - tri.p1, P - tri.p1));
    vec3 c2 = normalize(cross(tri.p3 - tri.p2, P - tri.p2));
    vec3 c3 = normalize(cross(tri.p1 - tri.p3, P - tri.p3));
    if (dot(c1, N) < 0.9 || dot(c2, N) < 0.9 || dot(c3, N) < 0.9) return res;

    res.is_hit = true;
    res.hit_dis = t;
    res.hit_point = P;
    res.hit_normal = N;

    return res;
}

HitResult hitScene(Ray ray)
{
    HitResult res;
    res.is_hit = false;
    res.hit_dis = INFINITY;

    for (int i = 0; i < triangle_num; i++)
    {
        Triangle tri = getTriangle(i);
        HitResult r = hitTriangle(tri, ray);
        if (r.is_hit && r.hit_dis < res.hit_dis) {
            res = r;
            res.hit_mat = getMaterial(i);
        }
    }

    return res;
}


void main()
{
    float x_coord = (pixCoord.x);
    float y_coord = (pixCoord.y);

    Ray ray;
    vec3 srceen_coord = vec3(x_coord, y_coord, 1.0);
    ray.start = vec3(0.0, 0.0, 2.0);
    ray.dir = normalize(srceen_coord - ray.start);

    HitResult res = hitScene(ray);
    if (res.is_hit) {
        fragColor = vec4(res.hit_mat.baseColor, 1.0);
    }
    else {
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}