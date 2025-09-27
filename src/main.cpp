#include <iostream>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../scenarios/BaseScene.h"
#include "../scenarios/BasicDemoScenario.h"
#include "../scenarios/TerrainScenario.h"
#include "../scenarios/BallCollisionScene/BallCollisionScene.h"

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
    std::cout << "Select a scene to run:" << std::endl;
    std::cout << "1. Basic Demo (Cube + Sphere with gravity)" << std::endl;
    std::cout << "2. Beautiful Terrain (Procedural landscape)" << std::endl;
    std::cout << "3. Mesh Intensive Demo (Performance stress test)" << std::endl;
    std::cout << "4. Ball Collision Scene (Multiple balls on bounded plane)" << std::endl;
    std::cout << "5. Advanced Demo (Coming soon)" << std::endl;
    std::cout << "6. Particle System (Coming soon)" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice (0-6): ";
    
    int choice;
    std::cin >> choice;
    return choice;
}

// Create scene based on user choice
std::unique_ptr<BaseScene> createScene(int choice) {
    switch (choice) {
        case 4:
            return std::make_unique<BallCollisionScene>();
        case 5:
        case 6:
            std::cout << "This scene is not implemented yet!" << std::endl;
            return nullptr;
        default:
            std::cout << "This scene uses the old architecture. Please select option 4 for the new Ball Collision Scene." << std::endl;
            return nullptr;
    }
}

int main() {
    // Initialize window and OpenGL
    GLFWwindow* window = initializeWindow();
    if (!window) {
        return -1;
    }
    
    // Select scene
    int choice = selectScenario();
    if (choice == 0) {
        glfwTerminate();
        return 0;
    }
    
    // Create and initialize scene
    auto scene = createScene(choice);
    if (!scene) {
        glfwTerminate();
        return -1;
    }
    
    if (!scene->initialize(window)) {
        std::cerr << "Failed to initialize scene: " << scene->getName() << std::endl;
        glfwTerminate();
        return -1;
    }
    
    std::cout << "\nRunning scene: " << scene->getName() << std::endl;
    std::cout << "Description: " << scene->getDescription() << std::endl;
    std::cout << "\nControls:" << std::endl;
    std::cout << "WASD - Move, I/K - Up/Down, Mouse - Look" << std::endl;
    std::cout << "Shift - Sprint, Scroll/+/- - Zoom" << std::endl;
    std::cout << "B - Toggle controls, F - Toggle FPS display, ESC - Exit" << std::endl;
    
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
        
        // Handle FPS display toggle
        static bool fKeyPressed = false;
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fKeyPressed) {
            scene->toggleFPSDisplay();
            fKeyPressed = true;
        } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
            fKeyPressed = false;
        }
        
        // Update scene
        scene->update(deltaTime);
        
        // Render scene
        scene->render();
        
        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    scene->cleanup();
    glfwTerminate();
    
    std::cout << "Physics engine shutdown complete." << std::endl;
    return 0;
}
