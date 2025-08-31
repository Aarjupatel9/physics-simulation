#include <iostream>
#include <vector>
#include "World.h"
#include "RigidBody2D.h"

int main() {
    std::cout << "Starting 2D Physics Engine Simulation..." << std::endl;

    // Create a world with gravity pointing down
    World world(Vec2(0.0, -9.8)); // meters per second squared

    // Create a rigid body (e.g., a 1kg box)
    RigidBody2D* box = new RigidBody2D(0.0, 100.0, 1.0); // Start at (0, 100), 1kg mass
    world.AddBody(box);
    
    // Simulation parameters
    const float timeStep = 1.0f / 60.0f; // 60 updates per second
    const int numSteps = 600; // Simulate for 10 seconds

    // The simulation loop
    for (int i = 0; i < numSteps; ++i) {
        world.Update(timeStep);
        
        // Print the position every few steps to see it fall
        if (i % 10 == 0) {
            std::cout << "Step " << i << ": Position = " << box->position << std::endl;
        }
    }

    std::cout << "Simulation finished." << std::endl;
    
    // Clean up allocated memory
    delete box;

    return 0;
}
