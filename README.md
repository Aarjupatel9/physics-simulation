Physics Simulation (3D)

Overview
This is a minimal 3D physics sandbox written in C++ using OpenGL, GLFW, GLAD, and GLM. It renders a cube and a sphere, simulates gravity, resolves ground-plane collisions, and includes a free-fly camera with modern controls.

Features
- 3D rendering via OpenGL Core profile
- Physics integration (gravity) and simple ground collision with restitution
- Free-fly camera: WASD + I/K, mouse look, zoom, sprint
- Toggle controls on/off with B (releases/captures mouse)

Controls
- Move: W/A/S/D
- Up/Down: I / K
- Look: Mouse
- Sprint: Left Shift
- Zoom: Mouse scroll, or +/- keys
- Toggle controls capture: B
- Exit: Esc

Prerequisites
These are fetched automatically at configure time; no manual library installs needed.
- CMake 3.10+ (3.27 recommended)
- Git (to fetch dependencies)
- A C/C++ toolchain:
  - macOS: Xcode Command Line Tools (clang)
  - Windows: Visual Studio 2019/2022 (MSVC) or MSYS2/MinGW

Note on CMake policies
We pass CMAKE_POLICY_VERSION_MINIMUM=3.5 to allow older third-party subprojects (e.g., glad) to configure cleanly.

macOS: Build & Run
1) Install Xcode CLT (if not already):
   xcode-select --install

2) Build using the provided script (recommended):
   ./build.sh

   Or manually:
   cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5
   cmake --build build -j

3) Run:
   ./build/PhysicsEngine

Windows: Build & Run (Visual Studio/MSVC)
1) Install:
   - Visual Studio 2022 (Desktop development with C++) or Build Tools
   - CMake (add to PATH)
   - Git (add to PATH)

2) Configure (x64 generator shown):
   cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_POLICY_VERSION_MINIMUM=3.5

3) Build (Release suggested):
   cmake --build build --config Release -j

4) Run:
   build/Release/PhysicsEngine.exe

Windows: Build & Run (MSYS2/MinGW, optional)
1) Install MSYS2 and base-devel toolchain, ensure cmake and git are installed.
2) From MinGW shell:
   cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
   cmake --build build -j
3) Run:
   build/PhysicsEngine.exe

Linux: Build & Run
1) Install dependencies:
   # Ubuntu/Debian
   sudo apt update && sudo apt install build-essential cmake git
   
   # Fedora/RHEL
   sudo dnf install gcc-c++ cmake git

2) Build using the provided script:
   ./build.sh

3) Run:
   ./build/PhysicsEngine

Clean Build
- Remove build outputs:
  rm -rf build
  # Windows
  rmdir /S /Q build

Build Script
The project includes a convenient build script `build.sh` that:
- Automatically cleans the build directory
- Configures CMake with the correct policy settings
- Builds the project with optimal parallel compilation
- Provides clear feedback during the build process

Usage: `./build.sh`

Troubleshooting
- Policy error at configure (glad/glm): Ensure you pass -DCMAKE_POLICY_VERSION_MINIMUM=3.5 when running CMake configure.
- Linker errors about OpenGL on macOS: Command Line Tools are required; re-run xcode-select --install.
- Black screen: Ensure your GPU supports OpenGL 3.3 Core Profile.

Project Structure
- src/main.cpp: app entry, rendering, camera, and buffer setup
- src/World.cpp, include/World.h: world update, gravity, collisions
- src/RigidBody3D.cpp, include/RigidBody3D.h: body integration and collision helpers
- CMakeLists.txt: dependencies (GLFW, GLAD, GLM) via FetchContent

# physics-simulation