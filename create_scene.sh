#!/bin/bash

# Scene Generator Script
# Usage: ./create_scene.sh <SceneName>
# Example: ./create_scene.sh BallCollision2

if [ $# -eq 0 ]; then
    echo "Usage: $0 <SceneName>"
    echo "Example: $0 BallCollision2"
    exit 1
fi

SCENE_NAME=$1
SCENE_DIR="demos/${SCENE_NAME}"

echo "Creating scene: $SCENE_NAME"
echo "Directory: $SCENE_DIR"

# Create directory
mkdir -p "$SCENE_DIR"

# Create header file
cat > "$SCENE_DIR/${SCENE_NAME}Scene.h" << EOF
#pragma once

#include "BaseScene.h"

class ${SCENE_NAME}Scene : public BaseScene {
public:
    ${SCENE_NAME}Scene();
    virtual ~${SCENE_NAME}Scene() = default;

    // BaseScene overrides
    bool initialize(GLFWwindow* window) override;
    void update(float deltaTime) override;
    void render() override;
    void cleanup() override;
    void initializeObjects() override;
    
    const char* getName() const override { return "${SCENE_NAME} Scene"; }
    const char* getDescription() const override { return "Generated ${SCENE_NAME} scene"; }
};
EOF

# Create implementation file
cat > "$SCENE_DIR/${SCENE_NAME}Scene.cpp" << EOF
#include "${SCENE_NAME}Scene.h"
#include <iostream>

${SCENE_NAME}Scene::${SCENE_NAME}Scene() {
    std::cout << "=== ${SCENE_NAME} Scene ===" << std::endl;
    std::cout << "Starting ${SCENE_NAME} Scene..." << std::endl;
}

bool ${SCENE_NAME}Scene::initialize(GLFWwindow* window) {
    std::cout << "Initializing ${SCENE_NAME} Scene..." << std::endl;
    
    // Call base class initialization (sets up camera, shaders, etc.)
    if (!BaseScene::initialize(window)) {
        return false;
    }
    
    // Initialize our objects
    initializeObjects();
    
    std::cout << "${SCENE_NAME} Scene initialized successfully!" << std::endl;
    return true;
}

void ${SCENE_NAME}Scene::initializeObjects() {
    std::cout << "Creating ${SCENE_NAME} Scene objects..." << std::endl;
    
    // Add your objects here
    std::cout << "${SCENE_NAME} Scene: Ready for objects" << std::endl;
}

void ${SCENE_NAME}Scene::update(float deltaTime) {
    // Call base class update (handles physics, camera, etc.)
    BaseScene::update(deltaTime);
    
    // Add your custom update logic here
}

void ${SCENE_NAME}Scene::render() {
    // Call base class render (handles all rendering)
    BaseScene::render();
}

void ${SCENE_NAME}Scene::cleanup() {
    std::cout << "Cleaning up ${SCENE_NAME} Scene..." << std::endl;
    BaseScene::cleanup();
    std::cout << "${SCENE_NAME} Scene cleanup complete" << std::endl;
}
EOF

# Create main.cpp
cat > "$SCENE_DIR/main.cpp" << EOF
#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "${SCENE_NAME}Scene.h"

// Window dimensions
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// GLFW error callback
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// Window resize callback
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    std::cout << "=== ${SCENE_NAME} Demo ===" << std::endl;
    std::cout << "Starting ${SCENE_NAME} Scene..." << std::endl;
    
    // Initialize GLFW
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    
    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "${SCENE_NAME} Scene", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    try {
        ${SCENE_NAME}Scene scene;
        
        if (!scene.initialize(window)) {
            std::cerr << "Failed to initialize ${SCENE_NAME} Scene" << std::endl;
            glfwTerminate();
            return -1;
        }
        
        std::cout << "${SCENE_NAME} Demo running successfully!" << std::endl;
        std::cout << "Controls:" << std::endl;
        std::cout << "  WASD - Move, I/K - Up/Down, Mouse - Look" << std::endl;
        std::cout << "  Shift - Sprint, Scroll/+/- - Zoom" << std::endl;
        std::cout << "  B - Toggle controls, F - Toggle FPS display, ESC - Exit" << std::endl;
        
        // Main loop
        while (!glfwWindowShouldClose(window)) {
            float currentTime = glfwGetTime();
            static float lastTime = currentTime;
            float deltaTime = currentTime - lastTime;
            lastTime = currentTime;
            
            // Update scene
            scene.update(deltaTime);
            
            // Render scene
            scene.render();
            
            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        
        scene.cleanup();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwTerminate();
    std::cout << "${SCENE_NAME} Demo closed successfully!" << std::endl;
    return 0;
}
EOF

# Create CMakeLists.txt
cat > "$SCENE_DIR/CMakeLists.txt" << EOF
# ${SCENE_NAME} CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Create the ${SCENE_NAME} executable
add_executable(${SCENE_NAME}
    main.cpp
    ${SCENE_NAME}Scene.cpp
)

# Link against the PhysicsEngine library
target_link_libraries(${SCENE_NAME} PhysicsEngine)

# Set include directories
target_include_directories(${SCENE_NAME} PRIVATE
    \${CMAKE_SOURCE_DIR}/engine/include
    \${CMAKE_SOURCE_DIR}/engine/src
)

# Set C++ standard
set_target_properties(${SCENE_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
EOF

echo "Scene '$SCENE_NAME' created successfully!"
echo "Files created:"
echo "  - $SCENE_DIR/${SCENE_NAME}Scene.h"
echo "  - $SCENE_DIR/${SCENE_NAME}Scene.cpp"
echo "  - $SCENE_DIR/main.cpp"
echo "  - $SCENE_DIR/CMakeLists.txt"
echo ""
echo "Next steps:"
echo "  1. Add 'add_subdirectory(${SCENE_NAME})' to demos/CMakeLists.txt"
echo "  2. Run: cmake -B build"
echo "  3. Run: cmake --build build --target ${SCENE_NAME}"
echo "  4. Run: ./build/${SCENE_NAME}"
