/**
* Project: LiteRT
* -- Math Utility
*/
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

inline vec3 operator/(vec3 v, double t) {
    return vec3(v.x / t, v.y / t, v.z / t);
}