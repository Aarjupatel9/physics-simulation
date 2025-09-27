#include "BaseScene.h"
#include "../src/core/World.h"
#include "../src/rendering/Camera.h"
#include "../src/rendering/Shader.h"
#include "../src/rendering/Mesh.h"
#include "../src/rendering/MeshCache.h"
#include "../src/rendering/FPSRenderer.h"
#include "../src/core/RigidBody3D.h"
#include "../src/shapes/Box.h"
#include "../src/shapes/Sphere.h"
#include "../src/shapes/Plane.h"
#include "../src/utils/MeshGenerator.h"
#include <iostream>

BaseScene::BaseScene() {
    // Initialize common components
}

bool BaseScene::initialize(GLFWwindow* window) {
    std::cout << "Initializing " << getName() << "..." << std::endl;
    
    // Store window reference
    m_window = window;
    
    // Setup common components
    setupCommonComponents(window);
    
    // Load common meshes
    loadCommonMeshes();
    
    // Setup GLFW callbacks
    setupGLFWCallbacks(window);
    
    // Initialize scene-specific objects
    initializeObjects();
    
    std::cout << getName() << " initialized successfully!" << std::endl;
    return true;
}

void BaseScene::setupCommonComponents(GLFWwindow* window) {
    // Create physics world
    m_world = std::make_unique<World>(glm::vec3(0.0f, -9.81f, 0.0f));
    
    // Create camera
    m_camera = std::make_unique<Camera>();
    
    // Create shader
    m_shader = std::make_unique<Shader>();
    
    // Load shader from embedded source
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        out vec3 FragPos;
        out vec3 Normal;
        
        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 FragPos;
        in vec3 Normal;
        
        uniform vec3 uColor;
        uniform vec3 lightPos;
        uniform vec3 lightColor;
        
        void main() {
            // Ambient lighting
            float ambient = 0.5;
            
            // Diffuse lighting
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            
            vec3 result = (ambient + diff) * lightColor * uColor;
            FragColor = vec4(result, 1.0);
        }
    )";
    
    if (!m_shader->loadFromSource(vertexShaderSource, fragmentShaderSource)) {
        std::cerr << "Failed to load shader!" << std::endl;
        return;
    }
    std::cout << "Shader loaded successfully!" << std::endl;
    
    // Create FPS renderer
    m_fpsRenderer = std::make_unique<FPSRenderer>();
    m_fpsRenderer->initialize();
    
    std::cout << "Common components setup complete" << std::endl;
}

void BaseScene::loadCommonMeshes() {
    std::cout << "Loading common meshes..." << std::endl;
    
    // First, preload all common meshes
    MeshCache::getInstance().preloadCommonMeshes();
    
    // Then, get the meshes from cache
    m_boxMesh = MeshCache::getInstance().getMesh(MeshCache::CUBE_KEY);
    m_sphereMesh = MeshCache::getInstance().getMesh(MeshCache::SPHERE_KEY);
    m_planeMesh = MeshCache::getInstance().getMesh(MeshCache::GROUND_PLANE_KEY);
    
    // Verify meshes are loaded
    if (!m_boxMesh || !m_sphereMesh || !m_planeMesh) {
        std::cerr << "Error: Failed to load one or more meshes!" << std::endl;
    }
    
    std::cout << "Common meshes loaded" << std::endl;
}

void BaseScene::setupGLFWCallbacks(GLFWwindow* window) {
    // Set up GLFW callbacks for camera controls
    glfwSetCursorPosCallback(window, Camera::mouseCallback);
    glfwSetScrollCallback(window, Camera::scrollCallback);
    glfwSetKeyCallback(window, Camera::keyCallback);
    
    // Initially disable cursor since controls are enabled by default
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void BaseScene::createBox(glm::vec3 position, 
                         glm::vec3 scale,
                         glm::vec3 rotation,
                         glm::vec3 color,
                         bool enablePhysics,
                         float mass) {
    // Create physics body
    auto physicsBody = std::make_unique<RigidBody3D>(
        std::make_unique<Box>(scale.x, scale.y, scale.z),
        enablePhysics ? mass : 0.0f
    );
    
    physicsBody->setPosition(position);
    if (rotation != glm::vec3(0.0f)) {
        physicsBody->setRotation(rotation);
    }
    physicsBody->setStatic(!enablePhysics);
    
    // Store object info
    ObjectInfo objInfo;
    objInfo.physicsBody = std::move(physicsBody);
    objInfo.color = color;
    objInfo.mesh = m_boxMesh;
    
    // Add to world if physics enabled
    if (enablePhysics) {
        m_world->AddBody(objInfo.physicsBody.get());
        m_physicsObjects.push_back(objInfo.physicsBody.get());
    }
    
    m_objects.push_back(std::move(objInfo));
    
    std::cout << "Created box at (" << position.x << ", " << position.y << ", " << position.z 
              << ") with scale (" << scale.x << ", " << scale.y << ", " << scale.z 
              << "), physics: " << (enablePhysics ? "enabled" : "disabled") << std::endl;
}

void BaseScene::createSphere(glm::vec3 position, 
                            float radius,
                            glm::vec3 color,
                            bool enablePhysics,
                            float mass,
                            glm::vec3 initialVelocity) {
    // Create physics body
    auto physicsBody = std::make_unique<RigidBody3D>(
        std::make_unique<Sphere>(radius),
        enablePhysics ? mass : 0.0f
    );
    
    physicsBody->setPosition(position);
    physicsBody->setStatic(!enablePhysics);
        
    // Store object info
    ObjectInfo objInfo;
    objInfo.physicsBody = std::move(physicsBody);
    objInfo.color = color;
    objInfo.mesh = m_sphereMesh;
    
    // Add to world if physics enabled
    if (enablePhysics) {
        m_world->AddBody(objInfo.physicsBody.get());
        m_physicsObjects.push_back(objInfo.physicsBody.get());
    }
    
    m_objects.push_back(std::move(objInfo));
    
    std::cout << "Created sphere at (" << position.x << ", " << position.y << ", " << position.z 
              << ") with radius " << radius 
              << ", physics: " << (enablePhysics ? "enabled" : "disabled") << std::endl;
}

void BaseScene::createPlane(glm::vec3 position, 
                           glm::vec2 size,
                           glm::vec3 rotation,
                           glm::vec3 color,
                           bool enablePhysics) {
    // Create physics body with correct dimensions
    auto physicsBody = std::make_unique<RigidBody3D>(
        std::make_unique<Plane>(size.x, size.y),
        enablePhysics ? 0.0f : 0.0f  // Planes are always static for now
    );
    
    std::cout << "Created plane at (" << position.x << ", " << position.y << ", " << position.z 
              << ") with size (" << size.x << " x " << size.y << ")"
              << ", physics: " << (enablePhysics ? "enabled" : "disabled") << std::endl;
    
    physicsBody->setPosition(position);
    if (rotation != glm::vec3(0.0f)) {
        physicsBody->setRotation(rotation);
    }
    physicsBody->setStatic(true);
    
    // Store object info
    ObjectInfo objInfo;
    objInfo.physicsBody = std::move(physicsBody);
    objInfo.color = color;
    objInfo.mesh = m_planeMesh;
    
    // Add to world if physics enabled
    if (enablePhysics) {
        m_world->AddBody(objInfo.physicsBody.get());
        m_physicsObjects.push_back(objInfo.physicsBody.get());
    }
    
    m_objects.push_back(std::move(objInfo));
    
    std::cout << "Created plane at (" << position.x << ", " << position.y << ", " << position.z 
              << ") with size (" << size.x << ", " << size.y << ")" << std::endl;
}

void BaseScene::renderObject(const RigidBody3D& body, glm::vec3 color) {
    // Create model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, body.getPosition());
    
    // Apply rotation if needed
    // TODO: Add rotation support
    
    // Apply scale based on shape type and get the correct mesh
    std::shared_ptr<Mesh> meshToRender = nullptr;
    
    // Debug: Check what type of shape we have
    const BaseShape* shape = body.getShape();
    if (!shape) {
        std::cout << "Warning: No shape found for object" << std::endl;
        return;
    }
    
    // Try to cast to different shape types
    if (const Box* box = dynamic_cast<const Box*>(shape)) {
        glm::vec3 dims = box->getDimensions();
        model = glm::scale(model, dims);
        meshToRender = m_boxMesh;
    } else if (const Sphere* sphere = dynamic_cast<const Sphere*>(shape)) {
        float radius = sphere->getRadius();
        // Base mesh is 1-unit radius, so scale directly by radius
        model = glm::scale(model, glm::vec3(radius));
        meshToRender = m_sphereMesh;
    } else if (const Plane* plane = dynamic_cast<const Plane*>(shape)) {
        // Scale the plane mesh to match the actual plane size
        // Base mesh is now 1x1, so scale directly by dimensions
        glm::vec2 dimensions = plane->getDimensions();
        model = glm::scale(model, glm::vec3(dimensions.x, 1.0f, dimensions.y));
        meshToRender = m_planeMesh;
    } else {
        std::cout << "Warning: Unknown shape type: " << typeid(*shape).name() << std::endl;
    }
    
    // Set uniforms
    m_shader->setUniform("model", model);
    m_shader->setUniform("uColor", color);
    m_shader->setUniform("lightPos", glm::vec3(10.0f, 10.0f, 10.0f));
    m_shader->setUniform("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    
    // Render the appropriate mesh
    if (meshToRender) {
        meshToRender->draw();
    } else {
        std::cout << "Warning: No mesh found for object type" << std::endl;
    }
}

void BaseScene::renderAllObjects() {
    // Render all objects (both static and physics)
    for (const auto& obj : m_objects) {
        if (obj.physicsBody) {
            renderObject(*obj.physicsBody, obj.color);
        }
    }
}

glm::mat4 BaseScene::getViewMatrix() const {
    return m_camera ? m_camera->getViewMatrix() : glm::mat4(1.0f);
}

glm::mat4 BaseScene::getProjectionMatrix() const {
    return m_camera ? m_camera->getProjectionMatrix(800.0f/600.0f) : glm::mat4(1.0f);
}

void BaseScene::update(float deltaTime) {
    // Update physics world
    if (m_world) {
        m_world->Update(deltaTime);
    }
    
    // Update camera
    if (m_camera) {
        m_camera->update(m_window, deltaTime);
    }
    
    // Update FPS renderer with current metrics
    if (m_fpsRenderer) {
        // Calculate object count from physics bodies
        int objectCount = static_cast<int>(m_objects.size());
        
        // Estimate collision checks (simple approximation)
        int collisionChecks = objectCount * objectCount / 2; // N*(N-1)/2 for all pairs
        
        // Estimate draw calls (objects + 1 for each mesh type used)
        int drawCalls = static_cast<int>(m_objects.size());
        
        // Estimate triangles (rough approximation)
        int trianglesRendered = objectCount * 12; // Assuming ~12 triangles per object on average
        
        m_fpsRenderer->update(deltaTime, objectCount, collisionChecks, drawCalls, trianglesRendered);
    }
    
    // Update scene-specific logic
    updateScene(deltaTime);
}

void BaseScene::render() {
    // Clear screen
    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use shader
    m_shader->use();
    
    // Set view and projection matrices
    glm::mat4 view = getViewMatrix();
    glm::mat4 projection = getProjectionMatrix();
    
    m_shader->setUniform("view", view);
    m_shader->setUniform("projection", projection);
    
    // Render all objects
    renderAllObjects();
    
    // Render scene-specific objects
    renderScene();
    
    // Render FPS
    renderFPS();
}

void BaseScene::cleanup() {
    std::cout << "Cleaning up " << getName() << "..." << std::endl;
    
    // Clear objects
    m_objects.clear();
    m_physicsObjects.clear();
    
    // Reset components
    m_world.reset();
    m_camera.reset();
    m_shader.reset();
    m_fpsRenderer.reset();
    
    std::cout << getName() << " cleanup complete" << std::endl;
}
