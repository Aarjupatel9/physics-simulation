#pragma once

#include "BaseScenario.h"
#include <memory>
#include <vector>
#include <random>
#include <glm/glm.hpp>

// Forward declarations
class World;
class Camera;
class Shader;
class Mesh;
class RigidBody3D;

// Include FPSRenderer header to avoid incomplete type issues
#include "../src/rendering/FPSRenderer.h"

/**
 * Ball Collision Demo
 * 
 * Features:
 * - 5x5 meter bounded plane with walls
 * - Multiple balls with random initial velocities
 * - Ball-to-ball and ball-to-wall collisions
 * - Performance monitoring
 */
class BallCollisionDemo : public BaseScenario {
public:
    BallCollisionDemo();
    virtual ~BallCollisionDemo() = default;

    // BaseScenario interface
    const char* getName() const override { return "Ball Collision Demo"; }
    const char* getDescription() const override { return "Multiple balls with collisions on a bounded 5x5m plane"; }
    
    bool initialize(GLFWwindow* window) override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;

private:
    // Core systems
    std::unique_ptr<World> m_world;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<FPSRenderer> m_fpsRenderer;
    
    // Meshes
    std::shared_ptr<Mesh> m_ballMesh;
    std::shared_ptr<Mesh> m_floorMesh;
    std::shared_ptr<Mesh> m_wallMesh;
    
    // Physics objects
    std::vector<std::unique_ptr<RigidBody3D>> m_balls;
    std::unique_ptr<RigidBody3D> m_floor;
    std::vector<std::unique_ptr<RigidBody3D>> m_walls;
    
    // Demo parameters
    static constexpr float PLANE_SIZE = 5.0f;         // 5x5 meters
    static constexpr float BALL_RADIUS = 0.2f;        // 20 cm radius
    static constexpr float WALL_HEIGHT = 0.3f;        // 30 cm height
    static constexpr float WALL_WIDTH = 0.2f;         // 20 cm width
    static constexpr int NUM_BALLS = 50;              // Number of balls
    
    // Statistics
    int m_objectCount = 0;
    
    // Window reference
    GLFWwindow* m_window = nullptr;
    
    // Random number generation
    std::random_device m_rd;
    std::mt19937 m_gen;
    std::uniform_real_distribution<float> m_velDist;
    std::uniform_real_distribution<float> m_posDist;
    
    // Helper methods
    void createBalls();
    void createFloor();
    void createWalls();
    glm::vec3 getRandomVelocity();
    glm::vec3 getRandomPosition();
};
