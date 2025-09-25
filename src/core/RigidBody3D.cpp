#include "RigidBody3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

// Construct a rigid body with position and mass; inertia is approximated
RigidBody3D::RigidBody3D(glm::vec3 pos, float mass) {
    position = pos;
    velocity = glm::vec3(0.0f);
    forceAccumulator = glm::vec3(0.0f);
    
    orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion (no rotation)
    angularVelocity = glm::vec3(0.0f);
    torqueAccumulator = glm::vec3(0.0f);

    if (mass == 0.0f) {
        inverseMass = 0.0f;
        // For a static object, we can use an inverse inertia tensor of all zeros
        inverseInertiaTensor = glm::mat3(0.0f);
    } else {
        inverseMass = 1.0f / mass;
        // For now, approximate inertia tensor of a sphere
        // A real engine would calculate this based on the shape.
        float I = (2.0f / 5.0f) * mass * 1.0f; // Assuming radius of 1 for simplicity
        inverseInertiaTensor = glm::inverse(glm::mat3(I));
    }
}

void RigidBody3D::AddForce(const glm::vec3& force) {
    forceAccumulator += force;
}

void RigidBody3D::ClearAccumulators() {
    forceAccumulator = glm::vec3(0.0f);
    torqueAccumulator = glm::vec3(0.0f);
}

// Semi-implicit Euler integration for linear and angular motion
void RigidBody3D::Integrate(float dt) {
    if (inverseMass == 0.0f) return; // Do not integrate static objects

    // --- Linear Motion ---
    // Calculate acceleration from force
    glm::vec3 linearAcceleration = forceAccumulator * inverseMass;
    // Update velocity
    velocity += linearAcceleration * dt;
    // Update position
    position += velocity * dt;

    // --- Angular Motion ---
    // Calculate angular acceleration from torque
    glm::vec3 angularAcceleration = inverseInertiaTensor * torqueAccumulator;
    // Update angular velocity
    angularVelocity += angularAcceleration * dt;
    // Update orientation (rotation)
    glm::quat deltaRotation = glm::quat(0.0f, angularVelocity.x, angularVelocity.y, angularVelocity.z) * orientation;
    orientation += deltaRotation * (dt * 0.5f);
    orientation = glm::normalize(orientation); // Keep quaternion normalized

    // Clear forces for the next frame
    ClearAccumulators();
}

// Detect collision with an infinite plane at y = groundY
bool RigidBody3D::CheckGroundCollision(float groundY, float radius) {
    // Check if the bottom of the sphere/cube is touching or below the ground
    return (position.y - radius) <= groundY;
}

// Resolve ground collision by correcting position and reflecting velocity
void RigidBody3D::ResolveGroundCollision(float groundY, float restitution, float radius) {
    if (inverseMass == 0.0f) return; // Don't resolve collisions for static objects
    
    // Position correction: move object above ground
    if (position.y - radius < groundY) {
        position.y = groundY + radius;
    }
    
    // Velocity reflection: bounce with restitution
    if (velocity.y < 0.0f) {
        velocity.y = -velocity.y * restitution;
    }
    
    // Add some damping to prevent infinite bouncing
    velocity *= 0.99f;
}
