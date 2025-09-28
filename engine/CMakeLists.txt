# Physics Engine Shared Library CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10...3.27)

# Set the project name
project(PhysicsEngine VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Cross-platform compiler settings
if(MSVC)
    # Visual Studio specific settings
    add_compile_options(/W4)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    # Suppress OpenGL deprecation warnings on Windows
    add_compile_definitions(GL_SILENCE_DEPRECATION)
    # Define M_PI for MinGW compatibility
    add_compile_definitions(_USE_MATH_DEFINES)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # GCC/Clang specific settings
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(APPLE)
        # Suppress OpenGL deprecation warnings on macOS
        add_compile_definitions(GL_SILENCE_DEPRECATION)
    endif()
endif()

# --- Library Integration using FetchContent ---
include(FetchContent)

# GLFW (Windowing and Input)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLAD (OpenGL Loader)
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# GLM (Mathematics Library)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)

# Find OpenGL
find_package(OpenGL REQUIRED)

# --- Engine Source Files ---
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")

# Create shared library
add_library(PhysicsEngine SHARED ${ENGINE_SOURCES})

# Set library properties
set_target_properties(PhysicsEngine PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "include/RigidBody3D.h;include/Vec2.h;include/World.h"
)

# Include directories
target_include_directories(PhysicsEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${glm_SOURCE_DIR}>
    $<BUILD_INTERFACE:${glad_SOURCE_DIR}/include>
)

# Link libraries
target_link_libraries(PhysicsEngine PUBLIC
    glad
    ${OPENGL_LIBRARIES}
    glfw
)

# Export targets for use by demos
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
