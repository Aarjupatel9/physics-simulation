#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

// Forward declare OpenGL types
typedef unsigned int GLuint;

// Manages skybox rendering with cube map
class Skybox {
public:
    Skybox();
    ~Skybox();
    
    // Initialize skybox with day preset
    bool initialize();
    
    // Render the skybox
    void render(const glm::mat4& view, const glm::mat4& projection);
    
    // Get skybox shader program ID
    GLuint getShaderProgram() const { return m_shaderProgram; }

private:
    GLuint m_VAO, m_VBO;
    GLuint m_cubeMap;
    GLuint m_shaderProgram;
    
    // Generate day skybox colors
    std::vector<float> generateDaySkyboxColors();
    
    // Create skybox shader
    bool createShader();
    
    void cleanup();
};
