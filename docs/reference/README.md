# Reference Materials

Technical reference documentation for physics properties, configurations, and common scenarios.

## 📚 Available References

### 🔧 Physics Configuration
- **[BulletPhysicsProperties.md](./BulletPhysicsProperties.md)**
  - Complete reference for Bullet Physics rigid body properties
  - Mass, friction, restitution, damping parameters
  - Collision detection settings and optimization
  - **Use Case**: Configuring realistic physics behavior

### 🎯 Physics Scenarios
- **[PhysicsScenarios.md](./PhysicsScenarios.md)**
  - Common physics scenarios and their solutions
  - Troubleshooting guide for physics issues
  - Performance optimization techniques
  - **Use Case**: Solving physics problems and debugging

### ⚽ Ball Physics Analysis
- **[BallMovementPatterns.md](./BallMovementPatterns.md)**
  - Ball movement angle analysis and patterns
  - Collision prediction and trajectory planning
  - Multi-ball collision scenarios
  - **Use Case**: Designing ball physics demonstrations

### 📹 Camera Configuration
- **[CameraPositions.md](./CameraPositions.md)**
  - Camera position and angle references
  - Optimal viewing configurations for different scenarios
  - Top-down, side-view, and perspective setups
  - **Use Case**: Setting up optimal scene viewing

## 🎯 Quick Reference Tables

### Common Physics Values
| Property | Realistic | Game-like | Demo |
|----------|-----------|-----------|------|
| **Gravity** | -9.81 m/s² | -20.0 m/s² | -9.81 m/s² |
| **Ball Mass** | 0.45 kg | 1.0 kg | 1.0 kg |
| **Restitution** | 0.7 | 1.0 | 1.0 |
| **Friction** | 0.3 | 0.0 | 0.0 |

### Common Camera Positions
| View Type | Position | Target | Use Case |
|-----------|----------|---------|----------|
| **Top-Down** | (0, 10, 0) | (0, 0, 0) | Ball tracking |
| **Perspective** | (5, 3, 5) | (0, 0, 0) | General viewing |
| **Side View** | (10, 2, 0) | (0, 0, 0) | Collision analysis |

### Ball Movement Angles
| Angle | Pattern | Walls Hit | Collision Type |
|-------|---------|-----------|----------------|
| **45°** | Diagonal | 2-4 walls | Regular bouncing |
| **67°** | Golden ratio | All walls | Complex patterns |
| **30°** | Shallow | 2 walls | Simple bouncing |
| **90°** | Perpendicular | 2 walls | Back-and-forth |

## 📖 How to Use These References

### For Scene Development
1. **Start with**: [PhysicsScenarios.md](./PhysicsScenarios.md) for common setups
2. **Configure physics**: [BulletPhysicsProperties.md](./BulletPhysicsProperties.md) for parameters
3. **Set camera**: [CameraPositions.md](./CameraPositions.md) for optimal viewing
4. **Ball physics**: [BallMovementPatterns.md](./BallMovementPatterns.md) for movement design

### For Troubleshooting
1. **Physics issues**: Check [PhysicsScenarios.md](./PhysicsScenarios.md) troubleshooting section
2. **Performance problems**: Review optimization techniques in physics scenarios
3. **Collision issues**: Analyze ball movement patterns and collision settings
4. **Visual problems**: Adjust camera positions for better visibility

### For Experimentation
- **Try different angles**: Use ball movement patterns as starting points
- **Adjust physics**: Experiment with different property combinations
- **Camera angles**: Test various viewing positions for your use case
- **Scenario variations**: Modify existing scenarios for new demonstrations

---

**Reference Type**: Technical Documentation  
**Last Updated**: October 2025  
**Covers**: Bullet Physics 3.25, RealityCore Engine
