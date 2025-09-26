#include "Box.h"
#include "../core/PhysicsConstants.h"
#include "../core/InertiaTensorCache.h"
#include <algorithm>

Box::Box(float width, float height, float depth) 
    : m_dimensions(glm::vec3(width, height, depth)) {
}

float Box::getVolume() const {
    glm::vec3 scaled = m_dimensions * m_scale;
    return scaled.x * scaled.y * scaled.z;
}

glm::mat3 Box::getInertiaTensor(float mass) const {
    glm::vec3 scaled = m_dimensions * m_scale;
    
    // Generate cache key
    std::string key = InertiaTensorCache::generateBoxKey(scaled.x, scaled.y, scaled.z, mass);
    
    // Check cache first
    auto& cache = InertiaTensorCache::getInstance();
    glm::mat3 cached = cache.getInertiaTensor(key);
    
    // If not identity matrix, return cached result
    if (cached != glm::mat3(1.0f)) {
        return cached;
    }
    
    // Calculate inertia tensor for a box centered at origin
    float w = scaled.x, h = scaled.y, d = scaled.z;
    float Ixx = mass * (h * h + d * d) / 12.0f;
    float Iyy = mass * (w * w + d * d) / 12.0f;
    float Izz = mass * (w * w + h * h) / 12.0f;
    
    glm::mat3 tensor(
        Ixx, 0.0f, 0.0f,
        0.0f, Iyy, 0.0f,
        0.0f, 0.0f, Izz
    );
    
    // Cache the result
    cache.cacheInertiaTensor(key, tensor);
    
    return tensor;
}

glm::vec3 Box::getBoundingBoxMin() const {
    glm::vec3 scaled = m_dimensions * m_scale;
    return -scaled * 0.5f;
}

glm::vec3 Box::getBoundingBoxMax() const {
    glm::vec3 scaled = m_dimensions * m_scale;
    return scaled * 0.5f;
}

glm::vec3 Box::getCenter() const {
    return glm::vec3(0.0f);
}

bool Box::containsPoint(const glm::vec3& point) const {
    glm::vec3 scaled = m_dimensions * m_scale;
    glm::vec3 half = scaled * 0.5f;
    
    return (point.x >= -half.x && point.x <= half.x &&
            point.y >= -half.y && point.y <= half.y &&
            point.z >= -half.z && point.z <= half.z);
}

void Box::setDimensions(const glm::vec3& dimensions) {
    m_dimensions = dimensions;
    m_meshGenerated = false; // Regenerate mesh
}

const std::vector<float>& Box::getVertices() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_vertices;
}

const std::vector<float>& Box::getNormals() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_normals;
}

const std::vector<unsigned int>& Box::getIndices() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_indices;
}

void Box::generateMesh() const {
    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();
    
    glm::vec3 scaled = m_dimensions * m_scale;
    float w = scaled.x * 0.5f, h = scaled.y * 0.5f, d = scaled.z * 0.5f;
    
    // 8 vertices of the box
    std::vector<glm::vec3> positions = {
        {-w, -h, -d}, {w, -h, -d}, {w, h, -d}, {-w, h, -d}, // Back face
        {-w, -h,  d}, {w, -h,  d}, {w, h,  d}, {-w, h,  d}  // Front face
    };
    
    // 6 face normals
    std::vector<glm::vec3> normals = {
        {0, 0, -1}, {0, 0, 1},   // Back, Front
        {-1, 0, 0}, {1, 0, 0},   // Left, Right
        {0, -1, 0}, {0, 1, 0}    // Bottom, Top
    };
    
    // Face indices (each face has 4 vertices)
    std::vector<std::vector<int>> faces = {
        {0, 1, 2, 3}, // Back
        {4, 7, 6, 5}, // Front
        {0, 4, 5, 1}, // Bottom
        {2, 6, 7, 3}, // Top
        {0, 3, 7, 4}, // Left
        {1, 5, 6, 2}  // Right
    };
    
    // Generate vertices and normals
    for (int face = 0; face < 6; ++face) {
        for (int vertex = 0; vertex < 4; ++vertex) {
            int posIndex = faces[face][vertex];
            
            // Add position
            m_vertices.push_back(positions[posIndex].x);
            m_vertices.push_back(positions[posIndex].y);
            m_vertices.push_back(positions[posIndex].z);
            
            // Add normal
            m_normals.push_back(normals[face].x);
            m_normals.push_back(normals[face].y);
            m_normals.push_back(normals[face].z);
        }
    }
    
    // Generate indices (2 triangles per face)
    for (int face = 0; face < 6; ++face) {
        int baseIndex = face * 4;
        
        // First triangle
        m_indices.push_back(baseIndex);
        m_indices.push_back(baseIndex + 1);
        m_indices.push_back(baseIndex + 2);
        
        // Second triangle
        m_indices.push_back(baseIndex);
        m_indices.push_back(baseIndex + 2);
        m_indices.push_back(baseIndex + 3);
    }
    
    m_meshGenerated = true;
}
