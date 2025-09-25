#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Shader::Shader() : m_programID(0), m_vertexShader(0), m_fragmentShader(0) {}

Shader::~Shader() {
    cleanup();
}

bool Shader::loadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    cleanup();
    
    if (!compileShader(m_vertexShader, GL_VERTEX_SHADER, vertexSource)) {
        return false;
    }
    
    if (!compileShader(m_fragmentShader, GL_FRAGMENT_SHADER, fragmentSource)) {
        return false;
    }
    
    if (!linkProgram()) {
        return false;
    }
    
    return true;
}

void Shader::use() const {
    glUseProgram(m_programID);
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) const {
    GLint location = glGetUniformLocation(m_programID, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
    GLint location = glGetUniformLocation(m_programID, name.c_str());
    if (location != -1) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }
}

void Shader::setUniform(const std::string& name, float value) const {
    GLint location = glGetUniformLocation(m_programID, name.c_str());
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::setUniform(const std::string& name, int value) const {
    GLint location = glGetUniformLocation(m_programID, name.c_str());
    if (location != -1) {
        glUniform1i(location, value);
    }
}

bool Shader::compileShader(GLuint& shader, GLenum type, const std::string& source) {
    shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed: " << infoLog << std::endl;
        return false;
    }
    
    return true;
}

bool Shader::linkProgram() {
    m_programID = glCreateProgram();
    glAttachShader(m_programID, m_vertexShader);
    glAttachShader(m_programID, m_fragmentShader);
    glLinkProgram(m_programID);
    
    GLint success;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_programID, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        return false;
    }
    
    return true;
}

void Shader::cleanup() {
    if (m_vertexShader) {
        glDeleteShader(m_vertexShader);
        m_vertexShader = 0;
    }
    if (m_fragmentShader) {
        glDeleteShader(m_fragmentShader);
        m_fragmentShader = 0;
    }
    if (m_programID) {
        glDeleteProgram(m_programID);
        m_programID = 0;
    }
}
