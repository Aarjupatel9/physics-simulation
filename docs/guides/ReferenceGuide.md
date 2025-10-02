# RealityCore Physics Engine - Reference Documentation

## Overview
This folder contains comprehensive reference documentation for the RealityCore physics engine, built on Bullet Physics 3.25.

## 📚 Available References

### 1. [BulletPhysicsProperties.md](./BulletPhysicsProperties.md)
**Complete reference for Bullet Physics properties and parameters**
- Friction properties (surface, rolling, spinning)
- Restitution (bounciness) settings
- Mass and inertia configuration
- Damping properties (air resistance)
- Continuous Collision Detection (CCD)
- Activation states and performance settings
- Code access patterns and default values

### 2. [PhysicsScenarios.md](./PhysicsScenarios.md)
**Ready-to-use physics configurations for common scenarios**
- Ball movement scenarios (endless, realistic, ice hockey, bowling)
- Surface types (ice, rubber, concrete, sand)
- Object behaviors (feather, steel ball, rubber ball, static objects)
- Collision scenarios (elastic, inelastic, billiard balls)
- Performance considerations and troubleshooting

### 3. [CameraPositions.md](./CameraPositions.md)
**Optimal camera configurations for different viewing scenarios**
- Top-down views (perfect bird's eye, overhead, centered)
- Angled views (isometric, shallow, steep)
- Side views (2D-like, elevated)
- First person views (ground level, low ground)
- Cinematic views (dramatic, sweeping)
- Scene-specific configurations and implementation templates

## 🎯 Quick Access

### For Friction Settings
```cpp
// Frictionless (endless movement)
bulletBody->setFriction(0.0f);
bulletBody->setRollingFriction(0.0f);

// Realistic movement
bulletBody->setFriction(0.7f);
bulletBody->setRollingFriction(0.05f);
```

### For Bounce Settings
```cpp
// No bounce (clay-like)
bulletBody->setRestitution(0.0f);

// Perfect bounce (no energy loss)
bulletBody->setRestitution(1.0f);
```

### For Static Objects
```cpp
// Make object static (immovable)
rigidBody->setMass(0.0f);
rigidBody->setStatic(true);
```

### For High-Speed Objects
```cpp
// Enable CCD to prevent tunneling
rigidBody->enableCCD(0.1f, 0.5f);
```

### For Camera Positioning
```cpp
// Perfect top-down view for ball physics
m_camera->setPosition(glm::vec3(0.0f, 8.0f, 0.0f));
m_camera->setYaw(0.0f);
m_camera->setPitch(-85.0f);

// Isometric 3D view
m_camera->setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
m_camera->setYaw(-45.0f);
m_camera->setPitch(-35.0f);
```

## 🔧 Common Issues & Solutions

### Ball Stops Moving Despite Zero Friction
1. **Check sleeping**: `bulletBody->setActivationState(DISABLE_DEACTIVATION);`
2. **Reduce collision margins**: `shape->setMargin(0.0001f);`
3. **Increase solver iterations**: `solverInfo.m_numIterations = 100;`

### Objects Pass Through Each Other
1. **Enable CCD**: `rigidBody->enableCCD(0.1f, 0.5f);`
2. **Reduce time step**: `world->stepSimulation(deltaTime, 10, 1.0f/120.0f);`
3. **Check object speeds**: Very fast objects need special handling

### Poor Performance
1. **Use static objects**: `rigidBody->setStatic(true);` for immovable objects
2. **Enable sleeping**: `bulletBody->setActivationState(ISLAND_SLEEPING);`
3. **Reduce solver iterations**: Balance accuracy vs performance

## 📖 Usage Examples

### Creating a Frictionless Ball
```cpp
// From BallCollision2Scene.cpp
btRigidBody* bulletBody = ballObject.physicsBody->getBulletRigidBody();
bulletBody->setFriction(0.0f);        // No surface friction
bulletBody->setRollingFriction(0.0f); // No rolling friction
bulletBody->setActivationState(DISABLE_DEACTIVATION); // Never sleeps
bulletBody->getCollisionShape()->setMargin(0.0001f);  // Precise collisions
```

### Creating a Static Ground
```cpp
// Static ground with good grip
rigidBody->setMass(0.0f);             // Static (infinite mass)
rigidBody->setStatic(true);           // Explicitly static
bulletBody->setFriction(0.8f);        // High friction for grip
bulletBody->setRestitution(0.3f);     // Medium bounce
```

## 🔍 Finding Information

1. **Property Values**: Check `BulletPhysicsProperties.md` → specific property section
2. **Use Cases**: Check `PhysicsScenarios.md` → scenario type
3. **Troubleshooting**: Check both documents → troubleshooting sections
4. **Code Examples**: Both documents contain ready-to-use code snippets

## 📝 Version Information

- **Bullet Physics**: 3.25 (LTS)
- **RealityCore Engine**: Current
- **Documentation Updated**: October 2025

---

**💡 Tip**: Use Ctrl+F (Cmd+F on Mac) to quickly search for specific properties or scenarios within the reference documents.

*This reference documentation is maintained alongside the RealityCore physics engine development.*
