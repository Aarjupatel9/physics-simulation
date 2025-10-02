# RealityCore Demo Scenes

This folder contains interactive demo scenes that showcase different aspects of the RealityCore physics engine. Each demo is a separate executable that links to the shared engine library.

## 🎮 Available Demo Scenes

### 1. Ball Collision 2 (`BallCollision2`)
- **Features**: Two-ball collision physics demonstration with perfect elasticity
- **Physics**: Ball-to-ball and ball-to-wall collisions with momentum conservation
- **Controls**: Automatic simulation with real-time collision tracking
- **Use Case**: Advanced collision physics validation and demonstration

### 2. Ball Collision Demo (`BallCollisionDemo`)
- **Features**: 15 balls with collision detection on a 5x5 meter bounded plane
- **Physics**: Conservative collisions (no energy loss, no friction)
- **Controls**: Press 'P' to start physics simulation
- **Use Case**: Multi-object collision performance testing

### 3. Ball Free Fall (`BallFreeFall`)
- **Features**: Simple ball falling under gravity in open space
- **Physics**: Basic gravity simulation without ground collision
- **Use Case**: Fundamental physics testing and debugging

### 4. Basic Demo (`BasicDemo`)
- **Features**: Simple physics demo with cube and sphere
- **Physics**: Gravity simulation with ground collision
- **Use Case**: Basic physics engine testing and validation

### 5. Basic Ground Ball (`BasicGroundBall`)
- **Features**: Single ball collision with ground plane
- **Physics**: Ball-ground collision with realistic physics
- **Use Case**: Ground collision testing and validation

### 6. Terrain Demo (`TerrainDemo`)
- **Features**: Beautiful procedural terrain with skybox
- **Rendering**: Advanced terrain generation and environmental effects
- **Use Case**: Graphics and rendering performance testing

## 🚀 Running Demos

### Quick Start
```bash
# Build all demos
./build.sh

# Run any demo from the organized bin/ directory
./build/bin/BallCollision2
./build/bin/BallCollisionDemo
./build/bin/BallFreeFall
./build/bin/BasicDemo
./build/bin/BasicGroundBall
./build/bin/TerrainDemo
```

### Windows
```cmd
# Build all demos
build.bat

# Run any demo
build\bin\BallCollision2.exe
build\bin\BallCollisionDemo.exe
build\bin\BallFreeFall.exe
build\bin\BasicDemo.exe
build\bin\BasicGroundBall.exe
build\bin\TerrainDemo.exe
```

## 🎯 Universal Controls

All demos support the following controls:

### Camera Controls
- **Movement**: W/A/S/D (forward/left/backward/right)
- **Up/Down**: I/K keys
- **Look Around**: Mouse movement
- **Sprint**: Left Shift (hold)
- **Zoom**: Mouse scroll wheel or +/- keys

### System Controls
- **Toggle Controls**: B key (releases/captures mouse cursor)
- **Toggle FPS Display**: F key
- **Exit**: Esc key

### Demo-Specific Controls
- **Ball Collision Demo**: Press 'P' to start physics simulation
- **Other Demos**: Automatic simulation start

## 🏗️ Creating New Demo Scenes

### Using the Scene Generator
```bash
# Create a new scene
./create_scene.sh <SceneName>

# Example: Create a new collision test scene
./create_scene.sh CollisionTest
```

### Manual Steps After Generation
1. **Add to CMakeLists.txt**: Add `add_subdirectory(<SceneName>)` to `demos/CMakeLists.txt`
2. **Build**: `cmake -B build && cmake --build build --target <SceneName>`
3. **Run**: `./build/bin/<SceneName>`

### What the Script Creates
The script automatically generates:
- **Scene Directory**: `demos/<SceneName>/`
- **Header File**: `<SceneName>Scene.h` (with proper BaseScene inheritance)
- **Implementation**: `<SceneName>Scene.cpp` (with all required methods)
- **Main File**: `main.cpp` (with complete GLFW setup)
- **CMakeLists.txt**: Proper linking and configuration

## 📊 Demo Categories

### Physics Validation
- **BallFreeFall**: Basic gravity and physics validation
- **BasicGroundBall**: Ground collision testing
- **BasicDemo**: General physics engine validation

### Collision Systems
- **BallCollision2**: Advanced two-ball collision physics
- **BallCollisionDemo**: Multi-object collision performance

### Rendering & Graphics
- **TerrainDemo**: Advanced terrain generation and environmental rendering

## 🔧 Technical Details

### Architecture
- Each demo extends the `BaseScene` class
- Demos link against the shared `RealityCore` library
- Clean separation between engine and demo code
- Cross-platform compatibility (Windows, macOS, Linux)

### Build System
- Demos are built as separate executables
- Output organized in `build/bin/` directory
- Automatic library discovery via RPATH
- Parallel compilation support

### Performance
- Shared engine library reduces executable size
- Mesh caching for optimal rendering performance
- Object pooling for memory efficiency
- Real-time FPS monitoring available

## 📚 Related Documentation

- **[Main README](../README.md)**: Project overview and setup
- **[Build System Plan](../docs/project-plans/BUILD_SYSTEM_REFACTOR_PLAN.md)**: Build architecture details
- **[Physics Reference](../docs/reference/BulletPhysicsProperties.md)**: Physics configuration guide
- **[Camera Reference](../docs/reference/CameraPositions.md)**: Camera setup and positioning

---

**Last Updated**: October 2025  
**Total Demos**: 6  
**Engine**: RealityCore Physics Engine  
**Build System**: CMake + Organized Output Structure
