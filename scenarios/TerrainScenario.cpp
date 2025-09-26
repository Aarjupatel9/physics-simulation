#include "TerrainScenario.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

TerrainScenario::TerrainScenario() : m_window(nullptr) {
    // Set sun direction (afternoon sun)
    m_sunDirection = glm::normalize(glm::vec3(-0.5f, -0.8f, -0.3f));
}

TerrainScenario::~TerrainScenario() {
    cleanup();
}

bool TerrainScenario::initialize(GLFWwindow* window) {
    m_window = window;
    
    // Initialize skybox
    m_skybox = std::make_unique<Skybox>();
    if (!m_skybox->initialize()) {
        std::cerr << "Failed to initialize skybox" << std::endl;
        return false;
    }
    
    // Initialize terrain
    m_terrain = std::make_unique<Terrain>();
    if (!m_terrain->initialize(TERRAIN_SIZE, TERRAIN_SIZE, TERRAIN_SCALE, TERRAIN_HEIGHT, TERRAIN_ROUGHNESS)) {
        std::cerr << "Failed to initialize terrain" << std::endl;
        return false;
    }
    
    // Initialize grid renderer for debugging
    m_gridRenderer = std::make_unique<GridRenderer>();
    if (!m_gridRenderer->initialize(32.0f, 16)) { // 16x16 grid over 32x32 units
        std::cerr << "Failed to initialize grid renderer" << std::endl;
        return false;
    }
    m_gridRenderer->setColor(glm::vec3(0.1f, 0.1f, 0.1f)); // Very dark lines for better visibility
    
    // Initialize camera closer to see the square
    m_camera = std::make_unique<Camera>();
    m_camera->setPosition(glm::vec3(5.0f, 5.0f, 5.0f)); // Closer position
    m_camera->setYaw(-45.0f);   // Looking towards center
    m_camera->setPitch(-30.0f); // Looking down at angle
    
    // Setup GLFW callbacks
    glfwSetCursorPosCallback(window, Camera::mouseCallback);
    glfwSetScrollCallback(window, Camera::scrollCallback);
    glfwSetKeyCallback(window, Camera::keyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Enable depth testing (OpenGL calls will be available from main.cpp context)
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LEQUAL);
    
    std::cout << "Terrain scenario initialized successfully!" << std::endl;
    std::cout << "Terrain size: " << TERRAIN_SIZE << "x" << TERRAIN_SIZE << std::endl;
    std::cout << "Camera position: " << m_camera->getPosition().x << ", " << m_camera->getPosition().y << ", " << m_camera->getPosition().z << std::endl;
    std::cout << "Use WASD to move, mouse to look around" << std::endl;
    
    return true;
}

void TerrainScenario::update(float deltaTime) {
    // Update camera
    m_camera->update(m_window, deltaTime);
    
    // Debug output removed - movement confirmed working
    
    // Update sun direction (optional: make it move over time)
    // m_sunDirection = glm::normalize(glm::vec3(
    //     cos(glfwGetTime() * 0.1f) * 0.5f,
    //     -0.8f,
    //     sin(glfwGetTime() * 0.1f) * 0.3f
    // ));
}

void TerrainScenario::render() {
    // Note: OpenGL calls are handled by the main application
    // This method will be called after glClear() in main.cpp
    
    // Get camera matrices
    glm::mat4 view = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(800.0f / 600.0f);
    
    // Render skybox first
    m_skybox->render(view, projection);
    
    // Render terrain
    glm::mat4 terrainModel = glm::mat4(1.0f);
    m_terrain->render(terrainModel, view, projection, m_sunDirection);
    
    // Render grid overlay for debugging
    m_gridRenderer->render(view, projection);
    
    // TODO: Add rock scattering here
    // TODO: Add water rendering here
}

void TerrainScenario::cleanup() {
    m_skybox.reset();
    m_terrain.reset();
    m_gridRenderer.reset();
    m_camera.reset();
}
