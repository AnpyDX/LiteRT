/**
* Project: LiteRT
* -- Application Object
*/

#pragma once
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace LRT
{
    struct ApplicationCreateInfo
    {
        /* Window Configure */
        std::string title = "LiteRT Application";
        uint32_t width = 800;
        uint32_t height = 600;
        
        /* Backend Configure */
        int gl_major_version = 3;
        int gl_minor_version = 3;
    };


    class Application
    {
    public:
        Application() = default;
        Application(ApplicationCreateInfo info);
        ~Application();

    private:
        void initWindow();
        void initBackend();

    public:
        ApplicationCreateInfo m_appInfo;
        GLFWwindow* m_window;
    };

}
