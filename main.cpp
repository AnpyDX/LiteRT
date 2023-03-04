/**
* Lite RT Projects
* -- Main
*/

#include <stdexcept>
#include "obj_loader.h"
#include "Console.h"
#include "Application.h"
#include "Buffer.h"
#include "DataTypes.h"
using namespace LRT;

class LRT_APP : public Application
{
private:
    Buffer* m_data_buffer;

public:
    LRT_APP(ApplicationCreateInfo info): Application(info) {}
    ~LRT_APP()
    {
        delete m_data_buffer;
    }

    void init()
    {
        // set resize callback
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)->void {
            glViewport(0, 0, width, height);
        });
    }

    void genBuffer()
    {
        /* Scene Data */
        OBJ_Object model = obj_loader("/home/anpyd/Workspace/RayTracing/assets/models/bunny.obj");
        InternalTypes::Material mat;
        mat.baseColor = glm::vec3(0.5);
        mat.emissive = glm::vec3(0.0);
        
        /* Generate Encoded Data */
        std::vector<InternalTypes::Triangle_encoded> encode_data;
        for (auto f : model.faces) {
            InternalTypes::Triangle_encoded e_tri;
            InternalTypes::Triangle tri;
            e_tri.add_triangle(tri);
            e_tri.add_material(mat);
            encode_data.push_back(e_tri);
        }

        /* Create Buffer */
        m_data_buffer = new Buffer((void*)encode_data.data(), encode_data.size() * sizeof(InternalTypes::Triangle_encoded), GL_RGB32F);
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(m_window))
        {
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }

    void run()
    {
        init();
        genBuffer();
        mainLoop();
    }
};


int main()
{
    ApplicationCreateInfo createInfo {
        .title = "Lite RT",
        .gl_major_version = 4,
        .gl_minor_version = 5,
    };

    try {
        auto app = LRT_APP(createInfo);
        app.run();
    }
    catch (std::exception& e) {
        Console.Warn(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}