#include "PhysicsObjectPool.h"
#include "RigidBody3D.h"
#include "BaseShape.h"
#include "../shapes/Box.h"
#include "../shapes/Sphere.h"
#include <sstream>
#include <iomanip>

PhysicsObjectPool& PhysicsObjectPool::getInstance() {
    static PhysicsObjectPool instance;
    return instance;
}

std::unique_ptr<RigidBody3D> PhysicsObjectPool::acquireRigidBody(std::unique_ptr<BaseShape> shape, float mass) {
    if (!shape) {
        return nullptr;
    }
    
    std::string key = generateShapeKey(shape.get(), mass);
    
    std::lock_guard<std::mutex> globalLock(m_globalMutex);
    auto& pool = m_shapePools[key];
    
    std::lock_guard<std::mutex> poolLock(pool.mutex);
    
    if (!pool.available.empty()) {
        // Reuse existing body
        std::unique_ptr<RigidBody3D> body = std::move(pool.available.front());
        pool.available.pop();
        pool.totalReused++;
        
        // Reset the body
        resetRigidBody(body.get());
        
        // Set new shape and mass
        body->setShape(std::move(shape));
        body->setMass(mass);
        
        return body;
    } else {
        // Create new body
        pool.totalCreated++;
        return std::make_unique<RigidBody3D>(std::move(shape), mass);
    }
}

void PhysicsObjectPool::releaseRigidBody(std::unique_ptr<RigidBody3D> body) {
    if (!body) return;
    
    // Get shape information for key generation
    const BaseShape* shape = body->getShape();
    if (!shape) {
        // Can't pool without shape information
        body.reset();
        return;
    }
    
    float mass = body->getMass();
    std::string key = generateShapeKey(shape, mass);
    
    std::lock_guard<std::mutex> globalLock(m_globalMutex);
    auto& pool = m_shapePools[key];
    
    std::lock_guard<std::mutex> poolLock(pool.mutex);
    
    // Reset the body
    resetRigidBody(body.get());
    
    // Add to pool (limit pool size to prevent memory bloat)
    if (pool.available.size() < 50) { // Max 50 bodies per shape type
        pool.available.push(std::move(body));
    } else {
        // Pool is full, destroy the body
        body.reset();
    }
}

void PhysicsObjectPool::preallocateBodies() {
    std::lock_guard<std::mutex> lock(m_globalMutex);
    
    std::cout << "Pre-allocating physics bodies..." << std::endl;
    
    // Pre-allocate common shapes and masses
    const size_t preallocCount = 20;
    
    // Box bodies
    for (size_t i = 0; i < preallocCount; ++i) {
        auto boxShape = std::make_unique<Box>(1.0f, 1.0f, 1.0f);
        auto body = std::make_unique<RigidBody3D>(std::move(boxShape), 10.0f);
        resetRigidBody(body.get());
        
        std::string key = generateShapeKey(body->getShape(), 10.0f);
        m_shapePools[key].available.push(std::move(body));
        m_shapePools[key].totalCreated++;
    }
    
    // Small box bodies (for performance test)
    for (size_t i = 0; i < preallocCount; ++i) {
        auto boxShape = std::make_unique<Box>(0.4f, 0.4f, 0.4f);
        auto body = std::make_unique<RigidBody3D>(std::move(boxShape), 1.0f);
        resetRigidBody(body.get());
        
        std::string key = generateShapeKey(body->getShape(), 1.0f);
        m_shapePools[key].available.push(std::move(body));
        m_shapePools[key].totalCreated++;
    }
    
    // Sphere bodies
    for (size_t i = 0; i < preallocCount; ++i) {
        auto sphereShape = std::make_unique<Sphere>(0.5f, 32);
        auto body = std::make_unique<RigidBody3D>(std::move(sphereShape), 5.0f);
        resetRigidBody(body.get());
        
        std::string key = generateShapeKey(body->getShape(), 5.0f);
        m_shapePools[key].available.push(std::move(body));
        m_shapePools[key].totalCreated++;
    }
    
    // Small sphere bodies (for performance test)
    for (size_t i = 0; i < preallocCount; ++i) {
        auto sphereShape = std::make_unique<Sphere>(0.3f, 32);
        auto body = std::make_unique<RigidBody3D>(std::move(sphereShape), 1.0f);
        resetRigidBody(body.get());
        
        std::string key = generateShapeKey(body->getShape(), 1.0f);
        m_shapePools[key].available.push(std::move(body));
        m_shapePools[key].totalCreated++;
    }
    
    std::cout << "Physics object pool pre-allocation complete" << std::endl;
    // printStatistics(); // Temporarily disabled to debug freezing issue
}

size_t PhysicsObjectPool::getTotalAvailable() const {
    std::lock_guard<std::mutex> lock(m_globalMutex);
    
    size_t total = 0;
    for (const auto& pair : m_shapePools) {
        std::lock_guard<std::mutex> poolLock(pair.second.mutex);
        total += pair.second.available.size();
    }
    return total;
}

size_t PhysicsObjectPool::getTotalCreated() const {
    std::lock_guard<std::mutex> lock(m_globalMutex);
    
    size_t total = 0;
    for (const auto& pair : m_shapePools) {
        std::lock_guard<std::mutex> poolLock(pair.second.mutex);
        total += pair.second.totalCreated;
    }
    return total;
}

size_t PhysicsObjectPool::getTotalReused() const {
    std::lock_guard<std::mutex> lock(m_globalMutex);
    
    size_t total = 0;
    for (const auto& pair : m_shapePools) {
        std::lock_guard<std::mutex> poolLock(pair.second.mutex);
        total += pair.second.totalReused;
    }
    return total;
}

void PhysicsObjectPool::printStatistics() const {
    // Don't use global mutex to avoid deadlock - just print basic info
    std::cout << "\n=== Physics Object Pool Statistics ===" << std::endl;
    std::cout << "Total pools: " << m_shapePools.size() << std::endl;
    std::cout << "=====================================" << std::endl;
}

void PhysicsObjectPool::clear() {
    std::lock_guard<std::mutex> lock(m_globalMutex);
    
    for (auto& pair : m_shapePools) {
        std::lock_guard<std::mutex> poolLock(pair.second.mutex);
        
        while (!pair.second.available.empty()) {
            pair.second.available.pop();
        }
        pair.second.totalCreated = 0;
        pair.second.totalReused = 0;
    }
    
    std::cout << "Physics object pool cleared" << std::endl;
}

std::string PhysicsObjectPool::generateShapeKey(const BaseShape* shape, float mass) {
    if (!shape) return "unknown_0.0";
    
    std::ostringstream oss;
    oss << shape->getTypeName() << "_" << std::fixed << std::setprecision(2) << mass;
    
    // Add specific shape parameters
    if (const Box* box = dynamic_cast<const Box*>(shape)) {
        glm::vec3 dims = box->getDimensions();
        oss << "_" << dims.x << "x" << dims.y << "x" << dims.z;
    } else if (const Sphere* sphere = dynamic_cast<const Sphere*>(shape)) {
        oss << "_r" << sphere->getRadius();
    }
    
    return oss.str();
}

void PhysicsObjectPool::resetRigidBody(RigidBody3D* body) {
    if (!body) return;
    
    // Reset to default state
    body->setPosition(glm::vec3(0.0f));
    body->setVelocity(glm::vec3(0.0f));
    body->setAngularVelocity(glm::vec3(0.0f));
    body->setRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
    body->setForce(glm::vec3(0.0f));
    body->setTorque(glm::vec3(0.0f));
}
