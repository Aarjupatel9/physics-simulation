#ifndef WORLD_H
#define WORLD_H

#include "RigidBody2D.h"
#include <vector>

class World {
public:
    // Gravity constant
    Vec2 gravity;

    // List of all rigid bodies in the simulation
    std::vector<RigidBody2D*> bodies;

    World(const Vec2& gravity);

    void AddBody(RigidBody2D* body);
    void Update(float dt);
};

#endif // WORLD_H
