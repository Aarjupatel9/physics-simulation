#ifndef RIGIDBODY2D_H
#define RIGIDBODY2D_H

#include "Vec2.h"

struct RigidBody2D {
    // Linear motion properties
    Vec2 position;
    Vec2 velocity;
    Vec2 force_accumulator;
    float mass;
    float inverse_mass; // Storing 1/mass is efficient

    // Angular motion properties
    float angle; // in radians
    float angular_velocity;
    float torque_accumulator;
    float moment_of_inertia;
    float inverse_moment_of_inertia; // Storing 1/I is efficient

    // Constructor
    RigidBody2D(float x, float y, float mass);

    // Functions to apply forces and torques
    void AddForce(const Vec2& force);
    void AddTorque(float torque);
    void ClearAccumulators();
};

#endif // RIGIDBODY2D_H
