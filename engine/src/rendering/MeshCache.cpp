#include "MeshCache.h"
#include "Mesh.h"
#include "../utils/MeshGenerator.h"
#include <iostream>

// Static key definitions are now constexpr in header

MeshCache& MeshCache::getInstance() {
    static MeshCache instance;
    return instance;
}

std::shared_ptr<Mesh> MeshCache::getMesh(const std::string& key) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        return it->second;
    }
    
    // Mesh not found in cache
    return nullptr;
}

void MeshCache::cacheMesh(const std::string& key, std::shared_ptr<Mesh> mesh) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cache[key] = mesh;
}

void MeshCache::preloadCommonMeshes() {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::cout << "Preloading common meshes..." << std::endl;
    
    // Preload cube mesh
    if (m_cache.find(CUBE_KEY) == m_cache.end()) {
        auto cubeMesh = std::make_shared<Mesh>();
        cubeMesh->loadVertices(MeshGenerator::generateCube());
        m_cache[CUBE_KEY] = cubeMesh;
        std::cout << "Cached cube mesh" << std::endl;
    }
    
    // Preload sphere mesh
    if (m_cache.find(SPHERE_KEY) == m_cache.end()) {
        auto sphereMesh = std::make_shared<Mesh>();
        sphereMesh->loadVertices(MeshGenerator::generateSphere(32, 16, 1.0f));
        m_cache[SPHERE_KEY] = sphereMesh;
        std::cout << "Cached sphere mesh (1.0f radius)" << std::endl;
    }
    
    // Preload ground plane mesh
    if (m_cache.find(GROUND_PLANE_KEY) == m_cache.end()) {
        auto groundMesh = std::make_shared<Mesh>();
        groundMesh->loadVertices(MeshGenerator::generateGroundPlane());
        m_cache[GROUND_PLANE_KEY] = groundMesh;
        std::cout << "Cached ground plane mesh" << std::endl;
    }
    
    // Preload small sphere mesh (for performance test)
    if (m_cache.find(SPHERE_SMALL_KEY) == m_cache.end()) {
        auto smallSphereMesh = std::make_shared<Mesh>();
        smallSphereMesh->loadVertices(MeshGenerator::generateSphere(32, 16, 0.3f));
        m_cache[SPHERE_SMALL_KEY] = smallSphereMesh;
        std::cout << "Cached small sphere mesh (0.3f radius)" << std::endl;
    }
    
    // Preload small cube mesh (for performance test)
    if (m_cache.find(CUBE_SMALL_KEY) == m_cache.end()) {
        auto smallCubeMesh = std::make_shared<Mesh>();
        // Generate a smaller cube by scaling the vertices
        auto cubeVertices = MeshGenerator::generateCube();
        for (size_t i = 0; i < cubeVertices.size(); i += 3) {
            cubeVertices[i] *= 0.4f;     // x
            cubeVertices[i + 1] *= 0.4f; // y
            cubeVertices[i + 2] *= 0.4f; // z
        }
        smallCubeMesh->loadVertices(cubeVertices);
        m_cache[CUBE_SMALL_KEY] = smallCubeMesh;
        std::cout << "Cached small cube mesh (0.4f scale)" << std::endl;
    }
    
    std::cout << "Mesh cache preloading complete. Total cached meshes: " << m_cache.size() << std::endl;
}

void MeshCache::clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cache.clear();
    std::cout << "Mesh cache cleared" << std::endl;
}

size_t MeshCache::getCacheSize() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_cache.size();
}

size_t MeshCache::getMemoryUsage() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    // Rough estimate: each mesh has vertices, assume average 1000 vertices * 3 floats * 4 bytes
    return m_cache.size() * 1000 * 3 * 4;
}

std::string MeshCache::generateSphereKey(float radius, int longitudeSegments, int latitudeSegments) {
    return "sphere_" + std::to_string(radius) + "_" + 
           std::to_string(longitudeSegments) + "_" + 
           std::to_string(latitudeSegments);
}

std::string MeshCache::generateCubeKey(float size) {
    return "cube_" + std::to_string(size);
}

std::string MeshCache::generateBoxKey(float width, float height, float depth) {
    return "box_" + std::to_string(width) + "_" + 
           std::to_string(height) + "_" + 
           std::to_string(depth);
}
