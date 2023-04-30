#define DATA_BLOCK_SIZE 6
#define BVHNODE_SIZE 4

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

struct BVHNode {
    int left, right;
    int n, index;
    vec3 AA, BB;
};

struct HitResult {
    bool is_hit;
    float hit_dis;
    vec3 hit_point;
    vec3 hit_normal;
    vec3 view_dir;
    Material hit_mat;
};

/* Data Unpack Functions */
Triangle getTriangle(samplerBuffer data, int index)
{
    Triangle tri;
    int offset = index * DATA_BLOCK_SIZE;

    // position
    tri.p1 = texelFetch(data, offset + 0).xyz;
    tri.p2 = texelFetch(data, offset + 1).xyz;
    tri.p3 = texelFetch(data, offset + 2).xyz;

    // normal
    tri.normal = texelFetch(data, offset + 3).xyz;

    return tri;
}

Material getMaterial(samplerBuffer data, int index)
{
    Material mat;
    int offset = index * DATA_BLOCK_SIZE;

    mat.baseColor = texelFetch(data, offset + 4).xyz;
    mat.emssive = texelFetch(data, offset + 5).xyz;

    return mat;
}

BVHNode getBVHNode(samplerBuffer data, int index)
{
    BVHNode node;
    int offset = index * BVHNODE_SIZE;

    ivec3 child = ivec3(texelFetch(data, offset + 0).xyz);
    ivec3 leafInfo = ivec3(texelFetch(data, offset + 1).xyz);

    node.left = child.x;
    node.right = child.y;
    node.n = leafInfo.x;
    node.index = leafInfo.y;
    node.AA = texelFetch(data, offset + 2).xyz;
    node.BB = texelFetch(data, offset + 3).xyz;

    return node;
}