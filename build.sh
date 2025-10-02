#!/bin/bash

# Cross-Platform RealityCore Physics Engine Build Script
# Compatible with macOS, Linux, and Windows (via Git Bash/MSYS2)

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Detect operating system
detect_os() {
    if [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="linux"
    elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
        OS="windows"
    else
        OS="unknown"
    fi
    print_status "Detected OS: $OS"
}

# Check prerequisites
check_prerequisites() {
    print_status "Checking prerequisites..."
    
    local missing_tools=()
    
    # Check CMake
    if ! command -v cmake &> /dev/null; then
        missing_tools+=("cmake")
    else
        CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
        print_status "CMake version: $CMAKE_VERSION"
        
        # Check if version is >= 3.10
        if ! cmake --version | grep -q "version 3\.[1-9][0-9]\|version [4-9]\."; then
            print_warning "CMake version $CMAKE_VERSION detected. Version 3.10+ recommended."
        fi
    fi
    
    # Check Git
    if ! command -v git &> /dev/null; then
        missing_tools+=("git")
    else
        print_status "Git found: $(git --version)"
    fi
    
    # Check C++ compiler
    if [[ "$OS" == "macos" ]]; then
        if ! command -v clang++ &> /dev/null; then
            missing_tools+=("Xcode Command Line Tools")
        else
            print_status "Clang++ found: $(clang++ --version | head -n1)"
        fi
    elif [[ "$OS" == "linux" ]]; then
        if ! command -v g++ &> /dev/null; then
            missing_tools+=("g++")
        else
            print_status "G++ found: $(g++ --version | head -n1)"
        fi
    elif [[ "$OS" == "windows" ]]; then
        # Check for MSVC or MinGW
        if ! command -v g++ &> /dev/null && ! command -v cl &> /dev/null; then
            missing_tools+=("C++ compiler (MSVC or MinGW)")
        else
            if command -v g++ &> /dev/null; then
                print_status "MinGW G++ found: $(g++ --version | head -n1)"
            elif command -v cl &> /dev/null; then
                print_status "MSVC found: $(cl 2>&1 | head -n1)"
            fi
        fi
    fi
    
    # Report missing tools
    if [ ${#missing_tools[@]} -ne 0 ]; then
        print_error "Missing required tools:"
        for tool in "${missing_tools[@]}"; do
            echo "  - $tool"
        done
        echo ""
        print_installation_instructions
        exit 1
    fi
    
    print_success "All prerequisites found!"
}

# Print installation instructions based on OS
print_installation_instructions() {
    print_status "Installation instructions:"
    echo ""
    
    if [[ "$OS" == "macos" ]]; then
        echo "macOS:"
        echo "  1. Install Xcode Command Line Tools:"
        echo "     xcode-select --install"
        echo ""
        echo "  2. Install CMake (if not installed):"
        echo "     brew install cmake"
        echo "     # Or download from: https://cmake.org/download/"
        echo ""
        echo "  3. Install Git (if not installed):"
        echo "     brew install git"
        echo "     # Or download from: https://git-scm.com/download/mac"
        
    elif [[ "$OS" == "linux" ]]; then
        echo "Linux (Ubuntu/Debian):"
        echo "  sudo apt update && sudo apt install build-essential cmake git"
        echo ""
        echo "Linux (Fedora/RHEL):"
        echo "  sudo dnf install gcc-c++ cmake git"
        echo ""
        echo "Linux (Arch):"
        echo "  sudo pacman -S base-devel cmake git"
        
    elif [[ "$OS" == "windows" ]]; then
        echo "Windows:"
        echo "  Option 1 - Visual Studio (Recommended):"
        echo "    1. Install Visual Studio 2022 with C++ development tools"
        echo "    2. Install CMake: https://cmake.org/download/"
        echo "    3. Install Git: https://git-scm.com/download/win"
        echo ""
        echo "  Option 2 - MSYS2/MinGW:"
        echo "    1. Install MSYS2: https://www.msys2.org/"
        echo "    2. Install toolchain: pacman -S mingw-w64-x86_64-toolchain"
        echo "    3. Install CMake: pacman -S mingw-w64-x86_64-cmake"
        echo "    4. Install Git: pacman -S git"
    fi
}

# Build the project
build_project() {
    print_status "Building Physics Simulation (Engine + Demos)..."
    
    # Clean and create build directory
    if [ -d "build" ]; then
        print_status "Cleaning build directory..."
        rm -rf build/*
    else
        print_status "Creating build directory..."
        mkdir -p build
    fi
    
    # Configure with CMake
    print_status "Configuring with CMake..."
    cd build
    
    # Set CMake generator based on OS
    if [[ "$OS" == "windows" ]]; then
        # Try MSVC first, fallback to MinGW
        if command -v cl &> /dev/null; then
            cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_POLICY_VERSION_MINIMUM=3.5
        else
            cmake .. -G "MinGW Makefiles" -DCMAKE_POLICY_VERSION_MINIMUM=3.5
        fi
    else
        cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5
    fi
    
    # Build the project
    print_status "Building..."
    if [[ "$OS" == "windows" ]] && command -v cl &> /dev/null; then
        # Use MSBuild for Visual Studio
        cmake --build . --config Release -j
    else
        # Use make for Unix-like systems
        make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    fi
    
    cd ..
}

# Find executable and provide run instructions
find_executable() {
    print_status "Locating executables..."
    
    local demos_found=()
    
    # Look for demo executables in organized bin/ directory
    if [[ "$OS" == "windows" ]]; then
        # Windows executables with .exe extension
        for demo in build/bin/*.exe; do
            if [ -f "$demo" ]; then
                demos_found+=("$demo")
            fi
        done
    else
        # Unix executables (macOS/Linux)
        for demo in build/bin/*; do
            if [ -f "$demo" ] && [ -x "$demo" ]; then
                demos_found+=("$demo")
            fi
        done
    fi
    
    if [ ${#demos_found[@]} -gt 0 ]; then
        print_success "Build complete! Found ${#demos_found[@]} demo(s):"
        echo ""
        print_status "Available demos:"
        for demo in "${demos_found[@]}"; do
            demo_name=$(basename "$demo" .exe)  # Remove .exe extension if present
            echo "  $demo_name"
        done
        echo ""
        print_status "To run a demo:"
        if [[ "$OS" == "windows" ]]; then
            echo "  ${demos_found[0]}"
        else
            echo "  ./${demos_found[0]}"
        fi
        echo ""
        print_status "Demo descriptions:"
        echo "  BallCollision2 - Two-ball collision physics demonstration"
        echo "  BallCollisionDemo - Multiple balls with collision detection on a bounded plane"
        echo "  BallFreeFall - Simple ball falling under gravity"
        echo "  BasicDemo - Simple physics demo with cube and sphere"
        echo "  BasicGroundBall - Ball collision with ground plane"
        echo "  TerrainDemo - Beautiful terrain with skybox and environmental effects"
    else
        print_error "No demo executables found! Build may have failed."
        exit 1
    fi
}

# Main execution
main() {
    echo "=========================================="
    echo "Physics Simulation Cross-Platform Build Script"
    echo "=========================================="
    echo ""
    
    detect_os
    check_prerequisites
    build_project
    find_executable
    
    print_success "Build process completed successfully!"
}

# Run main function
main "$@"