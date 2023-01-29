/**
 * Math Utility
 * */

#pragma once
#include <cmath>
#include <glm/glm.hpp>

inline glm::vec3 operator*(double t, glm::vec3& v)
{
    return glm::vec3(t * v.x, t * v.y, t * v.z);
}
