#include "CollisionSystem.h"
#include "RigidBody3D.h"

void CollisionSystem::CheckCollisions(const std::vector<RigidBody3D*>& bodies, std::vector<CollisionInfo>& collisions) {
    collisions.clear();
    
    // Check all pairs of bodies
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            if (!bodies[i] || !bodies[j]) continue;
            
            CollisionInfo info;
            if (CheckSphereSphere(bodies[i], bodies[j], info)) {
                collisions.push_back(info);
            }
        }
    }
}

void CollisionSystem::ResolveCollision(const CollisionInfo& collision, float restitution) {
    RigidBody3D* bodyA = collision.bodyA;
    RigidBody3D* bodyB = collision.bodyB;
    
    if (!bodyA || !bodyB) return;
    
    // Relative velocity
    glm::vec3 relativeVelocity = bodyB->velocity - bodyA->velocity;
    float velocityAlongNormal = glm::dot(relativeVelocity, collision.contactNormal);
    
    // Don't resolve if velocities are separating
    if (velocityAlongNormal > 0) return;
    
    // Calculate restitution
    float newRestitution = restitution * restitution;
    
    // Calculate impulse scalar
    float impulseScalar = -(1 + newRestitution) * velocityAlongNormal;
    impulseScalar /= bodyA->inverseMass + bodyB->inverseMass;
    
    glm::vec3 impulse = impulseScalar * collision.contactNormal;
    
    // Apply impulse
    bodyA->velocity -= impulse * bodyA->inverseMass;
    bodyB->velocity += impulse * bodyB->inverseMass;
    
    // Position correction
    const float percent = 0.2f; // How much to correct
    const float slop = 0.01f;   // How much overlap to ignore
    
    glm::vec3 correction = collision.contactNormal * (percent * glm::max(collision.penetration - slop, 0.0f) / (bodyA->inverseMass + bodyB->inverseMass));
    bodyA->position -= correction * bodyA->inverseMass;
    bodyB->position += correction * bodyB->inverseMass;
}

void CollisionSystem::CheckGroundCollisions(const std::vector<RigidBody3D*>& bodies, float groundY) {
    for (RigidBody3D* body : bodies) {
        if (!body) continue;
        
        CollisionInfo info;
        if (CheckGroundCollision(body, groundY, info)) {
            ResolveCollision(info);
        }
    }
}

bool CollisionSystem::CheckSphereSphere(RigidBody3D* bodyA, RigidBody3D* bodyB, CollisionInfo& info) {
    glm::vec3 distance = bodyB->position - bodyA->position;
    float distanceLength = glm::length(distance);
    
    // Assuming radius of 0.5 for both spheres
    float radiusA = 0.5f;
    float radiusB = 0.5f;
    float minDistance = radiusA + radiusB;
    
    if (distanceLength < minDistance) {
        info.bodyA = bodyA;
        info.bodyB = bodyB;
        info.contactNormal = glm::normalize(distance);
        info.contactPoint = bodyA->position + info.contactNormal * radiusA;
        info.penetration = minDistance - distanceLength;
        return true;
    }
    
    return false;
}

bool CollisionSystem::CheckGroundCollision(RigidBody3D* body, float groundY, CollisionInfo& info) {
    float radius = 0.5f;
    if (body->position.y - radius <= groundY) {
        info.bodyA = body;
        info.bodyB = nullptr; // Ground is static
        info.contactNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        info.contactPoint = glm::vec3(body->position.x, groundY, body->position.z);
        info.penetration = groundY - (body->position.y - radius);
        return true;
    }
    return false;
}
