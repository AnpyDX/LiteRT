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
    Ray(const glm::vec3& origin, const glm::vec3& dir)
    :origin(origin), direction(dir) {}

    glm::vec3 at(const double t)
    {
        return origin + t * direction;
    }

public:
    glm::vec3 origin = glm::vec3(0.0);
    glm::vec3 direction = glm::vec3(0.0);
};
