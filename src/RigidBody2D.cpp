#include "RigidBody2D.h"
#include <limits>

RigidBody2D::RigidBody2D(float x, float y, float mass) {
    position = Vec2(x, y);
    velocity = Vec2(0, 0);
    force_accumulator = Vec2(0, 0);
    this->mass = mass;

    angle = 0.0f;
    angular_velocity = 0.0f;
    torque_accumulator = 0.0f;

    if (mass > 0.0f) {
        inverse_mass = 1.0f / mass;
    } else {
        // A mass of 0 or less represents an immovable object
        inverse_mass = 0.0f;
    }

    // For now, we assume a simple 1x1 box inertia.
    // This will be replaced by shape-specific inertia later.
    moment_of_inertia = 1.0f; // Simplified
    if (moment_of_inertia > 0.0f) {
        inverse_moment_of_inertia = 1.0f / moment_of_inertia;
    } else {
        inverse_moment_of_inertia = 0.0f;
    }
}

void RigidBody2D::AddForce(const Vec2& force) {
    force_accumulator += force;
}

void RigidBody2D::AddTorque(float torque) {
    torque_accumulator += torque;
}

void RigidBody2D::ClearAccumulators() {
    force_accumulator = Vec2(0, 0);
    torque_accumulator = 0.0f;
}
