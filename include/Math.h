/**
* Project: LiteRT
* -- Math Utility
*/
#pragma once
#include <glm/glm.hpp>

using glm::vec3;

inline vec3 operator/(vec3 v, double t) {
    return vec3(v.x / t, v.y / t, v.z / t);
}