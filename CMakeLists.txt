# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(PhysicsEngine2D)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find all source files in the "src" directory
file(GLOB SOURCES "src/*.cpp")

# Add an executable target
add_executable(PhysicsEngine ${SOURCES})

# Add the "include" directory to the include path
# THIS IS THE CRUCIAL LINE THAT FIXES THE ERROR.
# It tells the compiler to look for headers in the "include" folder.
target_include_directories(PhysicsEngine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

