#include "InertiaTensorCache.h"
#include <iostream>
#include <sstream>
#include <iomanip>

InertiaTensorCache& InertiaTensorCache::getInstance() {
    static InertiaTensorCache instance;
    return instance;
}

glm::mat3 InertiaTensorCache::getInertiaTensor(const std::string& key) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        return it->second;
    }
    
    // Return identity matrix if not found
    return glm::mat3(1.0f);
}

void InertiaTensorCache::cacheInertiaTensor(const std::string& key, const glm::mat3& tensor) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cache[key] = tensor;
}

std::string InertiaTensorCache::generateBoxKey(float width, float height, float depth, float mass) {
    std::ostringstream oss;
    oss << "box_" << std::fixed << std::setprecision(2) 
        << width << "_" << height << "_" << depth << "_" << mass;
    return oss.str();
}

std::string InertiaTensorCache::generateSphereKey(float radius, float mass) {
    std::ostringstream oss;
    oss << "sphere_" << std::fixed << std::setprecision(2) 
        << radius << "_" << mass;
    return oss.str();
}

std::string InertiaTensorCache::generateCylinderKey(float radius, float height, float mass) {
    std::ostringstream oss;
    oss << "cylinder_" << std::fixed << std::setprecision(2) 
        << radius << "_" << height << "_" << mass;
    return oss.str();
}

void InertiaTensorCache::clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cache.clear();
    std::cout << "Inertia tensor cache cleared" << std::endl;
}

size_t InertiaTensorCache::getCacheSize() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_cache.size();
}
