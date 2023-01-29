/**
* Ray Object
*/

#pragma once
#include "Math.h"
#include "glm/fwd.hpp"
#include <pthread.h>

class Ray
{
public:
    Ray() {}
    Ray(const vec3& origin, const vec3& dir)
    :origin(origin), direction(dir) {}

    vec3 at(const double t)
    {
        return origin + t * direction;
    }

public:
    vec3 origin = vec3(0.0);
    vec3 direction = vec3(0.0);
};
