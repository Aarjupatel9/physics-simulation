#pragma once

#include <glm/glm.hpp>

namespace Physics {
    // Unit System: 1 unit = 1 meter
    constexpr float METER = 1.0f;
    constexpr float CENTIMETER = 0.01f;
    constexpr float MILLIMETER = 0.001f;
    
    // Physics Constants
    constexpr float GRAVITY = 9.81f; // m/s²
    constexpr float AIR_DENSITY = 1.225f; // kg/m³ at sea level
    
    // Default Physics Properties
    constexpr float DEFAULT_MASS = 1.0f; // kg
    constexpr float DEFAULT_DENSITY = 1000.0f; // kg/m³ (water density)
    constexpr float DEFAULT_FRICTION = 0.5f; // 0-1
    constexpr float DEFAULT_RESTITUTION = 0.3f; // 0-1 (bounciness)
    constexpr float DEFAULT_LINEAR_DAMPING = 0.99f; // 0-1
    constexpr float DEFAULT_ANGULAR_DAMPING = 0.99f; // 0-1
    
    // Simulation Settings
    constexpr float DEFAULT_TIME_STEP = 1.0f / 60.0f; // 60 FPS
    constexpr int DEFAULT_VELOCITY_ITERATIONS = 8;
    constexpr int DEFAULT_POSITION_ITERATIONS = 3;
    
    // Collision Detection
    constexpr float CONTACT_TOLERANCE = 0.01f; // meters
    constexpr float BROAD_PHASE_MARGIN = 0.1f; // meters
    
    // Default Gravity Vector (downward)
    constexpr glm::vec3 DEFAULT_GRAVITY = glm::vec3(0.0f, -GRAVITY, 0.0f);
    
    // Shape Defaults
    constexpr float DEFAULT_SPHERE_RADIUS = 0.5f; // 0.5 meters
    constexpr glm::vec3 DEFAULT_BOX_SIZE = glm::vec3(1.0f, 1.0f, 1.0f); // 1m x 1m x 1m
    constexpr float DEFAULT_CYLINDER_RADIUS = 0.5f; // 0.5 meters
    constexpr float DEFAULT_CYLINDER_HEIGHT = 1.0f; // 1 meter
    constexpr glm::vec2 DEFAULT_PLANE_SIZE = glm::vec2(10.0f, 10.0f); // 10m x 10m
    
    // Rendering Defaults
    constexpr int DEFAULT_SPHERE_SEGMENTS = 32;
    constexpr int DEFAULT_CYLINDER_SEGMENTS = 16;
}
