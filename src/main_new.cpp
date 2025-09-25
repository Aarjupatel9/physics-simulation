#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "scenarios/BaseScenario.h"
#include "scenarios/BasicDemoScenario.h"

// Initialize GLFW and OpenGL context
GLFWwindow* initializeWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Physics Engine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    
    glEnable(GL_DEPTH_TEST);
    return window;
}

// Display scenario selection menu
int selectScenario() {
    std::cout << "\n=== 3D Physics Engine ===" << std::endl;
    std::cout << "Select a scenario to run:" << std::endl;
    std::cout << "1. Basic Demo (Cube + Sphere with gravity)" << std::endl;
    std::cout << "2. Advanced Demo (Coming soon)" << std::endl;
    std::cout << "3. Particle System (Coming soon)" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice (0-3): ";
    
    int choice;
    std::cin >> choice;
    return choice;
}

// Create scenario based on user choice
std::unique_ptr<BaseScenario> createScenario(int choice) {
    switch (choice) {
        case 1:
            return std::make_unique<BasicDemoScenario>();
        case 2:
        case 3:
            std::cout << "This scenario is not implemented yet!" << std::endl;
            return nullptr;
        default:
            return nullptr;
    }
}

int main() {
    // Initialize window and OpenGL
    GLFWwindow* window = initializeWindow();
    if (!window) {
        return -1;
    }
    
    // Select scenario
    int choice = selectScenario();
    if (choice == 0) {
        glfwTerminate();
        return 0;
    }
    
    // Create and initialize scenario
    auto scenario = createScenario(choice);
    if (!scenario) {
        glfwTerminate();
        return -1;
    }
    
    if (!scenario->initialize(window)) {
        std::cerr << "Failed to initialize scenario: " << scenario->getName() << std::endl;
        glfwTerminate();
        return -1;
    }
    
    std::cout << "\nRunning scenario: " << scenario->getName() << std::endl;
    std::cout << "Description: " << scenario->getDescription() << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "WASD - Move, I/K - Up/Down, Mouse - Look" << std::endl;
    std::cout << "Shift - Sprint, Scroll/+/- - Zoom" << std::endl;
    std::cout << "B - Toggle controls, ESC - Exit" << std::endl;
    
    // Main loop
    float lastFrameTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrameTime = glfwGetTime();
        float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;
        
        // Handle input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
        
        // Update scenario
        scenario->update(deltaTime);
        
        // Render scenario
        scenario->render();
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    scenario->cleanup();
    glfwTerminate();
    
    std::cout << "Physics engine shutdown complete." << std::endl;
    return 0;
}
