/**
* Lite RT Projects
* -- Main
*/

#include <pthread.h>
#include <stdexcept>
#include "obj_loader.h"
#include "Console.h"
#include "Application.h"
#include "Buffer.h"
#include "DataTypes.h"
#include "ShaderProgram.h"
using namespace LRT;

class LRT_APP : public Application
{
private:
    Buffer* m_data_buffer;
    ShaderProgram* m_shader;
    GLuint VAO, VBO;
    uint32_t m_face_num;

public:
    LRT_APP(ApplicationCreateInfo info): Application(info) {}
    ~LRT_APP()
    {
        if (m_data_buffer != nullptr) delete m_data_buffer;
        if (m_shader != nullptr) delete m_shader;
        m_data_buffer = nullptr;
        m_shader = nullptr;
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
        OBJ_Object model = obj_loader("/home/anpyd/Workspace/RayTracing/assets/models/box.obj");
        if (model.normals.empty()) throw std::runtime_error("Model has no normals!");
        m_face_num = model.faces.size();
        
        /* Generate Encoded Data */
        std::vector<InternalTypes::Triangle_encoded> encode_data;
        for (auto f : model.faces) {
            InternalTypes::Triangle_encoded e_tri{};
            InternalTypes::Triangle tri{};
            InternalTypes::Material mat{};
            mat.baseColor = vec3(1.0, 0.2, 0.3);
            mat.emissive = vec3(0.0);
            tri.p1 = model.vertices[f.vertex_indices[0]];
            tri.p2 = model.vertices[f.vertex_indices[1]];
            tri.p3 = model.vertices[f.vertex_indices[2]];
            tri.normal = model.normals[f.normal_indices[0]];
            tri.material = mat;
            e_tri.transform(tri);
            encode_data.push_back(e_tri);
        }

        /* Create Buffer */
        m_data_buffer = new Buffer((void*)encode_data.data(), m_face_num * sizeof(glm::vec3) * 6, GL_RGB32F);
    }

    void mainLoop()
    {
        float vertices[] = {
            -1.0, 1.0, 0.0,
            -1.0, -1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            -1.0, -1.0, 0.0,
            1.0, -1.0, 0.0
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        m_shader = new ShaderProgram("/home/anpyd/Workspace/RayTracing/assets/shaders/PathTracing");
        
        while (!glfwWindowShouldClose(m_window))
        {
            processInput();
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shader->use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_BUFFER, m_data_buffer->getID());
            int w, h;
            glfwGetWindowSize(m_window, &w, &h);
            m_shader->setFloat("WIDTH", w);
            m_shader->setFloat("HEIGHT", h);
            m_shader->setInt("Data", 0);
            m_shader->setInt("triangle_num", m_face_num);
            glDrawArrays(GL_TRIANGLES, 0, 6);


            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }

    void processInput()
    {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(m_window, true);
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
        .gl_major_version = 3,
        .gl_minor_version = 3,
    };

    try {
        auto app = new LRT_APP(createInfo);
        app->run();
        delete app;
    }
    catch (std::exception& e) {
        Console.Warn(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}