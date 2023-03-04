/**
* ShaderProgram Object
*/

#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace LRT
{
    class ShaderProgram
    {
    public:
        ShaderProgram() = delete;
        ShaderProgram(const std::string& shaderPath);

        // Get Shader Program ID
        [[nodiscard]]
        unsigned int getID() const;
        // Use Shader Program
        void use() const;
        // Destroy Shader Program
        void destroy();

        /* Shader Unfiorms */
        void setBool(const std::string& name, bool value);
        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setVec3(const std::string& name, glm::vec3 value);
        void setMat4(const std::string& name, glm::mat4 value);

    private:
        unsigned int m_id;
    };
}
