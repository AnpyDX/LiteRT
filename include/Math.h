/**
 * Math Utility
 * */

#pragma once

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>

/* Using Field */
using std::fabs;
using std::clamp;

using glm::vec3;
using glm::normalize;
using glm::length;
using glm::dot;
using glm::cross;
using glm::reflect;
using glm::refract;

/* Constants Field */
constexpr vec3 RED(1, 0.5, 0.5);
constexpr vec3 GREEN(0.5, 1, 0.5);
constexpr vec3 BLUE(0.5, 0.5, 1);
constexpr vec3 YELLOW(1.0, 1.0, 0.1);
constexpr vec3 CYAN(0.1, 1.0, 1.0);
constexpr vec3 MAGENTA(1.0, 0.1, 1.0);
constexpr vec3 PURPLE(0.8, 0.6, 1.0);
constexpr vec3 GRAY(0.5, 0.5, 0.5);
constexpr vec3 WHITE(1, 1, 1);
constexpr vec3 LIGHT_YELLO(1, 0.9, 0.8);

/* Functions Field */
inline glm::vec3 operator*(double t, glm::vec3 v)
{
    return glm::vec3(t * v.x, t * v.y, t * v.z);
}

inline glm::vec3 operator*(glm::vec3 v, double t)
{
    return t * v;
}

inline glm::vec3 operator/(glm::vec3 v, double t)
{
    return glm::vec3(v.x / t, v.y / t, v.z / t);
}

/* Random Functions */
inline double random_double()
{
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double(); 
}

inline vec3 random_unit_vec3()
{
    vec3 v = vec3(random_double(-1, 1), random_double(-1, 1), random_double(-1, 1));
    return normalize(v);
}
