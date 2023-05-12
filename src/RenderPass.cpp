#include "RenderPass.h"
#include "GLFW/glfw3.h"
#include <stdexcept>
using namespace LRT;

SubPass::SubPass(SubPassCreateInfo& createInfo)
: m_info(createInfo), has_destroyed(false)
{
    if (createInfo.final_pass && !createInfo.out_attachments.empty()) {
        throw std::runtime_error("The out_attachments of the final subpass should be empty!");
    }
}

SubPass::~SubPass()
{
    if (has_destroyed) return;
    has_destroyed = true;
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (!m_info.final_pass) glDeleteFramebuffers(1, &FBO);
}

void SubPass::create_data(RenderPassDependence& dependence, GLFWwindow* window_h)
{
    // Check if the attachments is existed in dependence
    if (m_info.in_attachments.size() > dependence.size() || m_info.out_attachments.size() > dependence.size()) {
        throw std::runtime_error("The indices of attachments are out of range of renderpass dependence!");
    }

    m_renderpass_dep = dependence;
    m_window_handle = window_h;

    if (m_info.final_pass) {
        FBO = 0;
    }
    else {
        /* Framebuffer */
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        for (int i = 0; i < m_info.out_attachments.size(); i++)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, dependence[m_info.out_attachments[i]], 0);
            m_attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
        }
        glDrawBuffers(m_attachments.size(), m_attachments.data());
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    /* VertexBuffer */
    float square_vertices[] = {
        // position          texcoord
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices), square_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* Bind attachments to shader */
    m_info.shader_program_ref.use();
    for (int i = 0; i < m_info.in_attachments.size(); i++) {
        m_info.shader_program_ref.setInt(i, 5 + i);
        m_in_textures.push_back(dependence[m_info.in_attachments[i]]);
    }
    glUseProgram(0);
}

void SubPass::active_textures()
{
    for (int i = 0; i < m_in_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE5 + i);
        glBindTexture(GL_TEXTURE_2D, m_in_textures[i]);
    }
}

void SubPass::do_in_op()
{
    if (m_info.in_OP == SUBPASS_ATTACHMENTS_OP_CLEAR) {
        int w, h;
        glfwGetWindowSize(m_window_handle, &w, &h);
        for (int i = 0; i < m_info.in_attachments.size(); i++) {
            glBindTexture(GL_TEXTURE_2D, m_renderpass_dep[m_info.in_attachments[i]]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void SubPass::do_out_op()
{
    if (m_info.out_OP == SUBPASS_ATTACHMENTS_OP_CLEAR) {
        int w, h;
        glfwGetWindowSize(m_window_handle, &w, &h);
        for (int i = 0; i < m_info.out_attachments.size(); i++) {
            glBindTexture(GL_TEXTURE_2D, m_renderpass_dep[m_info.out_attachments[i]]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void SubPass::draw()
{
    m_info.shader_program_ref.use();
    active_textures();
    do_in_op();
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    do_out_op();
}


RenderPass::RenderPass(RenderpassCreateInfo createInfo)
: m_dependence(createInfo.dependence), m_window_handle(createInfo.window_handle)
{}

void RenderPass::add_pass(std::shared_ptr<SubPass> pass)
{
    m_subpass_set.push_back(pass);
    pass->create_data(m_dependence, m_window_handle);
}

void RenderPass::render()
{
    for (auto pass : m_subpass_set) {
        pass->draw();
    }
}