/**
* Project: LiteRT
* -- Internal Types (types that will be encoded)
*/

#pragma once
#include "Math.h"

using vec3 = glm::vec3;

namespace LRT::InternalTypes
{
    struct Material
    {
        vec3 emissive = vec3(0.0);
        vec3 baseColor = vec3(1.0);
    };

    struct Triangle
    {
        vec3 p1, p2, p3;
        vec3 normal;
        vec3 center;
    };

    struct Triangle_encoded
    {
        vec3 p1, p2, p3;
        vec3 normal;
        vec3 baseColor;
        vec3 emissive;

        inline void set_tri(Triangle tri) {
            p1 = tri.p1;
            p2 = tri.p2;
            p3 = tri.p3;
            normal = tri.normal;
        }

        inline void set_mat(Material mat) {
            baseColor = mat.baseColor;
            emissive = mat.emissive;
        }
    };

    struct BVH_encoded {
        vec3 childs; // (left, right, NULL)
        vec3 leafInfo; // (n, index, NULL)
        vec3 AA, BB;
    };
}