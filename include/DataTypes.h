/**
* Internal Types
*/

#pragma once
#include <glm/glm.hpp>

using vec3 = glm::vec3;

namespace InternalTypes
{
    struct Material
    {
        vec3 emissive = vec3(0.0);
        vec3 baseColor = vec3(0.0);
    };

    struct Triangle
    {
        vec3 p1, p2, p3;
        vec3 normal;
        Material material;
    };

    struct Triangle_encoded
    {
        vec3 p1, p2, p3;
        vec3 n1, n2, n3;
        vec3 emissive;
        vec3 baseColor;

        inline void add_triangle(Triangle t)
        {
            p1 = t.p1;
            p2 = t.p2;
            p3 = t.p3;
            n1 = t.normal;
            n2 = t.normal;
            n3 = t.normal;
        }
        inline void add_material(Material mat)
        {
            baseColor = mat.baseColor;
            emissive = mat.emissive;
        }
    };
}