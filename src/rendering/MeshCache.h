#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <mutex>

// Forward declaration
class Mesh;

// Mesh cache for storing and reusing generated meshes
class MeshCache {
public:
    // Singleton pattern
    static MeshCache& getInstance();
    
    // Get or create a cached mesh
    std::shared_ptr<Mesh> getMesh(const std::string& key);
    
    // Cache a mesh with a specific key
    void cacheMesh(const std::string& key, std::shared_ptr<Mesh> mesh);
    
    // Generate and cache common meshes
    void preloadCommonMeshes();
    
    // Clear all cached meshes
    void clear();
    
    // Get cache statistics
    size_t getCacheSize() const;
    size_t getMemoryUsage() const;
    
    // Predefined mesh keys
    static constexpr const char* CUBE_KEY = "cube_1.0";
    static constexpr const char* SPHERE_KEY = "sphere_1.0_32_16";
    static constexpr const char* GROUND_PLANE_KEY = "ground_plane_1x1";
    static constexpr const char* SPHERE_SMALL_KEY = "sphere_0.3_32_16";
    static constexpr const char* CUBE_SMALL_KEY = "cube_0.4";

private:
    MeshCache() = default;
    ~MeshCache() = default;
    
    // Disable copy constructor and assignment operator
    MeshCache(const MeshCache&) = delete;
    MeshCache& operator=(const MeshCache&) = delete;
    
    // Cache storage
    std::unordered_map<std::string, std::shared_ptr<Mesh>> m_cache;
    
    // Thread safety
    mutable std::mutex m_mutex;
    
    // Helper methods
    std::string generateSphereKey(float radius, int longitudeSegments, int latitudeSegments);
    std::string generateCubeKey(float size);
    std::string generateBoxKey(float width, float height, float depth);
};
