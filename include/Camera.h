/**
* Camera Object
*/

#pragma once
#include "Math.h"
#include "Ray.h"

class Camera
{
public:
    Camera() = delete;
    Camera(vec3 pos, vec3 front, vec3 up, double aspect_ratio)
    : position(pos), front(front), up(normalize(up))
    {
        viewport_width = aspect_ratio * viewport_height;
        right = normalize(cross(front, up));
        start_point = position + front + up * viewport_height / 2 - right * viewport_width / 2;
    }

    Ray getRay(double u, double v)
    {
        vec3 screen_coord = start_point + u * right - v * up;
        return Ray(position, screen_coord - position);
    }

public:
    vec3 position = vec3(0.0);
    double viewport_height = 1.0;
    double viewport_width = 1.0;
    vec3 front = vec3(0.0, 0.0, -1.0);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = vec3(1.0, 0.0, 0.0);
    vec3 start_point = vec3(0.0);
};