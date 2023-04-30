#define PI 3.1415926

uint gen_seed(vec2 pix, uint width, uint height, uint counter) {
    return (uint(
            uint((pix.x * 0.5 + 0.5) * width)  * uint(1973) + 
            uint((pix.y * 0.5 + 0.5) * height) * uint(9277) + 
            uint(counter) * uint(26699)) 
        | uint(1));
}

uint wang_hash(inout uint seed) {
    seed = uint(seed ^ uint(61)) ^ uint(seed >> uint(16));
    seed *= uint(9);
    seed = seed ^ (seed >> 4);
    seed *= uint(0x27d4eb2d);
    seed = seed ^ (seed >> 15);
    return seed;
}
 
float rand_0_to_1(uint seed) {
    return float(wang_hash(seed)) / 4294967296.0;
}

vec3 sample_hemisphere(uint seed)
{
    float z = rand_0_to_1(seed);
    float r = max(0, sqrt(1.0 - z*z));
    float phi = 2.0 * PI * rand_0_to_1(seed);
    return vec3(r * cos(phi), r * sin(phi), z);
}

vec3 rand_in_hemisphere(vec3 v, vec3 N)
{
    vec3 helper = vec3(1, 0, 0);
    if(abs(N.x) > 0.999) helper = vec3(0, 0, 1);
    vec3 tangent = normalize(cross(N, helper));
    vec3 bitangent = normalize(cross(N, tangent));
    return v.x * tangent + v.y * bitangent + v.z * N;
}