@echo off
setlocal enabledelayedexpansion

REM Cross-Platform Physics Engine Build Script for Windows
REM Compatible with Command Prompt, PowerShell, and Git Bash

echo ==========================================
echo Physics Engine Cross-Platform Build Script
echo ==========================================
echo.

REM Detect build environment
set BUILD_ENV=unknown
where cl >nul 2>&1
if %errorlevel% == 0 (
    set BUILD_ENV=msvc
    echo [INFO] Detected MSVC environment
) else (
    where g++ >nul 2>&1
    if %errorlevel% == 0 (
        set BUILD_ENV=mingw
        echo [INFO] Detected MinGW environment
    ) else (
        echo [ERROR] No C++ compiler found!
        echo.
        echo Please install one of the following:
        echo   1. Visual Studio 2022 with C++ development tools
        echo   2. MSYS2 with MinGW toolchain
        echo.
        pause
        exit /b 1
    )
)

REM Check prerequisites
echo [INFO] Checking prerequisites...

REM Check CMake
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found!
    echo Please install CMake from: https://cmake.org/download/
    echo Make sure to add it to your PATH.
    pause
    exit /b 1
) else (
    for /f "tokens=3" %%i in ('cmake --version ^| findstr "cmake version"') do set CMAKE_VERSION=%%i
    echo [INFO] CMake version: !CMAKE_VERSION!
)

REM Check Git
where git >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Git not found!
    echo Please install Git from: https://git-scm.com/download/win
    echo Make sure to add it to your PATH.
    pause
    exit /b 1
) else (
    echo [INFO] Git found
)

echo [SUCCESS] All prerequisites found!
echo.

REM Clean and create build directory
if exist build (
    echo [INFO] Cleaning build directory...
    rmdir /s /q build
)
echo [INFO] Creating build directory...
mkdir build

REM Configure with CMake
echo [INFO] Configuring with CMake...
cd build

if "%BUILD_ENV%" == "msvc" (
    cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    if %errorlevel% neq 0 (
        echo [ERROR] CMake configuration failed!
        cd ..
        pause
        exit /b 1
    )
    
    echo [INFO] Building...
    cmake --build . --config Release -j
    if %errorlevel% neq 0 (
        echo [ERROR] Build failed!
        cd ..
        pause
        exit /b 1
    )
) else (
    cmake .. -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    if %errorlevel% neq 0 (
        echo [ERROR] CMake configuration failed!
        cd ..
        pause
        exit /b 1
    )
    
    echo [INFO] Building...
    cmake --build . -j
    if %errorlevel% neq 0 (
        echo [ERROR] Build failed!
        cd ..
        pause
        exit /b 1
    )
)

cd ..

REM Find executable
echo [INFO] Locating executable...
set EXECUTABLE=
if exist "build\Release\PhysicsEngine.exe" (
    set EXECUTABLE=build\Release\PhysicsEngine.exe
) else if exist "build\PhysicsEngine.exe" (
    set EXECUTABLE=build\PhysicsEngine.exe
)

if defined EXECUTABLE (
    echo [SUCCESS] Build complete! Executable: %EXECUTABLE%
    echo.
    echo [INFO] To run the physics engine:
    echo   %EXECUTABLE%
    echo.
    echo [INFO] Available scenes:
    echo   1. Basic Demo (Cube + Sphere with gravity)
    echo   2. Beautiful Terrain (Procedural landscape)
    echo   3. Mesh Intensive Demo (Performance stress test)
    echo   4. Ball Collision Scene (Multiple balls on bounded plane)
    echo   5. Advanced Demo (Coming soon)
    echo   6. Particle System (Coming soon)
) else (
    echo [ERROR] Executable not found! Build may have failed.
    pause
    exit /b 1
)

echo.
echo [SUCCESS] Build process completed successfully!
pause
