#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <mutex>

// Cache for inertia tensor calculations
class InertiaTensorCache {
public:
    // Singleton pattern
    static InertiaTensorCache& getInstance();
    
    // Get cached inertia tensor for a shape
    glm::mat3 getInertiaTensor(const std::string& key);
    
    // Cache an inertia tensor with a specific key
    void cacheInertiaTensor(const std::string& key, const glm::mat3& tensor);
    
    // Generate key for common shapes
    static std::string generateBoxKey(float width, float height, float depth, float mass);
    static std::string generateSphereKey(float radius, float mass);
    static std::string generateCylinderKey(float radius, float height, float mass);
    
    // Clear all cached tensors
    void clear();
    
    // Get cache statistics
    size_t getCacheSize() const;
    
private:
    InertiaTensorCache() = default;
    ~InertiaTensorCache() = default;
    
    // Disable copy constructor and assignment operator
    InertiaTensorCache(const InertiaTensorCache&) = delete;
    InertiaTensorCache& operator=(const InertiaTensorCache&) = delete;
    
    // Cache storage
    std::unordered_map<std::string, glm::mat3> m_cache;
    
    // Thread safety
    mutable std::mutex m_mutex;
};
