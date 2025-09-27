#include "BallCollisionDemo.h"
#include "../src/core/World.h"
#include "../src/core/PhysicsConstants.h"
#include "../src/rendering/Camera.h"
#include "../src/rendering/Shader.h"
#include "../src/rendering/Mesh.h"
#include "../src/rendering/FPSRenderer.h"
#include "../src/rendering/MeshCache.h"
#include "../src/core/PhysicsObjectPool.h"
#include "../src/core/InertiaTensorCache.h"
#include "../src/shapes/Sphere.h"
#include "../src/shapes/Box.h"
#include "../src/shapes/Plane.h"
#include <iostream>

BallCollisionDemo::BallCollisionDemo() 
    : m_gen(m_rd()), 
      m_velDist(-8.0f, 8.0f), // Increased velocity range for more momentum
      m_posDist(-PLANE_SIZE/2.0f + BALL_RADIUS, PLANE_SIZE/2.0f - BALL_RADIUS) {
}

bool BallCollisionDemo::initialize(GLFWwindow* window) {
    std::cout << "Initializing Ball Collision Demo..." << std::endl;
    
    // Store window reference
    m_window = window;
    
    // Initialize core systems
    m_world = std::make_unique<World>(glm::vec3(0.0f, -9.81f, 0.0f)); // Standard gravity
    m_camera = std::make_unique<Camera>();
    m_shader = std::make_unique<Shader>();
    m_fpsRenderer = std::make_unique<FPSRenderer>();
    
    // Initialize shader with embedded shader source
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uColor;
        void main() {
            FragColor = vec4(uColor, 1.0);
        }
    )";
    
    if (!m_shader->loadFromSource(vertexShaderSource, fragmentShaderSource)) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return false;
    }
    
    // Initialize FPS renderer
    if (!m_fpsRenderer->initialize()) {
        std::cerr << "Failed to initialize FPS renderer!" << std::endl;
        return false;
    }
    
    // Set up camera for better viewing of 5x5 plane
    m_camera->setPosition(glm::vec3(0.0f, 8.0f, 8.0f)); // Closer for better view of smaller plane
    m_camera->setYaw(-90.0f); // Look towards center
    m_camera->setPitch(-25.0f); // Look down at the plane
    
    // Set up GLFW callbacks for mouse and keyboard input
    glfwSetCursorPosCallback(window, Camera::mouseCallback);
    glfwSetScrollCallback(window, Camera::scrollCallback);
    glfwSetKeyCallback(window, Camera::keyCallback);
    
    // Preload common meshes into cache
    MeshCache::getInstance().preloadCommonMeshes();
    
    // Pre-allocate physics objects
    PhysicsObjectPool::getInstance().preallocateBodies();
    
    // Get meshes from cache
    m_ballMesh = MeshCache::getInstance().getMesh(MeshCache::SPHERE_KEY);
    m_floorMesh = MeshCache::getInstance().getMesh(MeshCache::GROUND_PLANE_KEY);
    
    // Create a simple wall mesh (we'll generate it programmatically)
    m_wallMesh = std::make_shared<Mesh>();
    // For now, use the cube mesh for walls
    m_wallMesh = MeshCache::getInstance().getMesh(MeshCache::CUBE_KEY);
    
    // Create the scene objects
    createFloor();
    createWalls();
    createBalls();
    
    // Count total objects
    m_objectCount = static_cast<int>(m_balls.size() + 1 + m_walls.size()); // balls + floor + walls
    
    std::cout << "Ball Collision Demo initialized successfully!" << std::endl;
    std::cout << "  - Floor: " << PLANE_SIZE << "x" << PLANE_SIZE << " meters" << std::endl;
    std::cout << "  - Walls: 4 walls with " << WALL_HEIGHT << "m height, " << WALL_WIDTH << "m width" << std::endl;
    std::cout << "  - Balls: " << NUM_BALLS << " balls with " << BALL_RADIUS << "m radius" << std::endl;
    std::cout << "  - Total objects: " << m_objectCount << std::endl;
    
    return true;
}

void BallCollisionDemo::update(float deltaTime) {
    // Update physics world
    m_world->Update(deltaTime);
    
    // Update camera
    m_camera->update(m_window, deltaTime); // Pass window to camera
    
    // Update FPS renderer
    if (m_fpsRenderer) {
        int drawCalls = 1 + static_cast<int>(m_balls.size() + m_walls.size()); // floor + balls + walls
        int trianglesRendered = 2 + (32 * static_cast<int>(m_balls.size())) + (12 * static_cast<int>(m_walls.size()));
        
        size_t meshCacheSize = MeshCache::getInstance().getCacheSize();
        size_t inertiaCacheSize = InertiaTensorCache::getInstance().getCacheSize();
        size_t objectPoolAvailable = PhysicsObjectPool::getInstance().getTotalAvailable();
        size_t objectPoolReused = PhysicsObjectPool::getInstance().getTotalReused();
        
        // Estimate collision checks (n*(n-1)/2 for pairwise checks)
        int collisionChecks = (m_objectCount * (m_objectCount - 1)) / 2;
        
        m_fpsRenderer->update(deltaTime, m_objectCount, collisionChecks, drawCalls, trianglesRendered, 
                             meshCacheSize, inertiaCacheSize, objectPoolAvailable, objectPoolReused);
    }
}

void BallCollisionDemo::render() {
    // Clear screen
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up shader
    m_shader->use();
    
    // Set up matrices
    glm::mat4 view = m_camera->getViewMatrix();
    glm::mat4 projection = m_camera->getProjectionMatrix(800.0f/600.0f); // Assuming 800x600
    
    m_shader->setUniform("view", view);
    m_shader->setUniform("projection", projection);
    
    // Render floor
    glm::mat4 floorModel = glm::mat4(1.0f);
    floorModel = glm::scale(floorModel, glm::vec3(PLANE_SIZE, 1.0f, PLANE_SIZE)); // Scale to 5x5
    floorModel = glm::translate(floorModel, glm::vec3(0.0f, -0.5f, 0.0f)); // Position at y=0
    
    // Debug: Log floor rendering
    std::cout << "FLOOR RENDER - Scale: (" << PLANE_SIZE << ", 1.0, " << PLANE_SIZE << "), Translate: (0, -0.5, 0)" << std::endl;
    std::cout << "FLOOR RENDER - Final position should be: (0, -0.5, 0) with 5x5x1 scale" << std::endl;
    
    m_shader->setUniform("model", floorModel);
    m_shader->setUniform("uColor", glm::vec3(0.3f, 0.3f, 0.3f)); // Dark grey
    m_floorMesh->draw();
    
    // Render walls
    m_shader->setUniform("uColor", glm::vec3(0.6f, 0.4f, 0.2f)); // Brown color for walls
    
    int wallIndex = 0;
    std::string wallNames[] = {"North", "South", "East", "West"};
    
    for (const auto& wall : m_walls) {
        glm::mat4 wallModel = glm::mat4(1.0f);
        wallModel = glm::translate(wallModel, wall->getPosition());
        
        // Get the actual wall dimensions from the physics body's shape
        const Box* boxShape = static_cast<const Box*>(wall->getShape());
        glm::vec3 wallDims = boxShape->getDimensions();
        glm::vec3 wallPos = wall->getPosition();
        wallModel = glm::scale(wallModel, wallDims);
        
        // Debug: Log wall rendering
        std::cout << wallNames[wallIndex] << " Wall RENDER - Position: (" 
                  << wallPos.x << ", " << wallPos.y << ", " << wallPos.z 
                  << "), Scale: (" << wallDims.x << ", " << wallDims.y << ", " << wallDims.z << ")" << std::endl;
        
        m_shader->setUniform("model", wallModel);
        m_wallMesh->draw();
        
        wallIndex++;
    }
    
    // Render balls
    for (const auto& ball : m_balls) {
        glm::mat4 ballModel = glm::mat4(1.0f);
        ballModel = glm::translate(ballModel, ball->getPosition());
        ballModel = glm::scale(ballModel, glm::vec3(BALL_RADIUS * 2.0f)); // Scale to radius
        m_shader->setUniform("model", ballModel);
        
        // Color balls based on their velocity (faster = redder)
        glm::vec3 velocity = ball->getVelocity();
        float speed = glm::length(velocity);
        float maxSpeed = 10.0f; // Normalize to max expected speed
        float intensity = std::min(speed / maxSpeed, 1.0f);
        glm::vec3 color = glm::mix(glm::vec3(0.2f, 0.8f, 0.2f), glm::vec3(1.0f, 0.2f, 0.2f), intensity);
        m_shader->setUniform("uColor", color);
        
        m_ballMesh->draw();
    }
    
    // Render FPS display
    m_fpsRenderer->render(view, projection);
}

void BallCollisionDemo::cleanup() {
    std::cout << "Cleaning up Ball Collision Demo..." << std::endl;
    
    // FPS renderer cleanup is handled automatically
    
    // Clear all objects
    m_balls.clear();
    m_walls.clear();
    m_floor.reset();
    m_world.reset();
}

void BallCollisionDemo::createBalls() {
    std::cout << "Creating " << NUM_BALLS << " balls..." << std::endl;
    
    for (int i = 0; i < NUM_BALLS; ++i) {
        // Create ball physics body
        auto ballBody = std::make_unique<RigidBody3D>(
            std::make_unique<Sphere>(BALL_RADIUS), 
            1.0f // 1 kg mass
        );
        
        // Set random position (within the bounded area)
        glm::vec3 position = getRandomPosition();
        position.y = BALL_RADIUS + 0.1f; // Slightly above ground
        ballBody->setPosition(position);
        
        // Set random initial velocity
        glm::vec3 velocity = getRandomVelocity();
        ballBody->setVelocity(velocity);
        
        // Add to world
        m_world->AddBody(ballBody.get());
        m_balls.push_back(std::move(ballBody));
    }
    
    std::cout << "Created " << m_balls.size() << " balls" << std::endl;
}

void BallCollisionDemo::createFloor() {
    std::cout << "Creating floor..." << std::endl;
    
    // Create floor physics body
    m_floor = std::make_unique<RigidBody3D>(
        std::make_unique<Plane>(), 
        0.0f // Infinite mass (static)
    );
    
    m_floor->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    m_floor->setStatic(true); // Make it static
    
    // Debug: Log floor physics assignment
    glm::vec3 floorPos = m_floor->getPosition();
    std::cout << "FLOOR PHYSICS - Position: (" << floorPos.x << ", " << floorPos.y << ", " << floorPos.z << ")" << std::endl;
    std::cout << "FLOOR PHYSICS - PLANE_SIZE: " << PLANE_SIZE << " meters (5x5)" << std::endl;
    
    m_world->AddBody(m_floor.get());
    
    std::cout << "Floor created" << std::endl;
}

void BallCollisionDemo::createWalls() {
    std::cout << "Creating boundary walls..." << std::endl;
    
    // Create 4 walls around the perimeter
    float halfSize = PLANE_SIZE / 2.0f;
    
    // Wall positions and rotations
    struct WallInfo {
        glm::vec3 position;
        glm::vec3 rotation; // Euler angles in degrees
        glm::vec3 scale;
    };
    
    // Position walls to form a boundary around the 5x5 plane
    // North/South walls: 5m width, positioned at the edges
    // East/West walls: 4.6m length, positioned at the edges with gaps at corners
    
    float eastWestWallLength = 4.6f; // 4.6m length for east/west walls
    
    std::vector<WallInfo> wallInfos = {
        // North wall - positioned INSIDE +Z edge, spans full width (5m), sits ON GROUND (y=0)
        {glm::vec3(0.0f, WALL_HEIGHT/2.0f, halfSize - WALL_WIDTH/2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(PLANE_SIZE, WALL_HEIGHT, WALL_WIDTH)},
        // South wall - positioned INSIDE -Z edge, spans full width (5m), sits ON GROUND (y=0)
        {glm::vec3(0.0f, WALL_HEIGHT/2.0f, -halfSize + WALL_WIDTH/2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(PLANE_SIZE, WALL_HEIGHT, WALL_WIDTH)},
        // East wall - positioned INSIDE +X edge, 4.6m length, sits ON GROUND (y=0)
        {glm::vec3(halfSize - WALL_WIDTH/2.0f, WALL_HEIGHT/2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(WALL_WIDTH, WALL_HEIGHT, eastWestWallLength)},
        // West wall - positioned INSIDE -X edge, 4.6m length, sits ON GROUND (y=0)
        {glm::vec3(-halfSize + WALL_WIDTH/2.0f, WALL_HEIGHT/2.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(WALL_WIDTH, WALL_HEIGHT, eastWestWallLength)}
    };
    
    for (size_t i = 0; i < wallInfos.size(); ++i) {
        const auto& wallInfo = wallInfos[i];
        
        std::string wallNames[] = {"North", "South", "East", "West"};
        std::cout << wallNames[i] << " Wall ASSIGNMENT - Position: (" 
                  << wallInfo.position.x << ", " << wallInfo.position.y << ", " << wallInfo.position.z 
                  << "), Scale: (" << wallInfo.scale.x << ", " << wallInfo.scale.y << ", " << wallInfo.scale.z << ")" << std::endl;
        
        auto wallBody = std::make_unique<RigidBody3D>(
            std::make_unique<Box>(wallInfo.scale.x, wallInfo.scale.y, wallInfo.scale.z),
            0.0f // Infinite mass (static)
        );
        
        wallBody->setPosition(wallInfo.position);
        wallBody->setStatic(true);
        
        // Debug: Log actual physics body dimensions after creation
        const Box* boxShape = static_cast<const Box*>(wallBody->getShape());
        glm::vec3 actualDims = boxShape->getDimensions();
        glm::vec3 actualPos = wallBody->getPosition();
        std::cout << wallNames[i] << " Wall PHYSICS - Position: (" 
                  << actualPos.x << ", " << actualPos.y << ", " << actualPos.z 
                  << "), Dimensions: (" << actualDims.x << ", " << actualDims.y << ", " << actualDims.z << ")" << std::endl;
        
        m_world->AddBody(wallBody.get());
        m_walls.push_back(std::move(wallBody));
    }
    
    std::cout << "Created " << m_walls.size() << " boundary walls" << std::endl;
}

glm::vec3 BallCollisionDemo::getRandomVelocity() {
    return glm::vec3(
        m_velDist(m_gen),
        std::abs(m_velDist(m_gen)) * 0.3f, // Small upward velocity to prevent balls from sticking to ground
        m_velDist(m_gen)
    );
}

glm::vec3 BallCollisionDemo::getRandomPosition() {
    return glm::vec3(
        m_posDist(m_gen),
        0.0f, // Will be set to radius + small offset
        m_posDist(m_gen)
    );
}
