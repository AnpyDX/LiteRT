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
    enum SubPassAttachmentsOp {
        ATTACHMENT_OP_SAVE = 0,
        ATTACHMENT_OP_CLEAR
    };
    struct SubPassCreateInfo {
        bool final_pass = false;
        ShaderProgram& shader_program_ref;
        std::vector<uint32_t> in_attachments;
        std::vector<uint32_t> out_attachments;
        SubPassAttachmentsOp out_Op = ATTACHMENT_OP_SAVE;
    };

    /* RenderPass Objects */
    using RenderPassDependence = std::vector<GLuint>;

    class SubPass
    {
    friend class RenderPass;
    public:
        SubPass(SubPassCreateInfo& createInfo);
        ~SubPass();
    
    private:
        void create_data(RenderPassDependence& dependence);
        void active_textures();
        void do_attachment_op();
        void draw();

    public:
        SubPassCreateInfo m_info;
        std::vector<GLuint> m_attachments;
        std::vector<GLuint> m_in_textures;
        GLuint FBO;
        GLuint VAO;
        GLuint VBO;
        bool has_destroyed;
    };

    class RenderPass
    {
    public:
        RenderPass(RenderPassDependence dependence);
        ~RenderPass() = default;

        /* Add SubPass to RenderPass */
        void add_pass(std::shared_ptr<SubPass> pass);

        /* DO RenderPass */
        void render();

    private:
        RenderPassDependence m_dependence;
        std::vector<std::shared_ptr<SubPass>> m_subpass_set;;
    };
}