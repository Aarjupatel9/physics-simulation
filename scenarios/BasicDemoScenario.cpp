#include "BasicDemoScenario.h"
#include "../src/utils/MeshGenerator.h"
#include "../src/shapes/Box.h"
#include "../src/shapes/Sphere.h"
#include "../src/rendering/FPSRenderer.h"
#include "../src/rendering/MeshCache.h"
#include "../src/core/InertiaTensorCache.h"
#include "../src/core/PhysicsObjectPool.h"
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
    
    // Adjust camera position for better viewing
    if (m_performanceTestMode) {
        m_camera->setPosition(glm::vec3(0.0f, 15.0f, 25.0f)); // Higher and further back for performance test
        m_camera->setYaw(-90.0f); // Look towards center
        m_camera->setPitch(-20.0f); // Look down slightly
    } else {
        m_camera->setPosition(glm::vec3(0.0f, 8.0f, 12.0f)); // Better distance for basic demo
        m_camera->setYaw(-90.0f); // Look towards center
        m_camera->setPitch(-15.0f); // Look down to see ground
    }
    
    // Preload common meshes into cache
    MeshCache::getInstance().preloadCommonMeshes();
    
    // Pre-allocate physics objects
    PhysicsObjectPool::getInstance().preallocateBodies();
    
    // Get meshes from cache
    m_cubeMesh = MeshCache::getInstance().getMesh(MeshCache::CUBE_KEY);
    m_sphereMesh = MeshCache::getInstance().getMesh(MeshCache::SPHERE_KEY);
    m_groundMesh = MeshCache::getInstance().getMesh(MeshCache::GROUND_PLANE_KEY);
    
    // Verify meshes were loaded
    if (!m_cubeMesh || !m_sphereMesh || !m_groundMesh) {
        std::cerr << "Failed to load meshes from cache" << std::endl;
        return false;
    }
    
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
    
    // Add more objects for performance testing only if in performance test mode
    if (m_performanceTestMode) {
        createPerformanceTestObjects();
    }
    
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
               size_t meshCacheSize = MeshCache::getInstance().getCacheSize();
               size_t inertiaCacheSize = InertiaTensorCache::getInstance().getCacheSize();
               size_t objectPoolAvailable = PhysicsObjectPool::getInstance().getTotalAvailable();
               size_t objectPoolReused = PhysicsObjectPool::getInstance().getTotalReused();
               // Estimate collision checks (n*(n-1)/2 for pairwise checks)
               int collisionChecks = (m_objectCount * (m_objectCount - 1)) / 2;
               m_fpsRenderer->update(deltaTime, m_objectCount, collisionChecks, drawCalls, trianglesRendered, meshCacheSize, inertiaCacheSize, objectPoolAvailable, objectPoolReused);
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
    
    // Render all objects
    for (size_t i = 0; i < m_bodies.size(); ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, m_bodies[i]->getPosition());
        model = model * glm::mat4_cast(m_bodies[i]->getRotation());
        
        m_shader->setUniform("model", model);
        
        // Determine object type and color
        if (i == 0) {
            // Original cube
            m_shader->setUniform("uColor", glm::vec3(0.2f, 0.8f, 1.0f)); // Cyan
            m_cubeMesh->draw();
        } else if (i == 1) {
            // Original sphere
            m_shader->setUniform("uColor", glm::vec3(1.0f, 0.4f, 0.3f)); // Orange-red
            m_sphereMesh->draw();
        } else {
            // Performance test objects
            // Alternate between cubes and spheres with different colors
            bool isSphere = (i - 2) % 2 == 0; // -2 to account for original cube and sphere
            if (isSphere) {
                m_shader->setUniform("uColor", glm::vec3(0.8f, 0.2f, 0.8f)); // Magenta
                // Use cached small sphere mesh for performance test objects
                auto smallSphereMesh = MeshCache::getInstance().getMesh(MeshCache::SPHERE_SMALL_KEY);
                if (smallSphereMesh) {
                    smallSphereMesh->draw();
                } else {
                    m_sphereMesh->draw(); // Fallback to regular sphere
                }
            } else {
                m_shader->setUniform("uColor", glm::vec3(0.2f, 0.8f, 0.2f)); // Green
                // Use cached small cube mesh for performance test objects
                auto smallCubeMesh = MeshCache::getInstance().getMesh(MeshCache::CUBE_SMALL_KEY);
                if (smallCubeMesh) {
                    smallCubeMesh->draw();
                } else {
                    m_cubeMesh->draw(); // Fallback to regular cube
                }
            }
        }
    }
    
    // Render ground (always visible)
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

void BasicDemoScenario::createPerformanceTestObjects() {
    std::cout << "Creating performance test objects..." << std::endl;
    
    // Create a grid of objects for performance testing
    const int GRID_SIZE = 15; // 15x15 = 225 objects
    const float SPACING = 2.0f;
    
    for (int x = 0; x < GRID_SIZE; ++x) {
        for (int z = 0; z < GRID_SIZE; ++z) {
            float xPos = (x - GRID_SIZE/2.0f) * SPACING;
            float zPos = (z - GRID_SIZE/2.0f) * SPACING;
            float yPos = 5.0f + (x + z) * 0.1f; // Slight height variation
            
            // Alternate between cubes and spheres
            bool isSphere = (x + z) % 2 == 0;
            
            std::unique_ptr<RigidBody3D> body;
            if (isSphere) {
                body = std::make_unique<RigidBody3D>(std::make_unique<Sphere>(0.3f), 1.0f);
            } else {
                body = std::make_unique<RigidBody3D>(std::make_unique<Box>(0.4f, 0.4f, 0.4f), 1.0f);
            }
            body->setPosition(glm::vec3(xPos, yPos, zPos));
            
            m_world->AddBody(body.get());
            m_bodies.push_back(std::move(body));
        }
    }
    
    // Update object count
    m_objectCount = static_cast<int>(m_bodies.size());
    
    std::cout << "Performance test objects created: " << m_objectCount << " total objects" << std::endl;
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
