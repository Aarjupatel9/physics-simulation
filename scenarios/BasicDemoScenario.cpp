#include "BasicDemoScenario.h"
#include "../src/utils/MeshGenerator.h"
#include "../src/shapes/Box.h"
#include "../src/shapes/Sphere.h"
#include "../src/rendering/FPSRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

const char* BasicDemoScenario::s_vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
void main() {
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
)";

const char* BasicDemoScenario::s_fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 uColor;
void main() {
    FragColor = vec4(uColor, 1.0);
}
)";

BasicDemoScenario::BasicDemoScenario() : m_window(nullptr) {}

BasicDemoScenario::~BasicDemoScenario() {
    cleanup();
}

bool BasicDemoScenario::initialize(GLFWwindow* window) {
    m_window = window;
    
    // Initialize physics world
    m_world = std::make_unique<World>(glm::vec3(0.0f, -9.8f, 0.0f));
    
    // Create physics bodies with shapes
    auto cubeShape = std::make_unique<Box>(1.0f, 1.0f, 1.0f); // 1m x 1m x 1m cube
    auto sphereShape = std::make_unique<Sphere>(0.5f, 32); // 0.5m radius sphere
    
    auto cubeBody = std::make_unique<RigidBody3D>(std::move(cubeShape), 10.0f);
    auto sphereBody = std::make_unique<RigidBody3D>(std::move(sphereShape), 5.0f);
    
    // Set initial positions
    cubeBody->setPosition(glm::vec3(-1.0f, 2.0f, 0.0f));
    sphereBody->setPosition(glm::vec3(1.0f, 3.0f, 0.0f));
    
    m_world->AddBody(cubeBody.get());
    m_world->AddBody(sphereBody.get());
    
    m_bodies.push_back(std::move(cubeBody));
    m_bodies.push_back(std::move(sphereBody));
    
    // Initialize shader
    m_shader = std::make_unique<Shader>();
    if (!m_shader->loadFromSource(s_vertexShaderSource, s_fragmentShaderSource)) {
        std::cerr << "Failed to load shader" << std::endl;
        return false;
    }
    
    // Initialize camera
    m_camera = std::make_unique<Camera>();
    
    // Generate meshes
    m_cubeMesh = std::make_unique<Mesh>();
    m_cubeMesh->loadVertices(MeshGenerator::generateCube());
    
    m_sphereMesh = std::make_unique<Mesh>();
    m_sphereMesh->loadVertices(MeshGenerator::generateSphere(32, 16, 0.5f));
    
    m_groundMesh = std::make_unique<Mesh>();
    m_groundMesh->loadVertices(MeshGenerator::generateGroundPlane());
    
    // Setup GLFW callbacks
    glfwSetCursorPosCallback(window, Camera::mouseCallback);
    glfwSetScrollCallback(window, Camera::scrollCallback);
    glfwSetKeyCallback(window, Camera::keyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Initialize FPS renderer
    m_fpsRenderer = std::make_unique<FPSRenderer>();
    if (!m_fpsRenderer->initialize()) {
        std::cerr << "Failed to initialize FPS renderer" << std::endl;
        return false;
    }
    
    // Set initial object count
    m_objectCount = static_cast<int>(m_bodies.size());
    
    return true;
}

void BasicDemoScenario::update(float deltaTime) {
    // Update camera
    m_camera->update(m_window, deltaTime);
    
    // Update physics
    m_world->Update(deltaTime);
    
           // Update FPS renderer with current metrics
           if (m_fpsRenderer) {
               // Estimate draw calls and triangles (3 objects: cube, sphere, ground)
               int drawCalls = 3; // One draw call per object
               int trianglesRendered = 12 + 32 + 2; // Cube: 12, Sphere: 32, Ground: 2
               m_fpsRenderer->update(deltaTime, m_objectCount, m_collisionChecks, drawCalls, trianglesRendered);
           }
}

void BasicDemoScenario::render() {
    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use shader
    m_shader->use();
    
    // Set view and projection matrices
    glm::mat4 view = m_camera->getViewMatrix();
    glm::mat4 proj = m_camera->getProjectionMatrix(800.0f / 600.0f);
    
    m_shader->setUniform("view", view);
    m_shader->setUniform("proj", proj);
    
    // Render cube
    glm::mat4 cubeModel = glm::mat4(1.0f);
    cubeModel = glm::translate(cubeModel, m_bodies[0]->getPosition());
    cubeModel = cubeModel * glm::mat4_cast(m_bodies[0]->getRotation());
    
    m_shader->setUniform("model", cubeModel);
    m_shader->setUniform("uColor", glm::vec3(0.2f, 0.8f, 1.0f)); // Cyan
    m_cubeMesh->draw();
    
    // Render sphere
    glm::mat4 sphereModel = glm::mat4(1.0f);
    sphereModel = glm::translate(sphereModel, m_bodies[1]->getPosition());
    sphereModel = sphereModel * glm::mat4_cast(m_bodies[1]->getRotation());
    
    m_shader->setUniform("model", sphereModel);
    m_shader->setUniform("uColor", glm::vec3(1.0f, 0.4f, 0.3f)); // Orange-red
    m_sphereMesh->draw();
    
    // Render ground
    glm::mat4 groundModel = glm::mat4(1.0f);
    m_shader->setUniform("model", groundModel);
    m_shader->setUniform("uColor", glm::vec3(0.5f, 0.5f, 0.5f)); // Grey
    m_groundMesh->draw();
}

void BasicDemoScenario::renderFPS() {
    if (m_fpsRenderer && m_fpsRenderer->isDisplayEnabled()) {
        glm::mat4 view = m_camera->getViewMatrix();
        
        // Get window dimensions for aspect ratio
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        glm::mat4 projection = m_camera->getProjectionMatrix(aspectRatio);
        
        m_fpsRenderer->render(view, projection);
    }
}

void BasicDemoScenario::toggleFPSDisplay() {
    if (m_fpsRenderer) {
        m_fpsRenderer->toggleDisplay();
    }
}

bool BasicDemoScenario::isFPSDisplayEnabled() const {
    return m_fpsRenderer ? m_fpsRenderer->isDisplayEnabled() : false;
}

void BasicDemoScenario::cleanup() {
    // Cleanup is handled by unique_ptr destructors
    m_world.reset();
    m_bodies.clear();
    m_shader.reset();
    m_camera.reset();
    m_cubeMesh.reset();
    m_sphereMesh.reset();
    m_groundMesh.reset();
    m_fpsRenderer.reset();
}
