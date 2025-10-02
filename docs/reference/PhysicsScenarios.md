# Physics Scenarios Reference

## Overview
This document provides ready-to-use physics configurations for common scenarios in the RealityCore engine.

## Table of Contents
- [Ball Movement Scenarios](#ball-movement-scenarios)
- [Surface Types](#surface-types)
- [Object Behaviors](#object-behaviors)
- [Collision Scenarios](#collision-scenarios)
- [Performance Considerations](#performance-considerations)

---

## Ball Movement Scenarios

### 1. Endless Movement Ball (Frictionless)
**Use Case**: Ball that moves forever without stopping
```cpp
// Zero friction for endless movement
btRigidBody* bulletBody = ballObject.physicsBody->getBulletRigidBody();
bulletBody->setFriction(0.0f);        // No surface friction
bulletBody->setRollingFriction(0.0f); // No rolling friction
bulletBody->setRestitution(1.0f);     // Perfect bounce (optional)
```

### 2. Realistic Ball Movement
**Use Case**: Natural ball physics with gradual stopping
```cpp
bulletBody->setFriction(0.7f);        // Medium surface friction
bulletBody->setRollingFriction(0.05f); // Low rolling friction
bulletBody->setRestitution(0.8f);     // Good bounce
```

### 3. Ice Hockey Puck
**Use Case**: Low friction sliding with minimal rolling resistance
```cpp
bulletBody->setFriction(0.02f);       // Very low friction
bulletBody->setRollingFriction(0.001f); // Minimal rolling resistance
bulletBody->setRestitution(0.3f);     // Low bounce
```

### 4. Bowling Ball
**Use Case**: Heavy ball with good grip and controlled movement
```cpp
rigidBody->setMass(7.0f);             // 7kg mass
bulletBody->setFriction(0.6f);        // Good grip
bulletBody->setRollingFriction(0.08f); // Some rolling resistance
bulletBody->setRestitution(0.2f);     // Low bounce
```

---

## Surface Types

### 1. Ice Surface
**Use Case**: Slippery surface for sliding objects
```cpp
// Apply to ground/surface objects
bulletBody->setFriction(0.05f);       // Very slippery
bulletBody->setRestitution(0.1f);     // Low bounce
```

### 2. Rubber Surface
**Use Case**: High grip surface with good bounce
```cpp
bulletBody->setFriction(0.9f);        // High grip
bulletBody->setRestitution(0.8f);     // High bounce
```

### 3. Concrete Surface
**Use Case**: Standard ground surface
```cpp
bulletBody->setFriction(0.8f);        // Good grip
bulletBody->setRestitution(0.3f);     // Medium bounce
```

### 4. Sand Surface
**Use Case**: High resistance surface
```cpp
bulletBody->setFriction(0.9f);        // High friction
bulletBody->setRollingFriction(0.3f); // High rolling resistance
bulletBody->setRestitution(0.1f);     // Low bounce
```

---

## Object Behaviors

### 1. Feather (Light Object)
**Use Case**: Very light object affected by air resistance
```cpp
rigidBody->setMass(0.01f);            // Very light
bulletBody->setDamping(0.8f, 0.8f);   // High air resistance
bulletBody->setFriction(0.3f);        // Medium friction
bulletBody->setRestitution(0.1f);     // Very low bounce
```

### 2. Steel Ball (Heavy Object)
**Use Case**: Dense, heavy object
```cpp
rigidBody->setMass(10.0f);            // Heavy
bulletBody->setFriction(0.5f);        // Medium friction
bulletBody->setRollingFriction(0.02f); // Low rolling resistance
bulletBody->setRestitution(0.4f);     // Medium bounce
```

### 3. Rubber Ball (Bouncy Object)
**Use Case**: High bounce, medium weight
```cpp
rigidBody->setMass(0.5f);             // Medium weight
bulletBody->setFriction(0.6f);        // Good grip
bulletBody->setRollingFriction(0.03f); // Low rolling resistance
bulletBody->setRestitution(0.9f);     // High bounce
```

### 4. Static Wall/Ground
**Use Case**: Immovable objects
```cpp
rigidBody->setMass(0.0f);             // Static (infinite mass)
rigidBody->setStatic(true);           // Explicitly static
bulletBody->setFriction(0.8f);        // High friction for grip
bulletBody->setRestitution(0.5f);     // Medium bounce
```

---

## Collision Scenarios

### 1. Perfectly Elastic Collision
**Use Case**: No energy loss in collisions
```cpp
// Apply to both objects
bulletBody->setRestitution(1.0f);     // Perfect energy conservation
bulletBody->setFriction(0.0f);        // No friction loss
bulletBody->setRollingFriction(0.0f); // No rolling loss
```

### 2. Perfectly Inelastic Collision
**Use Case**: Maximum energy loss (objects stick together)
```cpp
bulletBody->setRestitution(0.0f);     // No bounce
bulletBody->setFriction(0.9f);        // High friction
```

### 3. Billiard Ball Collision
**Use Case**: Realistic ball-to-ball collisions
```cpp
bulletBody->setRestitution(0.95f);    // High bounce
bulletBody->setFriction(0.1f);        // Low surface friction
bulletBody->setRollingFriction(0.02f); // Low rolling friction
```

---

## Performance Considerations

### 1. High-Speed Objects
**Use Case**: Fast-moving objects that might tunnel through thin walls
```cpp
// Enable Continuous Collision Detection (CCD)
rigidBody->enableCCD(0.1f, 0.5f);     // motionThreshold, sweptSphereRadius
```

### 2. Static Objects (Performance Optimization)
**Use Case**: Objects that never move
```cpp
rigidBody->setMass(0.0f);             // Static
rigidBody->setStatic(true);           // Explicitly static
// Static objects don't participate in physics calculations
```

### 3. Sleeping Objects
**Use Case**: Objects that can go to sleep when not moving
```cpp
bulletBody->setActivationState(ISLAND_SLEEPING);      // Can sleep
bulletBody->setSleepingThresholds(0.8f, 1.0f);       // Linear, angular thresholds
```

### 4. Always Active Objects
**Use Case**: Objects that should never sleep
```cpp
bulletBody->setActivationState(DISABLE_DEACTIVATION); // Never sleeps
```

---

## Code Templates

### Complete Ball Setup Template
```cpp
void setupBall(BulletRigidBody* rigidBody, BallType type) {
    btRigidBody* bulletBody = rigidBody->getBulletRigidBody();
    
    switch(type) {
        case ENDLESS_MOVEMENT:
            bulletBody->setFriction(0.0f);
            bulletBody->setRollingFriction(0.0f);
            bulletBody->setRestitution(1.0f);
            break;
            
        case REALISTIC:
            bulletBody->setFriction(0.7f);
            bulletBody->setRollingFriction(0.05f);
            bulletBody->setRestitution(0.8f);
            break;
            
        case ICE_HOCKEY_PUCK:
            bulletBody->setFriction(0.02f);
            bulletBody->setRollingFriction(0.001f);
            bulletBody->setRestitution(0.3f);
            break;
            
        case BOWLING_BALL:
            rigidBody->setMass(7.0f);
            bulletBody->setFriction(0.6f);
            bulletBody->setRollingFriction(0.08f);
            bulletBody->setRestitution(0.2f);
            break;
    }
}
```

### Complete Surface Setup Template
```cpp
void setupSurface(BulletRigidBody* rigidBody, SurfaceType type) {
    btRigidBody* bulletBody = rigidBody->getBulletRigidBody();
    
    // All surfaces are static
    rigidBody->setMass(0.0f);
    rigidBody->setStatic(true);
    
    switch(type) {
        case ICE:
            bulletBody->setFriction(0.05f);
            bulletBody->setRestitution(0.1f);
            break;
            
        case RUBBER:
            bulletBody->setFriction(0.9f);
            bulletBody->setRestitution(0.8f);
            break;
            
        case CONCRETE:
            bulletBody->setFriction(0.8f);
            bulletBody->setRestitution(0.3f);
            break;
            
        case SAND:
            bulletBody->setFriction(0.9f);
            bulletBody->setRestitution(0.1f);
            break;
    }
}
```

---

## Troubleshooting

### Ball Stops Moving Despite Zero Friction
**Possible Causes:**
1. **Collision margins**: Very small movements might be absorbed by collision margins
2. **Solver precision**: Bullet's solver might introduce tiny energy losses
3. **Floating point precision**: Very small velocities might be clamped to zero
4. **Sleeping**: Object might be going to sleep due to low velocity

**Solutions:**
```cpp
// Prevent sleeping
bulletBody->setActivationState(DISABLE_DEACTIVATION);

// Reduce collision margins
bulletBody->getCollisionShape()->setMargin(0.0001f);

// Increase solver iterations for better precision
btContactSolverInfo& solverInfo = dynamicsWorld->getSolverInfo();
solverInfo.m_numIterations = 100; // Higher iterations
```

### Objects Passing Through Each Other
**Possible Causes:**
1. **High speed**: Objects moving too fast for discrete collision detection
2. **Thin objects**: Objects too thin relative to movement speed
3. **Large time steps**: Physics time step too large

**Solutions:**
```cpp
// Enable CCD for fast objects
rigidBody->enableCCD(0.1f, 0.5f);

// Reduce physics time step
world->stepSimulation(deltaTime, 10, 1.0f/120.0f); // 120 Hz physics
```

---

## Version Information

- **Bullet Physics Version**: 3.25 (LTS)
- **RealityCore Engine**: Current
- **Last Updated**: October 2025

---

*This document is part of the RealityCore physics engine reference materials.*
