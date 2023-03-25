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
        vec3 normal;
        vec3 baseColor;
        vec3 emissive;

        inline void transform(Triangle t)
        {
            p1 = t.p1;
            p2 = t.p2;
            p3 = t.p3;
            normal = t.normal;
            baseColor = t.material.baseColor;
            emissive = t.material.emissive;
        }
    };
}