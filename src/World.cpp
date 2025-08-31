#include "World.h"

World::World(const Vec2& gravity) : gravity(gravity) {}

void World::AddBody(RigidBody2D* body) {
    bodies.push_back(body);
}

// The main update loop for the physics engine
void World::Update(float dt) {
    // Loop through all bodies
    for (auto& body : bodies) {
        // 1. Apply forces (like gravity)
        // We only apply gravity if the body has mass (is not static)
        if (body->inverse_mass > 0) {
            body->AddForce(gravity * body->mass);
        }

        // 2. Integrate forces to find new velocity (Linear motion)
        // Using Explicit Euler integration: v_new = v_old + (F/m) * dt
        Vec2 linear_acceleration = body->force_accumulator * body->inverse_mass;
        body->velocity += linear_acceleration * dt;

        // 3. Integrate velocity to find new position
        // p_new = p_old + v_new * dt
        body->position += body->velocity * dt;

        // 4. Integrate torques to find new angular velocity (Angular motion)
        // a_new = a_old + (T/I) * dt
        float angular_acceleration = body->torque_accumulator * body->inverse_moment_of_inertia;
        body->angular_velocity += angular_acceleration * dt;
        
        // 5. Integrate angular velocity to find new angle
        // angle_new = angle_old + av_new * dt
        body->angle += body->angular_velocity * dt;


        // 6. Clear all accumulated forces and torques for the next frame
        body->ClearAccumulators();
    }
}
