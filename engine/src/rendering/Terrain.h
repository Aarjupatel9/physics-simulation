#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Mesh.h"
#include "Shader.h"

// Forward declare OpenGL types
typedef unsigned int GLuint;

// Manages terrain rendering with heightmap and textures
class Terrain {
public:
    Terrain();
    ~Terrain();
    
    // Initialize terrain with procedural generation
    bool initialize(int width = 256, int height = 256, float scale = 0.1f, float heightScale = 2.0f, float roughness = 1.0f);
    
    // Render the terrain
    void render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDir);
    
    // Get terrain height at world position
    float getHeightAt(float worldX, float worldZ) const;
    
    // Get terrain bounds
    glm::vec2 getBounds() const { return m_bounds; }

private:
    std::unique_ptr<Mesh> m_mesh;
    std::unique_ptr<Shader> m_shader;
    
    // Terrain data
    int m_width, m_height;
    float m_scale;
    glm::vec2 m_bounds;
    
    // Height data for collision
    std::vector<float> m_heightData;
    
    // OpenGL objects for rendering
    GLuint m_terrainVAO, m_terrainVBO, m_terrainEBO;
    size_t m_indexCount;
    
    // Create terrain shader
    bool createShader();
    
    // Convert world position to heightmap coordinates
    glm::ivec2 worldToHeightmap(float worldX, float worldZ) const;
};
