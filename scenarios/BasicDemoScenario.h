#pragma once

#include "BaseScenario.h"
#include "../src/core/World.h"
#include "../src/core/RigidBody3D.h"
#include "../src/rendering/Shader.h"
#include "../src/rendering/Mesh.h"
#include "../src/rendering/Camera.h"
#include <memory>
#include <vector>

// Forward declaration
class FPSRenderer;

// Basic demo scenario: Cube and sphere with gravity and ground collision
class BasicDemoScenario : public BaseScenario {
public:
    BasicDemoScenario();
    ~BasicDemoScenario();
    
    bool initialize(GLFWwindow* window) override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;
    
    // FPS display support
    void renderFPS() override;
    void toggleFPSDisplay() override;
    bool isFPSDisplayEnabled() const override;
    
    const char* getName() const override { return "Basic Demo"; }
    const char* getDescription() const override { return "Cube and sphere with gravity and ground collision"; }
    
    // Performance testing
    void setPerformanceTestMode(bool enabled) { m_performanceTestMode = enabled; }

private:
    // Physics
    std::unique_ptr<World> m_world;
    std::vector<std::unique_ptr<RigidBody3D>> m_bodies;
    
    // Rendering
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Mesh> m_cubeMesh;
    std::unique_ptr<Mesh> m_sphereMesh;
    std::unique_ptr<Mesh> m_groundMesh;
    
    // Window reference for input
    GLFWwindow* m_window;
    
    // FPS renderer
    std::unique_ptr<FPSRenderer> m_fpsRenderer;
    
    // Performance testing mode
    bool m_performanceTestMode = false;
    
    // Shader source code
    static const char* s_vertexShaderSource;
    static const char* s_fragmentShaderSource;
    
    // Performance testing
    void createPerformanceTestObjects();
};
