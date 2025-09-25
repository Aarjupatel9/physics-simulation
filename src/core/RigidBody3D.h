#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class RigidBody3D {
public:
    // Linear properties
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 forceAccumulator;
    float inverseMass;

    // Angular properties
    glm::quat orientation; // Represents rotation
    glm::vec3 angularVelocity;
    glm::vec3 torqueAccumulator;
    glm::mat3 inverseInertiaTensor;

    RigidBody3D(glm::vec3 pos, float mass);

    void AddForce(const glm::vec3& force);
    void ClearAccumulators();
    void Integrate(float dt);
    
    // Collision detection
    bool CheckGroundCollision(float groundY, float radius = 0.5f);
    void ResolveGroundCollision(float groundY, float restitution = 0.7f, float radius = 0.5f);
};
