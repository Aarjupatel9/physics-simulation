#pragma once

#include <vector>
#include <glm/glm.hpp>

// Forward declare OpenGL types
typedef unsigned int GLuint;

// Renders a simple grid overlay for debugging
class GridRenderer {
public:
    GridRenderer();
    ~GridRenderer();
    
    // Initialize grid system
    bool initialize(float size, int divisions);
    
    // Render the grid
    void render(const glm::mat4& view, const glm::mat4& projection);
    
    // Set grid color
    void setColor(const glm::vec3& color) { m_color = color; }

private:
    // Grid data
    std::vector<float> m_vertices;
    GLuint m_VAO, m_VBO;
    GLuint m_shaderProgram;
    glm::vec3 m_color;
    
    // Create grid geometry
    void createGrid(float size, int divisions);
    
    // Create grid shader
    bool createShader();
    
    void cleanup();
};
