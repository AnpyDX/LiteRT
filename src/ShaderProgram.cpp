#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
using namespace LRT;

std::vector<char> ShaderProgram::read_spirv(const std::string& path)
{
    std::ifstream spirv_f(path, std::ios::ate | std::ios::binary);
    if (!spirv_f.is_open()) {
        throw std::runtime_error("Failed to open SPIRV shader: " + path + "!");
    }

    std::vector<char> data(spirv_f.tellg());
    spirv_f.seekg(0);
    spirv_f.read(data.data(), data.size());
    spirv_f.close();
    return data;
}

ShaderProgram::ShaderProgram(const std::string& path)
{
    /* Read Shader */
    std::vector<char> vertex_data = read_spirv(path + ".vert.spirv");
    std::vector<char> frag_data = read_spirv(path + ".frag.spirv");
    if (vertex_data.empty() || frag_data.empty()) {
        throw std::runtime_error("Cannot read empty shader for ShaderProgram: " + path + "!");
    }

    /* Create Shaders */
    int success;
    char infoLog[512];
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderBinary(1, &vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertex_data.data(), vertex_data.size());
    glSpecializeShader(vertexShader, "main", 0, 0, 0);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to load vertex shader: " + path + "\nInfo> " + infoLog);
    }

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderBinary(1, &fragShader, GL_SHADER_BINARY_FORMAT_SPIR_V, frag_data.data(), frag_data.size());
    glSpecializeShader(fragShader, "main", 0, 0, 0);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to load fragment shader: " + path + "\n> ShaderInfo: " + infoLog);
    }

    /* Create Shader Program */
    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragShader);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to create shader program: " + path + "\nInfo> " + infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);
}

unsigned int ShaderProgram::getID() const
{
    return m_id;
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}

void ShaderProgram::destroy()
{
    glDeleteProgram(m_id);
}

void ShaderProgram::setBool(const uint32_t& location, bool value)
{
    glUniform1i(location, value);
}

void ShaderProgram::setInt(const uint32_t& location, int value)
{
    glUniform1i(location, value);
}

void ShaderProgram::setUint(const uint32_t& location, GLuint value)
{
    glUniform1ui(location, value);
}

void ShaderProgram::setFloat(const uint32_t& location, float value)
{
    glUniform1f(location, value);
}

void ShaderProgram::setVec3(const uint32_t& location, glm::vec3 value)
{
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setMat4(const uint32_t& location, glm::mat4 value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}