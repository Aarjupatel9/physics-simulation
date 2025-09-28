#pragma once

#include <vector>
#include <glm/glm.hpp>

class RigidBody3D;

// Handles collision detection and resolution between objects
class CollisionSystem {
public:
    struct CollisionInfo {
        RigidBody3D* bodyA;
        RigidBody3D* bodyB;
        glm::vec3 contactPoint;
        glm::vec3 contactNormal;
        float penetration;
    };

    // Check collisions between all bodies
    void CheckCollisions(const std::vector<RigidBody3D*>& bodies, std::vector<CollisionInfo>& collisions);
    
    // Resolve a single collision
    void ResolveCollision(const CollisionInfo& collision, float restitution = 0.7f);
    
    // Ground collision (special case)
    void CheckGroundCollisions(const std::vector<RigidBody3D*>& bodies, float groundY = -1.0f);

private:
    // Sphere-sphere collision detection
    bool CheckSphereSphere(RigidBody3D* bodyA, RigidBody3D* bodyB, CollisionInfo& info);
    
    // Ground collision detection
    bool CheckGroundCollision(RigidBody3D* body, float groundY, CollisionInfo& info);
};
