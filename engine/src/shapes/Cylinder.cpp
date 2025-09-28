#include "Cylinder.h"
#include "../core/PhysicsConstants.h"
#include <cmath>

Cylinder::Cylinder(float radius, float height, int segments) 
    : m_radius(radius), m_height(height), m_segments(segments) {
}

float Cylinder::getVolume() const {
    float scaledRadius = m_radius * std::max(m_scale.x, m_scale.z);
    float scaledHeight = m_height * m_scale.y;
    return M_PI * scaledRadius * scaledRadius * scaledHeight;
}

glm::mat3 Cylinder::getInertiaTensor(float mass) const {
    float scaledRadius = m_radius * std::max(m_scale.x, m_scale.z);
    float scaledHeight = m_height * m_scale.y;
    
    // Inertia tensor for cylinder aligned with Y-axis
    float Ixx = mass * (3.0f * scaledRadius * scaledRadius + scaledHeight * scaledHeight) / 12.0f;
    float Iyy = mass * scaledRadius * scaledRadius / 2.0f;
    float Izz = Ixx; // Same as Ixx for circular cross-section
    
    return glm::mat3(
        Ixx, 0.0f, 0.0f,
        0.0f, Iyy, 0.0f,
        0.0f, 0.0f, Izz
    );
}

glm::vec3 Cylinder::getBoundingBoxMin() const {
    float scaledRadius = m_radius * std::max(m_scale.x, m_scale.z);
    float scaledHeight = m_height * m_scale.y;
    return glm::vec3(-scaledRadius, -scaledHeight * 0.5f, -scaledRadius);
}

glm::vec3 Cylinder::getBoundingBoxMax() const {
    float scaledRadius = m_radius * std::max(m_scale.x, m_scale.z);
    float scaledHeight = m_height * m_scale.y;
    return glm::vec3(scaledRadius, scaledHeight * 0.5f, scaledRadius);
}

glm::vec3 Cylinder::getCenter() const {
    return glm::vec3(0.0f);
}

bool Cylinder::containsPoint(const glm::vec3& point) const {
    float scaledRadius = m_radius * std::max(m_scale.x, m_scale.z);
    float scaledHeight = m_height * m_scale.y;
    
    // Check if point is within cylinder bounds
    float distanceFromAxis = std::sqrt(point.x * point.x + point.z * point.z);
    return (distanceFromAxis <= scaledRadius && 
            std::abs(point.y) <= scaledHeight * 0.5f);
}

void Cylinder::setRadius(float radius) {
    m_radius = radius;
    m_meshGenerated = false; // Regenerate mesh
}

void Cylinder::setHeight(float height) {
    m_height = height;
    m_meshGenerated = false; // Regenerate mesh
}

void Cylinder::setSegments(int segments) {
    m_segments = segments;
    m_meshGenerated = false; // Regenerate mesh
}

const std::vector<float>& Cylinder::getVertices() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_vertices;
}

const std::vector<float>& Cylinder::getNormals() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_normals;
}

const std::vector<unsigned int>& Cylinder::getIndices() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_indices;
}

void Cylinder::generateMesh() const {
    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();
    
    float scaledRadius = m_radius * std::max(m_scale.x, m_scale.z);
    float scaledHeight = m_height * m_scale.y;
    float halfHeight = scaledHeight * 0.5f;
    
    // Generate side vertices
    for (int i = 0; i <= m_segments; ++i) {
        float angle = 2.0f * M_PI * i / m_segments;
        float x = std::cos(angle) * scaledRadius;
        float z = std::sin(angle) * scaledRadius;
        
        // Bottom vertex
        m_vertices.push_back(x);
        m_vertices.push_back(-halfHeight);
        m_vertices.push_back(z);
        
        // Top vertex
        m_vertices.push_back(x);
        m_vertices.push_back(halfHeight);
        m_vertices.push_back(z);
        
        // Side normals (pointing outward)
        glm::vec3 normal = glm::normalize(glm::vec3(x, 0.0f, z));
        m_normals.push_back(normal.x);
        m_normals.push_back(normal.y);
        m_normals.push_back(normal.z);
        m_normals.push_back(normal.x);
        m_normals.push_back(normal.y);
        m_normals.push_back(normal.z);
    }
    
    // Generate side indices
    for (int i = 0; i < m_segments; ++i) {
        int current = i * 2;
        int next = (i + 1) * 2;
        
        // First triangle
        m_indices.push_back(current);
        m_indices.push_back(next);
        m_indices.push_back(current + 1);
        
        // Second triangle
        m_indices.push_back(current + 1);
        m_indices.push_back(next);
        m_indices.push_back(next + 1);
    }
    
    // Generate top and bottom caps
    int capStartIndex = m_vertices.size() / 3;
    
    // Center vertices for caps
    m_vertices.push_back(0.0f); m_vertices.push_back(-halfHeight); m_vertices.push_back(0.0f); // Bottom center
    m_vertices.push_back(0.0f); m_vertices.push_back(halfHeight); m_vertices.push_back(0.0f);  // Top center
    
    m_normals.push_back(0.0f); m_normals.push_back(-1.0f); m_normals.push_back(0.0f); // Bottom normal
    m_normals.push_back(0.0f); m_normals.push_back(1.0f); m_normals.push_back(0.0f);  // Top normal
    
    // Cap vertices
    for (int i = 0; i <= m_segments; ++i) {
        float angle = 2.0f * M_PI * i / m_segments;
        float x = std::cos(angle) * scaledRadius;
        float z = std::sin(angle) * scaledRadius;
        
        // Bottom cap
        m_vertices.push_back(x);
        m_vertices.push_back(-halfHeight);
        m_vertices.push_back(z);
        m_normals.push_back(0.0f);
        m_normals.push_back(-1.0f);
        m_normals.push_back(0.0f);
        
        // Top cap
        m_vertices.push_back(x);
        m_vertices.push_back(halfHeight);
        m_vertices.push_back(z);
        m_normals.push_back(0.0f);
        m_normals.push_back(1.0f);
        m_normals.push_back(0.0f);
    }
    
    // Generate cap indices
    for (int i = 0; i < m_segments; ++i) {
        int bottomCenter = capStartIndex;
        int topCenter = capStartIndex + 1;
        int current = capStartIndex + 2 + i * 2;
        int next = capStartIndex + 2 + ((i + 1) % m_segments) * 2;
        
        // Bottom cap triangle
        m_indices.push_back(bottomCenter);
        m_indices.push_back(current);
        m_indices.push_back(next);
        
        // Top cap triangle
        m_indices.push_back(topCenter);
        m_indices.push_back(next + 1);
        m_indices.push_back(current + 1);
    }
    
    m_meshGenerated = true;
}
