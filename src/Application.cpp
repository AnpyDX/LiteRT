#include "Application.h"
#include "GLFW/glfw3.h"
#include <stdexcept>
using namespace LRT;

Application::Application(ApplicationCreateInfo info)
: m_appInfo(info)
{
    initWindow();
    initBackend();
}

Application::~Application()
{
    if (m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
        m_window = nullptr;
    }
}

void Application::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_appInfo.gl_major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_appInfo.gl_minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    m_window = glfwCreateWindow(m_appInfo.width, m_appInfo.height, m_appInfo.title.c_str(), nullptr, nullptr);

    if (m_window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create app window!");
    }
}

void Application::initBackend()
{
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize Backend!");
    }
}