#!/bin/bash

# Physics Engine Build Script
# This script builds the physics engine in the proper build directory

set -e  # Exit on any error

echo "Building Physics Engine..."

# Clean and create build directory
if [ -d "build" ]; then
    echo "Cleaning build directory..."
    rm -rf build/*
else
    echo "Creating build directory..."
    mkdir -p build
fi

# Configure and build
echo "Configuring with CMake..."
cd build
cmake .. -DCMAKE_POLICY_VERSION_MINIMUM=3.5

echo "Building..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "Build complete! Executable: ./build/PhysicsEngine"
echo "To run: ./build/PhysicsEngine"
