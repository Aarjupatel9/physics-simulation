#include "World.h"
#include "PhysicsConstants.h"

// Initialize world with a constant gravity vector
World::World(const glm::vec3& gravity) : gravity(gravity) {}

// Register a rigid body with the world
void World::AddBody(RigidBody3D* body) {
    bodies.push_back(body);
}

// Apply forces and integrate all bodies, then resolve collisions
void World::Update(float dt) {
    for (RigidBody3D* body : bodies) {
        if (!body) continue;
        if (body->isStatic()) {
            continue;
        }
        
        // Apply gravity if enabled
        if (body->isGravityEnabled()) {
            glm::vec3 gravityForce = gravity * body->getMass();
            body->addForce(gravityForce);
        }
        
        body->integrate(dt);
    }
    
    // Check for collisions after physics integration
    CheckCollisions();
}

void World::CheckCollisions() {
    for (RigidBody3D* body : bodies) {
        if (!body) continue;
        
        // Check ground collision
        if (body->checkGroundCollision(groundLevel)) {
            body->resolveGroundCollision(groundLevel);
        }
    }
}


