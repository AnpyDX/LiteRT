/**
* Lite RT Projects
* -- Main
*/

#include <cstdint>
#include <stdexcept>
#include "glm/common.hpp"
#include "obj_loader.h"
#include "Console.h"
#include "Application.h"
#include "Buffer.h"
#include "DataTypes.h"
#include "ShaderProgram.h"
#include "SceneManager.h"
using namespace LRT;

class LRT_APP : public Application
{
private:
    Buffer* m_data_buffer;
    Buffer* m_BVH_buffer;
    SceneManager* m_scene;
    ShaderProgram* m_shader;
    GLuint VAO, VBO;
    uint32_t m_face_num;
    uint32_t m_bvh_node_num;

public:
    LRT_APP(ApplicationCreateInfo info): Application(info) {
        init();
        readScene();
        genDataBuffer();
    }
    
    ~LRT_APP() {
        if (m_shader != nullptr) delete m_shader;
        if (m_BVH_buffer != nullptr) delete m_BVH_buffer;
        if (m_data_buffer != nullptr) delete m_data_buffer;
        if (m_scene != nullptr) delete m_scene;

        m_scene = nullptr;
        m_data_buffer = nullptr;
        m_shader = nullptr;

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void init()
    {
        // set resize callback
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)->void {
            glViewport(0, 0, width, height);
        });
    }

    void readScene()
    {
        m_scene = new SceneManager();
        m_scene->enable_BVH = true;
        /* Scene Data */
        OBJ_Object bunny_obj = obj_loader("/home/anpyd/Workspace/RayTracing/assets/models/bunny.obj");

        Model bunny_model {};
        InternalTypes::Material mat {};
        mat.baseColor = vec3(1.0);
        mat.emissive = vec3(0.0);
        
        bunny_model.add_obj_data(bunny_obj);
        bunny_model.material = mat;

        m_scene->add_model(bunny_model);
        m_face_num = m_scene->get_face_num();
    }

    void genDataBuffer()
    {
        m_scene->gen_encoded_data();
        auto& scene_data = m_scene->get_encoded_data();
        auto& bvh_data = m_scene->get_encoded_bvh();
        m_bvh_node_num = bvh_data.size();

        int a = 0;
        Console.Log(a++);

        m_data_buffer = new Buffer(
            (void*)scene_data.data(),
            scene_data.size() * sizeof(InternalTypes::Triangle_encoded),
            GL_RGB32F
        );
        m_BVH_buffer = new Buffer(
            (void*)bvh_data.data(),
            bvh_data.size() * sizeof(InternalTypes::BVH_encoded),
            GL_RGB32F
        );
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
        
        m_shader = new ShaderProgram("spirv-shaders/PathTracing");
        
        while (!glfwWindowShouldClose(m_window))
        {
            processInput();
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_shader->use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_BUFFER, m_data_buffer->getID());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_BUFFER, m_BVH_buffer->getID());
            int w, h;
            glfwGetWindowSize(m_window, &w, &h);
            m_shader->setFloat(0, w);
            m_shader->setFloat(1, h);
            m_shader->setInt(2, 0);
            m_shader->setInt(3, 1);
            m_shader->setInt(4, m_face_num);
            m_shader->setInt(5, m_bvh_node_num);
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
        mainLoop();
    }
};


int main()
{
    ApplicationCreateInfo createInfo {
        .title = "Lite RT",
        .gl_major_version = 4,
        .gl_minor_version = 6,
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
