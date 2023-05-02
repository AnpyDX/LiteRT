#version 460 core
#extension GL_GOOGLE_include_directive : enable

#include "include/Data.h"
#include "include/Utility.h"

#define INFINITY 1000000
#define MAX_BOUNCE 2

layout (location = 0) out vec4 fragColor;
layout (location = 0) in vec2 pixCoord;
layout (location = 1) in vec2 texCoord;

layout (location = 0) uniform sampler2D last_frame;
layout (location = 1) uniform int WIDTH;
layout (location = 2) uniform int HEIGHT;
layout (location = 3) uniform samplerBuffer TRI_DATA;
layout (location = 4) uniform samplerBuffer BVH_DATA;
layout (location = 5) uniform uint counter;


/* Helper Functions */
HitResult hitTriangle(Triangle tri, Ray ray)
{
    HitResult res;
    res.is_hit = false;
    res.hit_dis = INFINITY;

    vec3 S = ray.start;
    vec3 D = ray.dir;
    vec3 N = normalize(cross(tri.p1 - tri.p3, tri.p2 - tri.p1));
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
    res.view_dir = D;

    return res;
}

float hitAABB(Ray ray, vec3 AA, vec3 BB)
{
    vec3 invdir = 1.0 / ray.dir;
    vec3 a = (AA - ray.start) * invdir;
    vec3 b = (BB - ray.start) * invdir;
    vec3 tmax = max(a, b);
    vec3 tmin = min(a, b);
    float t_in = max(tmin.x, max(tmin.y, tmin.z));
    float t_out = min(tmax.x, min(tmax.y, tmax.z));

    return (t_in <= t_out) ? ((t_in > 0.0) ? t_in : t_out) : -1.0;
}

HitResult hitArray(Ray ray, int begin, int end)
{
    HitResult res;
    res.is_hit = false;
    res.hit_dis = INFINITY;

    for (int i = begin; i <= end; i++)
    {
        Triangle tri = getTriangle(TRI_DATA, i);
        HitResult t_res = hitTriangle(tri, ray);
        if (t_res.is_hit && res.hit_dis > t_res.hit_dis) {
            res = t_res;
            res.hit_mat = getMaterial(TRI_DATA, i);
        }
    }

    return res;
}

HitResult hitBVH(Ray ray)
{
    HitResult res;
    res.is_hit = false;
    res.hit_dis = INFINITY;

    int stack[256];
    int sp = 0;
    stack[sp++] = 0;

    while (sp > 0)
    {
        int top = stack[--sp];
        BVHNode node = getBVHNode(BVH_DATA, top);

        /* Leaf Node */
        if (node.n > 0) {
            int R_index = node.index + node.n - 1;
            HitResult t_res = hitArray(ray, node.index, R_index);
            if (t_res.is_hit && t_res.hit_dis < res.hit_dis) {
                res = t_res;
            }
            continue;
        }

        /* Normal Node */
        BVHNode left = getBVHNode(BVH_DATA, node.left);
        BVHNode right = getBVHNode(BVH_DATA, node.right);
        float d_l = hitAABB(ray, left.AA, left.BB);
        float d_r = hitAABB(ray, right.AA, right.BB);

        
        if (d_l > 0.0 && d_r > 0.0) {
            if (d_l < d_r) {
                stack[sp++] = node.right;
                stack[sp++] = node.left;
            }
            else {
                stack[sp++] = node.left;
                stack[sp++] = node.right;
            }
        }
        // only hit left
        else if (d_l > 0.0) {
            stack[sp++] = node.left;
        }
        // only hit right
        else if (d_r > 0.0) {
            stack[sp++] = node.right;
        }
    }

    return res;
}

vec3 pathTracing(HitResult res, int max_bounce, vec2 pix)
{
    vec3 Lo = vec3(0.0);
    vec3 history_Lo = vec3(1.0);

    uint seed = gen_seed(pix, WIDTH, HEIGHT, counter);

    for (int i = 0; i < max_bounce; i++)
    {
        vec3 wi = rand_in_hemisphere(sample_hemisphere(seed), res.hit_normal);

        Ray random_ray;
        random_ray.start = res.hit_point;
        random_ray.dir = wi;
        HitResult new_hit_res = hitBVH(random_ray);

        if (!new_hit_res.is_hit) break;

        float pdf = 1.0 / (2.0 * PI);
        float cosine_i = max(0.0, dot(random_ray.dir, res.hit_normal));
        vec3 f_r = res.hit_mat.baseColor / PI;

        vec3 Le = new_hit_res.hit_mat.emssive;
        Lo += history_Lo * Le * f_r * cosine_i / pdf;

        res = new_hit_res;
        history_Lo *= f_r * cosine_i / pdf;
    }

    return Lo;
}

void main()
{
    float x_coord = (pixCoord.x);
    float y_coord = (pixCoord.y);

    Ray ray;
    vec3 srceen_coord = vec3(x_coord, y_coord, 1.0);
    ray.start = vec3(0.0, 0.8, 2.0);
    ray.dir = normalize(srceen_coord - ray.start);

    HitResult res = hitBVH(ray);
    vec3 render_color;

    if (res.is_hit) {
        vec3 Le = res.hit_mat.emssive;
        vec3 Li = pathTracing(res, MAX_BOUNCE, vec2(x_coord, y_coord));
        render_color = Le + Li;
    }
    else {
        render_color = vec3(0.0);
    }

    // Blend with last frame
    vec3 last_color = texture(last_frame, texCoord).xyz;
    vec3 mix_color = mix(last_color, render_color, 1.0 / (counter + 1.0));
    fragColor = vec4(mix_color, 1.0);
}