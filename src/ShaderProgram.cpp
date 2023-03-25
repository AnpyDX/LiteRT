#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>
using namespace LRT;

ShaderProgram::ShaderProgram(const std::string& shaderPath)
{
    /* Read Shader */
    std::fstream vertexFstream(shaderPath + ".vert");
    std::fstream fragFstream(shaderPath + ".frag");
    if (!vertexFstream.is_open() || !fragFstream.is_open()) {
        throw std::runtime_error("Failed to open shader: " + shaderPath);
    }

    std::stringstream vertexSStream, fragSStream;
    std::string vertexString, fragString;
    // -- Vertex
    vertexSStream << vertexFstream.rdbuf();
    vertexString = vertexSStream.str();
    const char* vertexCode = vertexString.c_str();
    // -- Fragment
    fragSStream << fragFstream.rdbuf();
    fragString = fragSStream.str();
    const char* fragCode = fragString.c_str();

    vertexFstream.close();
    fragFstream.close();

    /* Create Shaders */
    int success;
    char infoLog[512];
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to compile vertex shader: " + shaderPath + "\nInfo> " + infoLog);
    }

    unsigned int fragShader;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragCode, nullptr);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to compile fragment shader: " + shaderPath + "\nInfo> " + infoLog);
    }

    /* Create Shader Program */
    m_id = glCreateProgram();
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragShader);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
        throw std::runtime_error("Failed to create shader program: " + shaderPath + "\nInfo> " + infoLog);
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

void ShaderProgram::setBool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setInt(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string& name, glm::vec3 value)
{
    glUniform1fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setMat4(const std::string& name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}