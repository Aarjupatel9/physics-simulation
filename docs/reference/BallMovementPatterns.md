# Ball Movement Patterns Reference

## Overview
This document provides different initial velocity angles for creating interesting ball movement patterns in physics simulations.

## Table of Contents
- [Angle Patterns](#angle-patterns)
- [Movement Behaviors](#movement-behaviors)
- [Wall Collision Sequences](#wall-collision-sequences)
- [Implementation Examples](#implementation-examples)

---

## Angle Patterns

### Simple Patterns (Single Axis Dominant)

#### 1. Shallow Angles (0° - 30°)
**Characteristics**: Mostly horizontal movement with occasional vertical wall hits
```cpp
float angle = 15.0f; // Mostly X-direction movement
// Result: Long horizontal runs, occasional Z-wall bounces
```

#### 2. Moderate Angles (30° - 60°)
**Characteristics**: Balanced diagonal movement, hits multiple walls regularly
```cpp
float angle = 45.0f; // Perfect diagonal
// Result: Equal X and Z components, regular corner-to-corner bouncing
```

#### 3. Steep Angles (60° - 90°)
**Characteristics**: Mostly vertical movement with occasional horizontal wall hits
```cpp
float angle = 75.0f; // Mostly Z-direction movement
// Result: Long vertical runs, occasional X-wall bounces
```

### Complex Patterns (Multi-Wall Interactions)

#### 1. Golden Ratio Angle (~67°)
**Characteristics**: Creates complex, non-repeating patterns
```cpp
float angle = 67.0f; // Based on golden ratio for complex patterns
// Result: Long sequences before pattern repetition
```

#### 2. Prime Number Angles
**Characteristics**: Avoid simple repetitive patterns
```cpp
float angle = 37.0f; // Prime number angle
float angle = 53.0f; // Another prime angle
// Result: Complex bouncing sequences
```

#### 3. Irrational Angles
**Characteristics**: Theoretically never-repeating patterns
```cpp
float angle = 36.87f; // Arctan(3/4) - creates interesting ratios
float angle = 26.57f; // Arctan(1/2) - simple but effective
```

---

## Movement Behaviors

### Velocity Components Analysis

For a given angle θ and speed s:
```cpp
float radians = glm::radians(angle);
glm::vec3 velocity = glm::vec3(
    s * cos(radians),  // X component (horizontal)
    0.0f,              // Y component (no vertical movement)
    s * sin(radians)   // Z component (depth)
);
```

### Speed Distribution by Angle

| Angle | X Component | Z Component | Behavior |
|-------|-------------|-------------|----------|
| 0° | 1.39 | 0.00 | Pure horizontal |
| 15° | 1.34 | 0.36 | Mostly horizontal |
| 30° | 1.20 | 0.70 | Moderate diagonal |
| 45° | 0.98 | 0.98 | Perfect diagonal |
| 60° | 0.70 | 1.20 | Steep diagonal |
| 75° | 0.36 | 1.34 | Mostly vertical |
| 90° | 0.00 | 1.39 | Pure vertical |

---

## Wall Collision Sequences

### Pattern Predictions

#### 15° Angle Pattern
```
Start → East Wall → West Wall → East Wall → (repeat)
Occasional: North/South wall hits
```

#### 45° Angle Pattern  
```
Start → Corner → Opposite Corner → Corner → (repeat)
Regular: All four walls hit in sequence
```

#### 67° Angle Pattern (Current Demo)
```
Start → North Wall → East Wall → South Wall → West Wall → (complex sequence)
Result: Hits all walls in varied patterns
```

---

## Multi-Ball Scenarios

### Two-Ball Collision System
**Use Case**: Demonstrate ball-to-ball collisions with different movement patterns
```cpp
// Ball 1: 67° angle (golden ratio pattern)
float angle1 = 67.0f;
glm::vec3 velocity1 = glm::vec3(
    speed * cos(glm::radians(angle1)), 0.0f, speed * sin(glm::radians(angle1))
);
createSphere(glm::vec3(0.0f, 0.6f, 0.0f), 0.3f, 
             glm::vec3(1.0f, 0.0f, 0.0f), true, 1.0f, velocity1); // Red ball

// Ball 2: 200° angle (opposite quadrant)
float angle2 = 200.0f;
glm::vec3 velocity2 = glm::vec3(
    speed * cos(glm::radians(angle2)), 0.0f, speed * sin(glm::radians(angle2))
);
createSphere(glm::vec3(-1.5f, 0.6f, -1.5f), 0.3f, 
             glm::vec3(0.0f, 1.0f, 0.0f), true, 1.0f, velocity2); // Green ball
```

### Optimal Two-Ball Angle Combinations
**For Maximum Collision Probability**:
- **67° + 200°**: Current demo setup - creates crossing paths
- **45° + 225°**: Perfect diagonal opposites
- **30° + 150°**: Shallow angles with good intersection
- **90° + 270°**: Pure vertical opposites (parallel paths)

### Ball-to-Ball Collision Physics
With perfect restitution (1.0f), ball-to-ball collisions should:
1. **Conserve momentum**: Total momentum before = Total momentum after
2. **Conserve energy**: Total kinetic energy preserved
3. **Exchange velocities**: In head-on collisions, balls exchange velocities
4. **Deflect at angles**: In glancing collisions, both balls deflect

---

## Implementation Examples

### Basic Angled Movement
```cpp
void createAngledBall(float angle) {
    float speed = 1.39f; // 5 km/h
    float radians = glm::radians(angle);
    glm::vec3 velocity = glm::vec3(
        speed * cos(radians),
        0.0f,
        speed * sin(radians)
    );
    
    createSphere(glm::vec3(0.0f, 0.6f, 0.0f), 0.3f, 
                 glm::vec3(1.0f, 0.0f, 0.0f), true, 1.0f, velocity);
}
```

### Random Angle Generation
```cpp
void createRandomAngledBall() {
    // Generate random angle between 0 and 360 degrees
    float randomAngle = static_cast<float>(rand()) / RAND_MAX * 360.0f;
    createAngledBall(randomAngle);
}
```

### Predefined Interesting Angles
```cpp
enum class BallPattern {
    HORIZONTAL,     // 0°
    SHALLOW,        // 15°
    MODERATE,       // 35°
    DIAGONAL,       // 45°
    GOLDEN,         // 67°
    STEEP,          // 75°
    VERTICAL,       // 90°
    COMPLEX_1,      // 123°
    COMPLEX_2,      // 200°
    COMPLEX_3,      // 285°
    NEAR_HORIZONTAL // 330°
};

float getPatternAngle(BallPattern pattern) {
    switch(pattern) {
        case BallPattern::HORIZONTAL: return 0.0f;
        case BallPattern::SHALLOW: return 15.0f;
        case BallPattern::MODERATE: return 35.0f;
        case BallPattern::DIAGONAL: return 45.0f;
        case BallPattern::GOLDEN: return 67.0f;
        case BallPattern::STEEP: return 75.0f;
        case BallPattern::VERTICAL: return 90.0f;
        case BallPattern::COMPLEX_1: return 123.0f;
        case BallPattern::COMPLEX_2: return 200.0f;
        case BallPattern::COMPLEX_3: return 285.0f;
        case BallPattern::NEAR_HORIZONTAL: return 330.0f;
        default: return 45.0f;
    }
}
```

---

## Testing Different Patterns

### Quick Pattern Testing
To test different patterns in BallCollision2Scene.cpp, simply change the angle:

```cpp
// In BallCollision2Scene.cpp, line ~80:
float randomAngle = 67.0f; // Change this value

// Try these interesting angles:
float randomAngle = 15.0f;  // Shallow - mostly horizontal
float randomAngle = 35.0f;  // Moderate diagonal
float randomAngle = 67.0f;  // Golden ratio - complex patterns
float randomAngle = 123.0f; // Obtuse angle - different quadrant
float randomAngle = 200.0f; // Third quadrant start
float randomAngle = 285.0f; // Fourth quadrant - steep
float randomAngle = 330.0f; // Nearly horizontal, negative Z
```

### Expected Behaviors

#### Angles 0° - 90° (First Quadrant)
- **Start direction**: Positive X, Positive Z
- **First wall hit**: Usually North or East wall
- **Pattern**: Clockwise-ish bouncing

#### Angles 90° - 180° (Second Quadrant)  
- **Start direction**: Negative X, Positive Z
- **First wall hit**: Usually North or West wall
- **Pattern**: Counter-clockwise-ish bouncing

#### Angles 180° - 270° (Third Quadrant)
- **Start direction**: Negative X, Negative Z  
- **First wall hit**: Usually South or West wall
- **Pattern**: Clockwise-ish bouncing

#### Angles 270° - 360° (Fourth Quadrant)
- **Start direction**: Positive X, Negative Z
- **First wall hit**: Usually South or East wall
- **Pattern**: Counter-clockwise-ish bouncing

---

## Performance Considerations

### Energy Conservation
With perfect restitution (1.0f), the ball should maintain constant speed:
```cpp
// Theoretical: Speed should remain constant at 1.39 m/s
// Reality: Minor floating-point precision losses may occur
```

### Pattern Complexity vs Performance
- **Simple angles** (0°, 45°, 90°): Predictable, less CPU overhead
- **Complex angles** (67°, 123°): More interesting, slightly more collision calculations
- **Random angles**: Most dynamic, but unpredictable for testing

---

## Troubleshooting

### Ball Loses Speed Over Time
**Cause**: Imperfect collision resolution or floating-point precision
**Solution**: 
```cpp
// Ensure perfect restitution
bulletBody->setRestitution(1.0f);
// Reduce collision margins
shape->setMargin(0.0001f);
```

### Ball Gets Stuck in Corner
**Cause**: Collision margin issues or simultaneous wall hits
**Solution**: Slightly adjust initial position or reduce collision margins

### Predictable Patterns
**Cause**: Simple angle ratios create repeating patterns
**Solution**: Use irrational angles or prime-number-based angles

---

## Version Information

- **RealityCore Engine**: Current
- **Physics Engine**: Bullet Physics 3.25
- **Last Updated**: October 2025

---

*This document is part of the RealityCore physics engine reference materials.*
