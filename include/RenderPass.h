/**
* Project: LiteRT
* -- RenderPass Object
*/

#pragma once
#include <memory>
#include <vector>
#include <map>
#include "ShaderProgram.h"

namespace LRT {

    /* Create Infos */
    using RenderPassDependence = std::vector<GLuint>;
    struct RenderpassCreateInfo {
        RenderPassDependence dependence;
        GLFWwindow* window_handle;
    };

    enum SubPassAttachmentsOP {
        SUBPASS_ATTACHMENTS_OP_SAVE = 0,
        SUBPASS_ATTACHMENTS_OP_CLEAR
    };

    struct SubPassCreateInfo {
        bool final_pass = false;
        ShaderProgram& shader_program_ref;
        std::vector<uint32_t> in_attachments;
        std::vector<uint32_t> out_attachments;
        SubPassAttachmentsOP in_OP = SUBPASS_ATTACHMENTS_OP_SAVE;
        SubPassAttachmentsOP out_OP = SUBPASS_ATTACHMENTS_OP_SAVE;
    };

    /* RenderPass Objects */

    class SubPass
    {
    friend class RenderPass;
    public:
        SubPass(SubPassCreateInfo& createInfo);
        ~SubPass();
    
    private:
        void create_data(RenderPassDependence& dependence, GLFWwindow* window_h);
        void active_textures();
        void do_in_op();
        void do_out_op();
        void draw();

    public:
        SubPassCreateInfo m_info;
        std::vector<GLuint> m_attachments;
        std::vector<GLuint> m_in_textures;
        GLuint FBO;
        GLuint VAO;
        GLuint VBO;
        RenderPassDependence m_renderpass_dep;
        GLFWwindow* m_window_handle;
        bool has_destroyed;
    };

    class RenderPass
    {
    public:
        RenderPass(RenderpassCreateInfo createInfo);
        ~RenderPass() = default;

        /* Add SubPass to RenderPass */
        void add_pass(std::shared_ptr<SubPass> pass);

        /* DO RenderPass */
        void render();

    private:
        RenderPassDependence m_dependence;
        GLFWwindow* m_window_handle;
        std::vector<std::shared_ptr<SubPass>> m_subpass_set;;
    };
}