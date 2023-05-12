#include "Camera.h"
#include "Console.h"
#include "glm/ext/matrix_transform.hpp"
using namespace LRT;


Camera::Camera(vec3 position, vec3 direction)
: m_matrix(mat4(1.0)), 
m_origin_pos(position), m_current_pos(position),
m_origin_dir(direction)
{}

mat4 Camera::get_matrix() {
    return m_matrix;
}

void Camera::set_speed(float speed) {
    m_speed = speed;
}

void Camera::set_sensitivity(float sensitivity) {
    m_sensitivity = sensitivity;
}

void Camera::update(GLFWwindow *window_ptr)
{
    bool cam_changed = false;

    /* Rotation */
    // TODO
    vec3 dir = m_origin_dir;

    /* Movement */
    if (glfwGetKey(window_ptr, GLFW_KEY_W) == GLFW_PRESS) {
        m_current_pos.z += m_speed * 0.05;
        cam_changed = true;
    }
    if (glfwGetKey(window_ptr, GLFW_KEY_S) == GLFW_PRESS) {
        m_current_pos.z -= m_speed * 0.05;
        cam_changed = true;
    }
    if (glfwGetKey(window_ptr, GLFW_KEY_A) == GLFW_PRESS) {
        m_current_pos.x -= m_speed * 0.05;
        cam_changed = true;
    }
    if (glfwGetKey(window_ptr, GLFW_KEY_D) == GLFW_PRESS) {
        m_current_pos.x += m_speed * 0.05;
        cam_changed = true;
    }

    vec3 step = m_current_pos - m_origin_pos;
    // Console.Log(step);
    
    m_matrix = mat4(
        1.0, 0.0, 0.0, step.x * dir.x + step.x,
        0.0, 1.0, 0.0, step.y * dir.y,
        0.0, 0.0, 1.0, step.z * dir.z,
        0.0, 0.0, 0.0, 1.0
    );

    Console.Log((vec4( 0.0, 0.0, 2.0, 1.0)) * m_matrix);

    if (cam_changed) {
        int w, h;
        glfwGetWindowSize(window_ptr, &w, &h);
        glfwCallFramebufferSizeCallback(window_ptr, w, h);
    }
}