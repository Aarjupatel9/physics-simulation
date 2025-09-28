# RealityCore Physics Engine (3D)

## Overview
A comprehensive 3D physics engine written in C++ using OpenGL, GLFW, GLAD, and GLM. Features a modular architecture with the engine compiled as a shared library and multiple demo scenes as separate executables. Includes advanced physics simulation, collision detection, and performance monitoring.

## Architecture
- **Shared Library Engine**: Core physics engine compiled as `libRealityCore.dylib` (macOS) / `RealityCore.dll` (Windows)
- **Modular Demos**: Each demo scene is a separate executable that links to the engine library
- **Clean Separation**: Engine source code in `engine/`, demo scenes in `demos/`
- **Cross-platform Build**: Automated build scripts for macOS, Windows, and Linux

## Features
- **3D Rendering**: OpenGL Core profile with modern shader-based rendering
- **Physics Engine**: Gravity simulation, collision detection, and rigid body dynamics
- **Multiple Demo Scenes**: Basic demo, terrain, performance testing, and ball collision scenarios
- **Free-fly Camera**: WASD movement, mouse look, zoom, and sprint controls
- **Performance Monitoring**: Real-time FPS display and performance metrics
- **Object Management**: Mesh caching, object pooling, and static object optimization
- **Conservative Collisions**: Perfectly elastic collisions with no energy loss

## Controls
- **Movement**: W/A/S/D (forward/left/backward/right)
- **Up/Down**: I/K keys
- **Look Around**: Mouse movement
- **Sprint**: Left Shift (hold)
- **Zoom**: Mouse scroll wheel or +/- keys
- **Toggle Controls**: B key (releases/captures mouse cursor)
- **Toggle FPS Display**: F key
- **Exit**: Esc key

## Prerequisites
The build system automatically fetches all required libraries. You only need:

### Required Tools
- **CMake 3.10+** (3.27+ recommended for best experience)
- **Git** (for dependency fetching)
- **C++ Compiler** with C++17 support:
  - **macOS**: Xcode Command Line Tools (clang)
  - **Windows**: LLVM/Clang 19.0.0+ (Recommended) or Visual Studio 2022 (MSVC) or MSYS2/MinGW
  - **Linux**: GCC 7+ or Clang 5+

### Windows-Specific Requirements
For Windows, we recommend **LLVM/Clang 19.0.0+** for the best C++17 support:

```cmd
# Install LLVM/Clang via winget (Recommended)
winget install LLVM.LLVM

# Or install Visual Studio 2022 with C++ development tools
# Or install MSYS2 with MinGW toolchain
```

## Quick Start

### Option 1: Automated Build (Recommended)
The project includes intelligent build scripts that automatically detect your platform and check prerequisites:

**macOS/Linux:**
```bash
./build.sh
```

**Windows:**
```cmd
build.bat
```
```powershell
.\build.ps1
```

The build scripts will:
- ✅ Check all prerequisites automatically
- ✅ Provide installation instructions if anything is missing
- ✅ Configure CMake with optimal settings for your platform
- ✅ Build the project with parallel compilation
- ✅ Locate the executable and provide run instructions

### Option 2: Manual Build

#### macOS
1. **Install Xcode Command Line Tools:**
   ```bash
   xcode-select --install
   ```

2. **Install CMake (if not installed):**
   ```bash
   brew install cmake
   # Or download from: https://cmake.org/download/
   ```

3. **Build:**
   ```bash
   cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5
   cmake --build build -j
   ```

4. **Run Demos:**
   ```bash
   # Run specific demo
   ./build/demos/BallCollisionDemo
   ./build/demos/BasicDemo
   ./build/demos/TerrainDemo
   ```

#### Windows (Visual Studio)
1. **Install Visual Studio 2022** with "Desktop development with C++" workload
2. **Install CMake** and add to PATH: https://cmake.org/download/
3. **Install Git** and add to PATH: https://git-scm.com/download/win

4. **Build:**
   ```cmd
   cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_POLICY_VERSION_MINIMUM=3.5
   cmake --build build --config Release -j
   ```

5. **Run Demos:**
   ```cmd
   # Run specific demo
   build\Release\demos\BallCollisionDemo.exe
   build\Release\demos\BasicDemo.exe
   build\Release\demos\TerrainDemo.exe
   ```

#### Windows (MSYS2/MinGW)
1. **Install MSYS2:** https://www.msys2.org/
2. **Install toolchain:**
   ```bash
   pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake git
   ```

3. **Build:**
   ```bash
   cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
   cmake --build build -j
   ```

4. **Run Demos:**
   ```bash
   # Run specific demo
   build/demos/BallCollisionDemo.exe
   build/demos/BasicDemo.exe
   build/demos/TerrainDemo.exe
   ```

#### Linux
1. **Install dependencies:**
   ```bash
   # Ubuntu/Debian
   sudo apt update && sudo apt install build-essential cmake git
   
   # Fedora/RHEL
   sudo dnf install gcc-c++ cmake git
   
   # Arch Linux
   sudo pacman -S base-devel cmake git
   ```

2. **Build:**
   ```bash
   cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5
   cmake --build build -j
   ```

3. **Run Demos:**
   ```bash
   # Run specific demo
   ./build/demos/BallCollisionDemo
   ./build/demos/BasicDemo
   ./build/demos/TerrainDemo
   ```

## Available Demo Scenes
Each demo scene is a separate executable that uses the shared physics engine library:

### 1. Ball Collision Demo (`./build/demos/BallCollisionDemo`)
- **Features**: 15 balls with collision detection on a 5x5 meter bounded plane
- **Physics**: Conservative collisions (no energy loss, no friction)
- **Controls**: Press 'P' to start physics simulation
- **Performance**: Optimized for collision detection and physics integration

### 2. Basic Demo (`./build/demos/BasicDemo`)
- **Features**: Simple physics demo with cube and sphere
- **Physics**: Gravity simulation with ground collision
- **Use Case**: Basic physics engine testing and validation

### 3. Terrain Demo (`./build/demos/TerrainDemo`)
- **Features**: Beautiful procedural terrain with skybox
- **Rendering**: Advanced terrain generation and environmental effects
- **Use Case**: Graphics and rendering performance testing

### Running Individual Demos
```bash
# Run specific demo
./build/demos/BallCollisionDemo
./build/demos/BasicDemo
./build/demos/TerrainDemo
```

## Creating New Demo Scenes

The project includes a powerful scene generator script that creates new demo scenes instantly:

### Using the Scene Generator
```bash
# Create a new scene
./create_scene.sh <SceneName>

# Example: Create a new collision test scene
./create_scene.sh CollisionTest
```

### What the Script Creates
The script automatically generates:
- **Scene Directory**: `demos/<SceneName>/`
- **Header File**: `<SceneName>Scene.h` (with proper BaseScene inheritance)
- **Implementation**: `<SceneName>Scene.cpp` (with all required methods)
- **Main File**: `main.cpp` (with complete GLFW setup)
- **CMakeLists.txt**: Proper linking and configuration

### Manual Steps After Generation
1. **Add to CMakeLists.txt**: Add `add_subdirectory(<SceneName>)` to `demos/CMakeLists.txt`
2. **Build**: `cmake -B build && cmake --build build --target <SceneName>`
3. **Run**: `./build/<SceneName>`

### Example Workflow
```bash
# Create a new physics test scene
./create_scene.sh PhysicsTest

# Add to CMakeLists.txt manually
echo "add_subdirectory(PhysicsTest)" >> demos/CMakeLists.txt

# Build and run
cmake -B build
cmake --build build --target PhysicsTest
./build/PhysicsTest
```

### Scene Structure
Each generated scene includes:
- **Camera controls** (WASD, mouse look, zoom)
- **FPS display** (press 'F' to toggle)
- **Physics world** setup
- **Mesh caching** system
- **Clean architecture** following BaseScene pattern

## Clean Build
To completely clean the build directory:

**macOS/Linux:**
```bash
rm -rf build
```

**Windows:**
```cmd
rmdir /S /Q build
```

## Troubleshooting

### Common Issues

**CMake Policy Errors:**
- Ensure you pass `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` when configuring
- The build scripts handle this automatically

**OpenGL Issues on macOS:**
- Install Xcode Command Line Tools: `xcode-select --install`
- Ensure your GPU supports OpenGL 3.3 Core Profile

**Windows Build Issues:**
- Make sure Visual Studio 2022 is installed with C++ development tools
- Ensure CMake and Git are in your PATH
- Try running from Visual Studio Developer Command Prompt

**Linux Build Issues:**
- Install build-essential package: `sudo apt install build-essential`
- Ensure you have OpenGL development libraries installed

**Black Screen/No Rendering:**
- Check that your graphics drivers are up to date
- Verify OpenGL 3.3+ support
- Try running with different graphics settings

### Getting Help
If you encounter issues not covered here:
1. Check that all prerequisites are installed correctly
2. Try a clean build (remove build directory and rebuild)
3. Ensure you're using a supported compiler version
4. Check the console output for specific error messages

## Project Structure
```
physics-simulation/
├── engine/                    # Core physics engine (shared library)
│   ├── src/                   # Engine source code
│   │   ├── core/              # Physics engine (World, RigidBody3D, etc.)
│   │   ├── rendering/         # Graphics rendering (Camera, Shader, Mesh, etc.)
│   │   ├── shapes/            # 3D shape definitions (Box, Sphere, Plane, etc.)
│   │   └── utils/             # Utility functions
│   ├── include/               # Engine header files
│   └── CMakeLists.txt         # Engine build configuration
├── demos/                     # Demo scenes (separate executables)
│   ├── BallCollisionScene/    # Ball collision simulation
│   ├── BasicDemoScene/        # Basic physics demo
│   ├── TerrainScene/          # Terrain rendering
│   └── CMakeLists.txt         # Demos build configuration
├── launcher/                  # Future launcher application
├── build/                     # Build output directory
│   ├── engine/                # Engine shared library
│   └── demos/                 # Demo executables
├── CMakeLists.txt             # Root CMake configuration
├── build.sh                   # Cross-platform build script (macOS/Linux)
├── build.bat                  # Windows Command Prompt build script
├── build.ps1                  # Windows PowerShell build script
└── README.md                  # This file
```

## Dependencies
All dependencies are automatically fetched via CMake FetchContent:
- **GLFW 3.3.8**: Window management and input handling
- **GLAD 0.1.36**: OpenGL function loading
- **GLM 0.9.9.8**: Mathematics library for graphics

## License
This project is open source. See the LICENSE file for details.