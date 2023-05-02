/**
* LiteRT Projects
* Author: github.com/AnpyDX
* -- Main
*/

#include <X11/extensions/Xrandr.h>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include "obj_loader.h"
#include "Console.h"
#include "Application.h"
#include "Buffer.h"
#include "DataTypes.h"
#include "ShaderProgram.h"
#include "SceneManager.h"
#include "RenderPass.h"
using namespace LRT;

struct ResizeRefreshedData {
    unsigned int *frame_counter;
    RenderPassDependence *dependence;
};

class LRT_APP : public Application
{
private:
    Buffer* m_data_buffer;
    Buffer* m_BVH_buffer;
    SceneManager* m_scene;
    uint32_t m_face_num;
    uint32_t m_bvh_node_num;
    unsigned int m_frame_counter;
    ResizeRefreshedData m_refreshable_data;

    std::shared_ptr<RenderPass> m_renderPass;
    RenderPassDependence m_renderpass_dependence;
    std::shared_ptr<ShaderProgram> m_raytracing_shader;
    std::shared_ptr<ShaderProgram> m_copy_shader;
    std::shared_ptr<ShaderProgram> m_present_shader;

public:
    LRT_APP(ApplicationCreateInfo info): Application(info) {
        init();
        readScene();
        createData();
        configPass();
    }
    
    ~LRT_APP() {
        if (m_BVH_buffer != nullptr) delete m_BVH_buffer;
        if (m_data_buffer != nullptr) delete m_data_buffer;
        if (m_scene != nullptr) delete m_scene;

        m_scene = nullptr;
        m_data_buffer = nullptr;
        m_BVH_buffer = nullptr;
    }

    void init()
    {
        m_frame_counter = 0;

        // set resize callback
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height)->void {
            // Resize viewport
            glViewport(0, 0, width, height);

            // Refresh data
            ResizeRefreshedData data = *(ResizeRefreshedData*)glfwGetWindowUserPointer(window);
            *data.frame_counter = 0;
            for (auto i : *data.dependence) {
                glBindTexture(GL_TEXTURE_2D, i);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
            }
            glBindTexture(GL_TEXTURE_2D, 0);

        });
    }

    void readScene()
    {
        m_scene = new SceneManager();
        m_scene->enable_BVH = true;
        /* Scene Data */
        OBJ_Object bunny_obj = obj_loader("models/bunny.obj");
        OBJ_Object light_obj = obj_loader("models/light.obj");
        OBJ_Object plane_obj = obj_loader("models/plane.obj");

        Model bunny_model {};
        InternalTypes::Material mat {};
        mat.baseColor = vec3(0.1, 0.5, 0.6);
        mat.emissive = vec3(0.0);
        
        bunny_model.add_obj_data(bunny_obj);
        bunny_model.material = mat;

        Model light_model {};
        InternalTypes::Material light_mat {};
        light_mat.baseColor = vec3(1.0);
        light_mat.emissive = vec3(2.0);

        light_model.add_obj_data(light_obj);
        light_model.material = light_mat;

        Model plane_model {};
        InternalTypes::Material plane_mat {};
        plane_mat.baseColor = vec3(1.0);
        plane_mat.emissive = vec3(0.0);
        plane_model.add_obj_data(plane_obj);
        plane_model.material = plane_mat;

        m_scene->add_model(bunny_model);
        m_scene->add_model(light_model);
        m_scene->add_model(plane_model);
        m_face_num = m_scene->get_face_num();
    }

    void createData()
    {
        /* Scene Buffers */
        m_scene->gen_encoded_data();
        auto& scene_data = m_scene->get_encoded_data();
        auto& bvh_data = m_scene->get_encoded_bvh();
        m_bvh_node_num = bvh_data.size();

        Console.Log("BVH size: " + std::to_string(m_bvh_node_num));

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

        /** RenderPass Dependences 
        * Layout: [0] => Raytracing result
        *         [1] => Copy Last Frame
        *         [2] => Present
        */
        m_renderpass_dependence.resize(3);
        glGenTextures(3, m_renderpass_dependence.data());
        for (auto i : m_renderpass_dependence) {
            glBindTexture(GL_TEXTURE_2D, i);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        /* Shaders */
        m_raytracing_shader = std::make_shared<ShaderProgram>("shaders/RayTracing");
        m_copy_shader = std::make_shared<ShaderProgram>("shaders/CopyRes");
        m_present_shader = std::make_shared<ShaderProgram>("shaders/Present");

        /* The data needed to be refreshed when window is resized */
        m_refreshable_data.frame_counter = &m_frame_counter;
        m_refreshable_data.dependence = &m_renderpass_dependence;
        glfwSetWindowUserPointer(m_window, &m_refreshable_data);
    }

    void configPass()
    {
        SubPassCreateInfo pass1_info {
            .shader_program_ref = *m_raytracing_shader,
            .in_attachments = { 1 },
            .out_attachments = { 0 },
            .out_Op = ATTACHMENT_OP_CLEAR,
        };
        SubPassCreateInfo pass2_info {
            .shader_program_ref = *m_copy_shader,
            .in_attachments = { 0 },
            .out_attachments = { 1 },
            .out_Op = ATTACHMENT_OP_CLEAR,
        };
        SubPassCreateInfo pass3_info {
            .final_pass = true,
            .shader_program_ref = *m_present_shader,
            .in_attachments = { 1 },
            .out_attachments = {},
            .out_Op = ATTACHMENT_OP_CLEAR,
        };

        m_renderPass = std::make_shared<RenderPass>(m_renderpass_dependence);
        m_renderPass->add_pass(std::make_shared<SubPass>(pass1_info));
        m_renderPass->add_pass(std::make_shared<SubPass>(pass2_info));
        m_renderPass->add_pass(std::make_shared<SubPass>(pass3_info));
    }

    void mainLoop()
    {

        /* Shader Uniform binding */
        m_raytracing_shader->use();
        m_raytracing_shader->setInt(3, 0);
        m_raytracing_shader->setInt(4, 1);
        glUseProgram(0);

        /* Render Loop */
        while (!glfwWindowShouldClose(m_window))
        {
            processInput();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_BUFFER, m_data_buffer->getID());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_BUFFER, m_BVH_buffer->getID());

            int w, h;
            glfwGetWindowSize(m_window, &w, &h);
            
            m_raytracing_shader->use();
            m_raytracing_shader->setFloat(1, w);
            m_raytracing_shader->setFloat(2, h);
            m_raytracing_shader->setUint(5, m_frame_counter++);
            glUseProgram(0);

            m_renderPass->render();

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
        .title = "Lite RT ( OpenGL 4.6 )",
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
