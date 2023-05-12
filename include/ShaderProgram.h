/**
* Project: LiteRT
* -- ShaderProgram Object
*/

#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MathUtility.h"

namespace LRT
{
    class ShaderProgram
    {
    public:
        ShaderProgram() = delete;
        ShaderProgram(const std::string& path);

        // Get Shader Program ID
        [[nodiscard]] unsigned int getID() const;
        
        // Use Shader Program
        void use() const;

        // Destroy Shader Program
        void destroy();

        /* Shader Unfiorms */
        void setBool(const uint32_t& location, bool value);
        void setInt(const uint32_t& location, int value);
        void setUint(const uint32_t& location, GLuint value);
        void setFloat(const uint32_t& location, float value);
        void setVec3(const uint32_t& location, glm::vec3 value);
        void setMat4(const uint32_t& location, glm::mat4 value);

    private:
        std::vector<char> read_spirv(const std::string& path);
    
    private:
        unsigned int m_id;
    };
}
