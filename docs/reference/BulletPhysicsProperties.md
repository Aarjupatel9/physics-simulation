# Bullet Physics Properties Reference

## Overview
This document provides a comprehensive reference for Bullet Physics properties used in the RealityCore engine. Use this as a quick reference instead of searching through code.

## Table of Contents
- [Friction Properties](#friction-properties)
- [Restitution Properties](#restitution-properties)
- [Mass and Inertia](#mass-and-inertia)
- [Damping Properties](#damping-properties)
- [Continuous Collision Detection (CCD)](#continuous-collision-detection-ccd)
- [Activation States](#activation-states)
- [Common Use Cases](#common-use-cases)

---

## Friction Properties

### Surface Friction (`setFriction`)
Controls how much objects resist sliding against each other.

```cpp
// Access Bullet rigid body
btRigidBody* bulletBody = rigidBody->getBulletRigidBody();
bulletBody->setFriction(value);
```

**Values:**
- `0.0f` - **Frictionless** (ice-like, endless sliding)
- `0.1f` - **Very Low** (smooth surfaces)
- `0.5f` - **Medium** (wood on wood)
- `0.8f` - **High** (rubber on concrete) - **Default in RealityCore**
- `1.0f+` - **Very High** (sandpaper-like)

### Rolling Friction (`setRollingFriction`)
Controls resistance when objects roll (spheres, cylinders).

```cpp
bulletBody->setRollingFriction(value);
```

**Values:**
- `0.0f` - **No rolling resistance** (endless rolling)
- `0.01f` - **Very Low** (ball bearing)
- `0.05f` - **Medium** (ball on smooth surface) - **Default in RealityCore**
- `0.1f` - **High** (ball on rough surface)
- `0.5f+` - **Very High** (ball on sand)

### Spinning Friction (`setSpinningFriction`)
Controls resistance when objects spin around their center.

```cpp
bulletBody->setSpinningFriction(value);
```

**Values:**
- `0.0f` - **No spin resistance**
- `0.05f` - **Low** - **Default in RealityCore**
- `0.1f+` - **Higher resistance**

---

## Restitution Properties

### Restitution (`setRestitution`)
Controls how "bouncy" collisions are (energy conservation).

```cpp
bulletBody->setRestitution(value);
```

**Values:**
- `0.0f` - **No bounce** (perfectly inelastic, clay-like)
- `0.3f` - **Low bounce** (wood)
- `0.7f` - **Medium bounce** (rubber ball)
- `0.9f` - **High bounce** (super ball)
- `1.0f` - **Perfect bounce** (no energy loss)

---

## Mass and Inertia

### Mass (`setMass`)
Controls object weight and inertia.

```cpp
rigidBody->setMass(value);
```

**Values:**
- `0.0f` - **Static object** (infinite mass, doesn't move)
- `0.1f` - **Very light** (feather)
- `1.0f` - **Standard** (1kg object)
- `10.0f+` - **Heavy** (bowling ball)

### Static vs Dynamic
```cpp
rigidBody->setStatic(true);  // Mass = 0, doesn't move
rigidBody->setStatic(false); // Dynamic, affected by forces
```

---

## Damping Properties

### Linear Damping (`setDamping`)
Reduces linear velocity over time (air resistance).

```cpp
bulletBody->setDamping(linearDamping, angularDamping);
```

**Linear Damping Values:**
- `0.0f` - **No damping** (space-like)
- `0.1f` - **Low** (air resistance)
- `0.5f` - **Medium** (water resistance)
- `0.9f` - **High** (thick fluid)

### Angular Damping
Reduces rotational velocity over time.

**Angular Damping Values:**
- `0.0f` - **No rotational damping**
- `0.1f` - **Low** (spinning in air)
- `0.5f` - **Medium** (spinning in water)
- `0.9f` - **High** (spinning in thick fluid)

---

## Continuous Collision Detection (CCD)

### Enable CCD
Prevents fast objects from passing through thin objects.

```cpp
rigidBody->enableCCD(motionThreshold, sweptSphereRadius);
```

**Parameters:**
- `motionThreshold` - Speed threshold to activate CCD (default: `0.1f`)
- `sweptSphereRadius` - Radius for swept sphere test (default: `0.5f`)

### Disable CCD
```cpp
rigidBody->disableCCD();
```

---

## Activation States

### Gravity
```cpp
rigidBody->setGravityEnabled(true);  // Affected by gravity
rigidBody->setGravityEnabled(false); // Floats in space
```

### Activation
```cpp
bulletBody->setActivationState(ACTIVE_TAG);           // Always active
bulletBody->setActivationState(ISLAND_SLEEPING);      // Can sleep
bulletBody->setActivationState(WANTS_DEACTIVATION);   // Wants to sleep
bulletBody->setActivationState(DISABLE_DEACTIVATION); // Never sleeps
```

---

## Common Use Cases

### 1. Frictionless Ball (Endless Movement)
```cpp
btRigidBody* bulletBody = rigidBody->getBulletRigidBody();
bulletBody->setFriction(0.0f);        // No surface friction
bulletBody->setRollingFriction(0.0f); // No rolling friction
bulletBody->setRestitution(1.0f);     // Perfect bounce (optional)
```

### 2. Realistic Ball
```cpp
bulletBody->setFriction(0.7f);        // Medium surface friction
bulletBody->setRollingFriction(0.05f); // Low rolling friction
bulletBody->setRestitution(0.8f);     // Good bounce
```

### 3. Static Ground/Wall
```cpp
rigidBody->setMass(0.0f);             // Static (infinite mass)
rigidBody->setStatic(true);           // Explicitly static
bulletBody->setFriction(0.8f);        // High friction for grip
```

### 4. Ice-like Surface
```cpp
bulletBody->setFriction(0.05f);       // Very low friction
bulletBody->setRollingFriction(0.01f); // Very low rolling friction
bulletBody->setRestitution(0.3f);     // Low bounce
```

### 5. Bouncy Ball
```cpp
bulletBody->setFriction(0.5f);        // Medium friction
bulletBody->setRollingFriction(0.02f); // Low rolling friction
bulletBody->setRestitution(0.9f);     // High bounce
```

### 6. Heavy Object (Bowling Ball)
```cpp
rigidBody->setMass(7.0f);             // 7kg mass
bulletBody->setFriction(0.6f);        // Good grip
bulletBody->setRollingFriction(0.08f); // Some rolling resistance
bulletBody->setRestitution(0.2f);     // Low bounce
```

### 7. Feather-like Object
```cpp
rigidBody->setMass(0.01f);            // Very light
bulletBody->setDamping(0.8f, 0.8f);   // High air resistance
bulletBody->setFriction(0.3f);        // Medium friction
bulletBody->setRestitution(0.1f);     // Very low bounce
```

---

## Code Access Patterns

### Through RealityCore Wrapper
```cpp
// Get BulletRigidBody from scene object
auto& ballObject = m_objects.back();
if (ballObject.physicsBody) {
    btRigidBody* bulletBody = ballObject.physicsBody->getBulletRigidBody();
    // Set properties...
}
```

### Direct Bullet Access
```cpp
// If you have direct access to btRigidBody
btRigidBody* body = /* ... */;
body->setFriction(0.5f);
body->setRollingFriction(0.05f);
body->setRestitution(0.8f);
```

---

## Performance Notes

1. **CCD** - Use sparingly, only for fast-moving objects
2. **High iteration counts** - Increase solver iterations for better accuracy
3. **Sleeping** - Let objects sleep when not moving to save CPU
4. **Collision margins** - Keep small but not zero (0.001f - 0.01f)

---

## Default Values in RealityCore

Based on `engine/src/bullet/BulletRigidBody.cpp`:

```cpp
// Default values set in constructor
m_rigidBody->setFriction(0.8f);         // High friction
m_rigidBody->setRollingFriction(0.05f);  // Medium rolling friction  
m_rigidBody->setSpinningFriction(0.05f); // Medium spinning friction
```

---

## Version Information

- **Bullet Physics Version**: 3.25 (LTS)
- **RealityCore Engine**: Current
- **Last Updated**: October 2025

---

*This document is part of the RealityCore physics engine reference materials.*
