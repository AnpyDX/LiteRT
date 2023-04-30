/**
* Project: LiteRT
* -- BVH Boost Type
*/

#pragma once
#include <vector>
#include <algorithm>
#include "Math.h"
#include "DataTypes.h"

using glm::min;
using glm::max;

struct BVHNode {
    int left, right;
    int n, index;
    glm::vec3 AA, BB;
};

struct _BVH_Triangle
{
    InternalTypes::Triangle tri;
    InternalTypes::Material mat;
};

/* Helper Functions */
namespace {
    inline bool cmpx(const _BVH_Triangle& t1, const _BVH_Triangle& t2) {
        return t1.tri.center.x > t2.tri.center.x;
    }
    inline bool cmpy(const _BVH_Triangle& t1, const _BVH_Triangle& t2) {
        return t1.tri.center.y > t2.tri.center.y;
    }
    inline bool cmpz(const _BVH_Triangle& t1, const _BVH_Triangle& t2) {
        return t1.tri.center.z > t2.tri.center.z;
    }
}


/** Build BVH Tree
    return -> index of root node
 */
inline int buildBVH(std::vector<_BVH_Triangle>& tris, std::vector<BVHNode>& nodes, int b, int e, int n)
{
    if (b > e) return 0;

    /* create new BVH Node */
    nodes.push_back(BVHNode());
    int index = nodes.size() - 1;
    nodes[index].AA = vec3(10000000.0);
    nodes[index].BB = vec3(-10000000.0);

    /* caculate certain node AABB */
    for (int i = b; i <= e; i++)
    {
        // AA (min coord)
        float min_x = min(tris[i].tri.p1.x, min(tris[i].tri.p2.x, tris[i].tri.p3.x));
        float min_y = min(tris[i].tri.p1.y, min(tris[i].tri.p2.y, tris[i].tri.p3.y));
        float min_z = min(tris[i].tri.p1.z, min(tris[i].tri.p2.z, tris[i].tri.p3.z));
        nodes[index].AA.x = min(nodes[index].AA.x, min_x);
        nodes[index].AA.y = min(nodes[index].AA.y, min_y);
        nodes[index].AA.z = min(nodes[index].AA.z, min_z);

        // BB (max coord)
        float max_x = max(tris[i].tri.p1.x, max(tris[i].tri.p2.x, tris[i].tri.p3.x));
        float max_y = max(tris[i].tri.p1.y, max(tris[i].tri.p2.y, tris[i].tri.p3.y));
        float max_z = max(tris[i].tri.p1.z, max(tris[i].tri.p2.z, tris[i].tri.p3.z));
        nodes[index].BB.x = max(nodes[index].BB.x, max_x);
        nodes[index].BB.y = max(nodes[index].BB.y, max_y);
        nodes[index].BB.z = max(nodes[index].BB.z, max_z);
    }

    /* rest number of triangles less than n, end building */
    if ((e - b + 1) <= n) {
        nodes[index].index = b;
        nodes[index].n = e - b + 1;
        return index;
    }

    /* build BVH Tree */
    float len_x = nodes[index].BB.x - nodes[index].AA.x;
    float len_y = nodes[index].BB.y - nodes[index].AA.y;
    float len_z = nodes[index].BB.z - nodes[index].AA.z;

    if (len_x >= len_y && len_x >= len_z) {
        std::sort(tris.begin() + b, tris.begin() + e + 1, cmpx);
    }
    else if (len_y >= len_x && len_y >= len_z) {
        std::sort(tris.begin() + b, tris.begin() + e + 1, cmpy);
    }
    else if (len_z >= len_x && len_z >= len_y) {
        std::sort(tris.begin() + b, tris.begin() + e + 1, cmpz);
    }

    int mid = (b + e) / 2;
    int left = buildBVH(tris, nodes, b, mid, n);
    int right = buildBVH(tris, nodes, mid + 1, e, n);

    nodes[index].left = left;
    nodes[index].right = right;

    return index;
}