#include "World.h"

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
        if (body->inverseMass == 0.0f) {
            continue;
        }
        glm::vec3 gravityForce = gravity * (1.0f / body->inverseMass);
        body->AddForce(gravityForce);
        body->Integrate(dt);
    }
    
    // Check for collisions after physics integration
    CheckCollisions();
}

void World::CheckCollisions() {
    for (RigidBody3D* body : bodies) {
        if (!body) continue;
        
        // Check ground collision
        if (body->CheckGroundCollision(groundLevel)) {
            body->ResolveGroundCollision(groundLevel);
        }
    }
}


