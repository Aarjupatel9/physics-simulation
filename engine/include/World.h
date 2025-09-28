#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "RigidBody3D.h"

// Simple 3D world that applies gravity and resolves ground collisions
class World {
public:
    glm::vec3 gravity;
    std::vector<RigidBody3D*> bodies;

    explicit World(const glm::vec3& gravity);

    void AddBody(RigidBody3D* body);
    void Update(float dt);
    
    // Collision handling
    void CheckCollisions();
    float groundLevel = -1.0f; // Ground plane Y position
};
