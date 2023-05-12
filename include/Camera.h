/**
* Project: LiteRT
* -- Camera Object
*/

#pragma once
#include <GLFW/glfw3.h>
#include "MathUtility.h"

namespace LRT {
    class Camera
    {
    public:
        Camera() = default;
        Camera(vec3 position, vec3 direction);

        /* Get Transfer Matrix (for viewprot coords) */
        mat4 get_matrix();

        /* Controller setting */
        void set_speed(float speed);
        void set_sensitivity(float sensitivity);

        /* Update Camera pos & matrix */
        void update(GLFWwindow* window_ptr);

    private:
        mat4 m_matrix;
        vec3 m_origin_pos;
        vec3 m_current_pos;
        vec3 m_origin_dir;
        float m_yaw;
        float m_pitch;
        float m_speed = 1.0;
        float m_sensitivity = 1.0;
    };
}