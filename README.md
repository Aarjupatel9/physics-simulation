# RealityCore Physics Engine (3D)

## Overview
A comprehensive 3D physics engine written in C++ using OpenGL, GLFW, GLAD, and GLM. Features a modular architecture with the engine compiled as a shared library and multiple demo scenes as separate executables. Includes advanced physics simulation, collision detection, and performance monitoring.

## Architecture
- **Shared Library Engine**: Core physics engine compiled as `libRealityCore.dylib` (macOS) / `RealityCore.dll` (Windows)
- **Modular Demos**: Each demo scene is a separate executable that links to the engine library
- **Clean Separation**: Engine source code in `engine/`, demo scenes in `demos/`
- **Organized Build Output**: Executables in `build/bin/`, libraries in `build/lib/`, dependencies in `build/deps/`
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
./build/bin/<DemoName>

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
build\bin\<DemoName>.exe

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
./build/bin/<DemoName>

   ```

## Demo Scenes

The project includes multiple interactive demo scenes showcasing different aspects of the physics engine. Each demo is a separate executable that uses the shared engine library.

**📋 For complete demo descriptions, controls, and usage instructions, see: [`demos/README.md`](./demos/README.md)**

### Quick Demo Overview
- **Physics Validation**: Basic physics, gravity, and ground collision testing
- **Collision Systems**: Single-ball, two-ball, and multi-ball collision demonstrations  
- **Rendering & Graphics**: Advanced terrain generation and environmental effects
- **Performance Testing**: Multi-object scenarios for performance analysis

### Build Output Structure
The build system organizes outputs into logical directories:
```
build/
├── bin/          # All executable demos
├── lib/          # Engine library and dependencies  
├── deps/         # External dependencies (Bullet, GLFW, etc.)
└── intermediate/ # Build artifacts and object files
```

### Running Demos
```bash
# Build all demos
./build.sh

# Run any demo from organized bin/ directory
./build/bin/<DemoName>

# Example:
./build/bin/BallCollision2
```

**📋 See [`demos/README.md`](./demos/README.md) for complete demo list and descriptions.**

## Documentation

Comprehensive documentation is available across multiple locations:

### 🎮 [Demo Scenes](./demos/README.md)
- **Complete demo descriptions** and usage instructions
- **Universal controls** and demo-specific features
- **Scene creation guide** and development workflow

### 📋 [Project Plans](./docs/project-plans/)
- **Implementation roadmaps** and progress tracking
- **Bullet Physics integration** status and next steps
- **Active task list** and completed milestones

### 📚 [Reference Materials](./docs/reference/)
- **Physics properties** and configuration guides
- **Common scenarios** and troubleshooting
- **Ball movement patterns** and collision analysis
- **Camera positions** for optimal viewing

### 📖 [User Guides](./docs/guides/)
- **How to use** reference materials effectively
- **Best practices** for scene development

**Quick Start**: See [`docs/README.md`](./docs/README.md) for complete navigation guide.

## Development

### Creating New Demo Scenes
The project includes a scene generator script for creating new demos. See [`demos/README.md`](./demos/README.md) for complete instructions on creating and managing demo scenes.
### Contributing
- **Engine Code**: Located in `engine/src/` - core physics and rendering systems
- **Demo Scenes**: Located in `demos/` - individual demo implementations  
- **Documentation**: Located in `docs/` - project plans, references, and guides

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