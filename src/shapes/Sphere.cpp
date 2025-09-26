#include "Sphere.h"
#include "../core/PhysicsConstants.h"
#include "../core/InertiaTensorCache.h"
#include <cmath>

Sphere::Sphere(float radius, int segments) 
    : m_radius(radius), m_segments(segments) {
}

float Sphere::getVolume() const {
    float scaledRadius = m_radius * std::max({m_scale.x, m_scale.y, m_scale.z});
    return (4.0f / 3.0f) * M_PI * scaledRadius * scaledRadius * scaledRadius;
}

glm::mat3 Sphere::getInertiaTensor(float mass) const {
    float scaledRadius = m_radius * std::max({m_scale.x, m_scale.y, m_scale.z});
    
    // Generate cache key
    std::string key = InertiaTensorCache::generateSphereKey(scaledRadius, mass);
    
    // Check cache first
    auto& cache = InertiaTensorCache::getInstance();
    glm::mat3 cached = cache.getInertiaTensor(key);
    
    // If not identity matrix, return cached result
    if (cached != glm::mat3(1.0f)) {
        return cached;
    }
    
    // Calculate inertia tensor for a sphere
    float I = (2.0f / 5.0f) * mass * scaledRadius * scaledRadius;
    
    glm::mat3 tensor(
        I, 0.0f, 0.0f,
        0.0f, I, 0.0f,
        0.0f, 0.0f, I
    );
    
    // Cache the result
    cache.cacheInertiaTensor(key, tensor);
    
    return tensor;
}

glm::vec3 Sphere::getBoundingBoxMin() const {
    float scaledRadius = m_radius * std::max({m_scale.x, m_scale.y, m_scale.z});
    return glm::vec3(-scaledRadius);
}

glm::vec3 Sphere::getBoundingBoxMax() const {
    float scaledRadius = m_radius * std::max({m_scale.x, m_scale.y, m_scale.z});
    return glm::vec3(scaledRadius);
}

glm::vec3 Sphere::getCenter() const {
    return glm::vec3(0.0f);
}

bool Sphere::containsPoint(const glm::vec3& point) const {
    float scaledRadius = m_radius * std::max({m_scale.x, m_scale.y, m_scale.z});
    return glm::length(point) <= scaledRadius;
}

void Sphere::setRadius(float radius) {
    m_radius = radius;
    m_meshGenerated = false; // Regenerate mesh
}

void Sphere::setSegments(int segments) {
    m_segments = segments;
    m_meshGenerated = false; // Regenerate mesh
}

const std::vector<float>& Sphere::getVertices() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_vertices;
}

const std::vector<float>& Sphere::getNormals() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_normals;
}

const std::vector<unsigned int>& Sphere::getIndices() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_indices;
}

void Sphere::generateMesh() const {
    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();
    
    float scaledRadius = m_radius * std::max({m_scale.x, m_scale.y, m_scale.z});
    
    // Generate vertices using UV sphere method
    for (int y = 0; y <= m_segments; ++y) {
        float yAngle = M_PI * y / m_segments;
        float yPos = std::cos(yAngle) * scaledRadius;
        float yRadius = std::sin(yAngle) * scaledRadius;
        
        for (int x = 0; x <= m_segments; ++x) {
            float xAngle = 2.0f * M_PI * x / m_segments;
            float xPos = std::cos(xAngle) * yRadius;
            float zPos = std::sin(xAngle) * yRadius;
            
            // Position
            m_vertices.push_back(xPos);
            m_vertices.push_back(yPos);
            m_vertices.push_back(zPos);
            
            // Normal (same as position normalized)
            glm::vec3 normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
            m_normals.push_back(normal.x);
            m_normals.push_back(normal.y);
            m_normals.push_back(normal.z);
        }
    }
    
    // Generate indices
    for (int y = 0; y < m_segments; ++y) {
        for (int x = 0; x < m_segments; ++x) {
            int current = y * (m_segments + 1) + x;
            int next = current + m_segments + 1;
            
            // First triangle
            m_indices.push_back(current);
            m_indices.push_back(next);
            m_indices.push_back(current + 1);
            
            // Second triangle
            m_indices.push_back(current + 1);
            m_indices.push_back(next);
            m_indices.push_back(next + 1);
        }
    }
    
    m_meshGenerated = true;
}
