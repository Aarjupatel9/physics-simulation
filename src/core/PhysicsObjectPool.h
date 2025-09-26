#pragma once

#include <unordered_map>
#include <vector>
#include <queue>
#include <memory>
#include <mutex>
#include <string>
#include <iostream>

// Forward declarations
class RigidBody3D;
class BaseShape;
class Box;
class Sphere;

// Specialized object pool for physics bodies with different shapes
class PhysicsObjectPool {
public:
    // Singleton pattern
    static PhysicsObjectPool& getInstance();
    
    // Get a rigid body from the pool
    std::unique_ptr<RigidBody3D> acquireRigidBody(std::unique_ptr<BaseShape> shape, float mass);
    
    // Return a rigid body to the pool
    void releaseRigidBody(std::unique_ptr<RigidBody3D> body);
    
    // Pre-allocate bodies for common shapes
    void preallocateBodies();
    
    // Get pool statistics
    size_t getTotalAvailable() const;
    size_t getTotalCreated() const;
    size_t getTotalReused() const;
    void printStatistics() const;
    
    // Clear all pools
    void clear();

private:
    PhysicsObjectPool() = default;
    ~PhysicsObjectPool() = default;
    
    // Disable copy constructor and assignment operator
    PhysicsObjectPool(const PhysicsObjectPool&) = delete;
    PhysicsObjectPool& operator=(const PhysicsObjectPool&) = delete;
    
    // Pool for different shape types
    struct ShapePool {
        std::queue<std::unique_ptr<RigidBody3D>> available;
        size_t totalCreated = 0;
        size_t totalReused = 0;
        mutable std::mutex mutex;
    };
    
    std::unordered_map<std::string, ShapePool> m_shapePools;
    mutable std::mutex m_globalMutex;
    
    // Helper methods
    std::string generateShapeKey(const BaseShape* shape, float mass);
    void resetRigidBody(RigidBody3D* body);
};
