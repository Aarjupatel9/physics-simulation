# Camera Positions Reference

## Overview
This document provides optimal camera configurations for different viewing scenarios in the RealityCore engine.

## Table of Contents
- [Top-Down Views](#top-down-views)
- [Angled Views](#angled-views)
- [Side Views](#side-views)
- [First Person Views](#first-person-views)
- [Cinematic Views](#cinematic-views)

---

## Top-Down Views

### 1. Perfect Top-Down (Bird's Eye)
**Use Case**: Ideal for observing ball movement, collisions, and physics simulations
```cpp
m_camera->setPosition(glm::vec3(0.0f, 8.0f, 0.0f));  // Directly above center
m_camera->setYaw(0.0f);     // Look straight ahead (towards -Z)
m_camera->setPitch(-85.0f); // Look almost straight down
```
**Best For**: Ball collision scenes, top-down games, physics debugging

### 2. High Overhead View
**Use Case**: Wide area observation with slight perspective
```cpp
m_camera->setPosition(glm::vec3(0.0f, 12.0f, 0.0f)); // Higher up for wider view
m_camera->setYaw(0.0f);     
m_camera->setPitch(-80.0f); // Slightly less steep
```
**Best For**: Large scenes, multiple object tracking

### 3. Centered Top-Down with Rotation
**Use Case**: Top-down view with ability to rotate around center
```cpp
m_camera->setPosition(glm::vec3(0.0f, 6.0f, 0.0f));  
m_camera->setYaw(45.0f);    // Rotated view
m_camera->setPitch(-75.0f); 
```
**Best For**: Architectural views, strategy game perspectives

---

## Angled Views

### 1. Isometric-Style View
**Use Case**: 3D perspective with good ground visibility
```cpp
m_camera->setPosition(glm::vec3(5.0f, 5.0f, 5.0f));  // Corner position
m_camera->setYaw(-45.0f);   // Look towards center
m_camera->setPitch(-35.0f); // Moderate downward angle
```
**Best For**: 3D games, architectural visualization

### 2. Shallow Angled View
**Use Case**: Ground-level perspective with slight elevation
```cpp
m_camera->setPosition(glm::vec3(3.0f, 2.0f, 3.0f));  
m_camera->setYaw(-45.0f);   
m_camera->setPitch(-15.0f); // Shallow angle
```
**Best For**: Racing games, ground-level action

### 3. Steep Angled View
**Use Case**: High angle for better scene overview
```cpp
m_camera->setPosition(glm::vec3(4.0f, 6.0f, 4.0f));  
m_camera->setYaw(-45.0f);   
m_camera->setPitch(-60.0f); // Steep downward angle
```
**Best For**: RTS games, simulation overview

---

## Side Views

### 1. Pure Side View (2D-like)
**Use Case**: Profile view for 2D-style games or analysis
```cpp
m_camera->setPosition(glm::vec3(8.0f, 3.0f, 0.0f));  // Side position
m_camera->setYaw(-90.0f);   // Look towards center from side
m_camera->setPitch(0.0f);   // Level horizon
```
**Best For**: Platformer games, side-scrolling, physics analysis

### 2. Elevated Side View
**Use Case**: Side view with height for better perspective
```cpp
m_camera->setPosition(glm::vec3(6.0f, 4.0f, 0.0f));  
m_camera->setYaw(-90.0f);   
m_camera->setPitch(-20.0f); // Slight downward angle
```
**Best For**: Sports games, side-view action games

---

## First Person Views

### 1. Ground Level First Person
**Use Case**: Human-eye level perspective
```cpp
m_camera->setPosition(glm::vec3(0.0f, 1.7f, 0.0f));  // Human eye height
m_camera->setYaw(0.0f);     
m_camera->setPitch(0.0f);   // Level horizon
```
**Best For**: FPS games, walking simulators

### 2. Low Ground View
**Use Case**: Close to ground perspective
```cpp
m_camera->setPosition(glm::vec3(0.0f, 0.5f, 0.0f));  // Low to ground
m_camera->setYaw(0.0f);     
m_camera->setPitch(0.0f);   
```
**Best For**: Small object perspective, insect-eye view

---

## Cinematic Views

### 1. Dramatic Low Angle
**Use Case**: Cinematic upward perspective
```cpp
m_camera->setPosition(glm::vec3(2.0f, 1.0f, 2.0f));  
m_camera->setYaw(-45.0f);   
m_camera->setPitch(30.0f);  // Look upward
```
**Best For**: Hero shots, dramatic scenes

### 2. Sweeping Overhead
**Use Case**: Cinematic aerial view
```cpp
m_camera->setPosition(glm::vec3(8.0f, 10.0f, 8.0f)); 
m_camera->setYaw(-45.0f);   
m_camera->setPitch(-45.0f); 
```
**Best For**: Establishing shots, scene transitions

---

## Scene-Specific Configurations

### Ball Collision Scene (5x5 Ground)
**Optimal Configuration**: Perfect Top-Down
```cpp
// Position: Directly above center, high enough to see entire 5x5 area
m_camera->setPosition(glm::vec3(0.0f, 8.0f, 0.0f));  
m_camera->setYaw(0.0f);     // No rotation needed
m_camera->setPitch(-85.0f); // Almost straight down
```
**Why**: 
- Sees entire 5x5 ground area
- Perfect for tracking ball bouncing between walls
- No perspective distortion
- Clear view of collision points

### Large Terrain Scene (20x20+ Ground)
**Optimal Configuration**: High Overhead
```cpp
m_camera->setPosition(glm::vec3(0.0f, 15.0f, 0.0f)); 
m_camera->setYaw(0.0f);     
m_camera->setPitch(-75.0f); 
```

### Multi-Object Physics Scene
**Optimal Configuration**: Isometric-Style
```cpp
m_camera->setPosition(glm::vec3(6.0f, 6.0f, 6.0f));  
m_camera->setYaw(-45.0f);   
m_camera->setPitch(-30.0f); 
```

---

## Camera Control Tips

### Smooth Transitions
```cpp
// Gradually change camera position over time
glm::vec3 targetPos = glm::vec3(0.0f, 8.0f, 0.0f);
glm::vec3 currentPos = m_camera->getPosition();
glm::vec3 newPos = glm::mix(currentPos, targetPos, deltaTime * 2.0f);
m_camera->setPosition(newPos);
```

### Dynamic Camera Following
```cpp
// Follow ball position while maintaining height
glm::vec3 ballPos = ball->getPosition();
m_camera->setPosition(glm::vec3(ballPos.x, 8.0f, ballPos.z));
```

### Boundary Constraints
```cpp
// Keep camera within bounds
glm::vec3 pos = m_camera->getPosition();
pos.x = glm::clamp(pos.x, -10.0f, 10.0f);
pos.z = glm::clamp(pos.z, -10.0f, 10.0f);
pos.y = glm::clamp(pos.y, 2.0f, 20.0f);
m_camera->setPosition(pos);
```

---

## Quick Reference Table

| View Type | Position | Yaw | Pitch | Best For |
|-----------|----------|-----|-------|----------|
| **Perfect Top-Down** | (0, 8, 0) | 0° | -85° | Ball physics, collisions |
| **Isometric** | (5, 5, 5) | -45° | -35° | 3D games, architecture |
| **Side View** | (8, 3, 0) | -90° | 0° | Platformers, 2D-style |
| **First Person** | (0, 1.7, 0) | 0° | 0° | FPS, walking sims |
| **Cinematic** | (8, 10, 8) | -45° | -45° | Cutscenes, establishing |

---

## Implementation Template

```cpp
void setOptimalCameraView(CameraViewType viewType) {
    switch(viewType) {
        case PERFECT_TOP_DOWN:
            m_camera->setPosition(glm::vec3(0.0f, 8.0f, 0.0f));
            m_camera->setYaw(0.0f);
            m_camera->setPitch(-85.0f);
            break;
            
        case ISOMETRIC:
            m_camera->setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
            m_camera->setYaw(-45.0f);
            m_camera->setPitch(-35.0f);
            break;
            
        case SIDE_VIEW:
            m_camera->setPosition(glm::vec3(8.0f, 3.0f, 0.0f));
            m_camera->setYaw(-90.0f);
            m_camera->setPitch(0.0f);
            break;
            
        // Add more cases as needed
    }
}
```

---

## Version Information

- **RealityCore Engine**: Current
- **Camera System**: GLM-based
- **Last Updated**: October 2025

---

*This document is part of the RealityCore physics engine reference materials.*
