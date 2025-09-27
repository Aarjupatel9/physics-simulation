# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10...3.27)

# Set the project name
project(PhysicsEngine3D VERSION 1.0.0 LANGUAGES CXX)

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
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # GCC/Clang specific settings
    add_compile_options(-Wall -Wextra -Wpedantic)
    if(APPLE)
        # Suppress OpenGL deprecation warnings on macOS
        add_compile_definitions(GL_SILENCE_DEPRECATION)
    endif()
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

# --- Library Integration using FetchContent ---
# This automatically downloads and configures the libraries we need.
include(FetchContent)

# GLFW (Windowing and Input)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.8
)
# Reduce GLFW build footprint
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
# Prefer the target provided by the project
FetchContent_MakeAvailable(glad)


# GLM (Mathematics Library) - header-only; fetch headers only (no configure/build)
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.8
)
FetchContent_MakeAvailable(glm)


# Find OpenGL
find_package(OpenGL REQUIRED)

# --- Project Configuration ---

# Find all source files in the "src" directory and subdirectories
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE SCENARIO_SOURCES "scenarios/*.cpp")

# Combine all sources
set(ALL_SOURCES ${SOURCES} ${SCENARIO_SOURCES})

# GLAD provides target `glad`

# Add an executable target
add_executable(PhysicsEngine ${ALL_SOURCES})

# Add include directories
target_include_directories(PhysicsEngine PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/scenarios"
    ${glm_SOURCE_DIR} # Add GLM's headers
    ${glad_SOURCE_DIR}/include # Add GLAD's headers
)

# Link the required libraries to our executable
target_link_libraries(PhysicsEngine PRIVATE 
    glad
    ${OPENGL_LIBRARIES} 
    glfw
)

