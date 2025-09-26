#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

// Forward declare OpenGL types
typedef unsigned int GLuint;

// Renders grass using instanced rendering for performance
class GrassRenderer {
public:
    struct GrassInstance {
        glm::vec3 position;
        float scale;
        float rotation;
        glm::vec3 color;
    };
    
    GrassRenderer();
    ~GrassRenderer();
    
    // Initialize grass system
    bool initialize();
    
    // Generate grass instances on terrain
    void generateGrass(const class Terrain& terrain, int grassCount = 10000);
    
    // Render all grass instances
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDir);
    
    // Clear grass instances
    void clear();

private:
    // Grass blade geometry
    std::vector<float> m_grassVertices;
    std::vector<unsigned int> m_grassIndices;
    
    // Instanced data
    std::vector<GrassInstance> m_instances;
    
    // OpenGL objects
    GLuint m_VAO, m_VBO, m_EBO;
    GLuint m_instanceVBO;
    GLuint m_shaderProgram;
    
    // Create grass blade geometry
    void createGrassBlade();
    
    // Create grass shader
    bool createShader();
    
    void cleanup();
};
