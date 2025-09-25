#pragma once

#include <string>
#include <glm/glm.hpp>

// Forward declare OpenGL types to avoid header conflicts
typedef unsigned int GLuint;
typedef int GLint;
typedef char GLchar;
typedef unsigned int GLenum;

// Manages OpenGL shader programs
class Shader {
public:
    Shader();
    ~Shader();
    
    // Load shaders from source strings
    bool loadFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    
    // Use this shader program
    void use() const;
    
    // Set uniforms
    void setUniform(const std::string& name, const glm::mat4& value) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, int value) const;
    
    // Get program ID
    GLuint getProgramID() const { return m_programID; }

private:
    GLuint m_programID;
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
    
    bool compileShader(GLuint& shader, GLenum type, const std::string& source);
    bool linkProgram();
    void cleanup();
};
