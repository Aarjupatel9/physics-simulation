#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <memory>

// Forward declare OpenGL types
typedef unsigned int GLuint;

// Renders rocks using instanced rendering for performance
class RockRenderer {
public:
    struct RockInstance {
        glm::vec3 position;
        float scale;
        float rotation;
        glm::vec3 color;
    };
    
    RockRenderer();
    ~RockRenderer();
    
    // Initialize rock system
    bool initialize();
    
    // Generate rock instances on terrain
    void generateRocks(const class Terrain& terrain, int rockCount = 500);
    
    // Render all rock instances
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDir);
    
    // Clear rock instances
    void clear();

private:
    // Rock geometry (simple cube)
    std::vector<float> m_rockVertices;
    std::vector<unsigned int> m_rockIndices;
    
    // Instanced data
    std::vector<RockInstance> m_instances;
    
    // OpenGL objects
    GLuint m_VAO, m_VBO, m_EBO;
    GLuint m_instanceVBO;
    GLuint m_shaderProgram;
    
    // Create rock geometry
    void createRockGeometry();
    
    // Create rock shader
    bool createShader();
    
    void cleanup();
};
