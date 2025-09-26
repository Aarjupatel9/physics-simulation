#pragma once

#include "BaseScenario.h"
#include "../src/rendering/Skybox.h"
#include "../src/rendering/Terrain.h"
#include "../src/rendering/GridRenderer.h"
#include "../src/rendering/Camera.h"
#include <memory>

// Beautiful terrain scenario with skybox, procedural terrain, and environment
class TerrainScenario : public BaseScenario {
public:
    TerrainScenario();
    ~TerrainScenario();
    
    bool initialize(GLFWwindow* window) override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;
    
    const char* getName() const override { return "Beautiful Terrain"; }
    const char* getDescription() const override { return "Procedural terrain with skybox, grass, and rocks"; }

private:
    // Rendering systems
    std::unique_ptr<Skybox> m_skybox;
    std::unique_ptr<Terrain> m_terrain;
    std::unique_ptr<GridRenderer> m_gridRenderer;
    std::unique_ptr<Camera> m_camera;
    
    // Window reference
    GLFWwindow* m_window;
    
    // Lighting
    glm::vec3 m_sunDirection;
    
    // Terrain parameters - simple flat test
    static constexpr int TERRAIN_SIZE = 32;  // Very small for testing
    static constexpr float TERRAIN_SCALE = 1.0f;  // 32 * 1.0 = 32 units total (bigger)
    static constexpr float TERRAIN_HEIGHT = 0.0f;  // Flat terrain
    static constexpr float TERRAIN_ROUGHNESS = 0.0f;  // No variation
};
