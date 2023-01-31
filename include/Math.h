/**
 * Math Utility
 * */

#pragma once
#include <cmath>
#include <cstdlib>
#include <glm/glm.hpp>

/* Using Field */
using std::fabs;

using glm::vec3;
using glm::normalize;
using glm::dot;
using glm::cross;

/* Constants Field */
constexpr vec3 RED(1, 0.5, 0.5);
constexpr vec3 GREEN(0.5, 1, 0.5);
constexpr vec3 BLUE(0.5, 0.5, 1);
constexpr vec3 YELLOW(1.0, 1.0, 0.1);
constexpr vec3 CYAN(0.1, 1.0, 1.0);
constexpr vec3 MAGENTA(1.0, 0.1, 1.0);
constexpr vec3 GRAY(0.5, 0.5, 0.5);
constexpr vec3 WHITE(1, 1, 1);

/* Functions Field */
inline glm::vec3 operator*(double t, glm::vec3& v)
{
    return glm::vec3(t * v.x, t * v.y, t * v.z);
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
