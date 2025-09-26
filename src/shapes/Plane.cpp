#include "Plane.h"
#include "../core/PhysicsConstants.h"

Plane::Plane(float width, float depth) 
    : m_dimensions(glm::vec2(width, depth)), m_normal(glm::vec3(0.0f, 1.0f, 0.0f)) {
}

glm::mat3 Plane::getInertiaTensor(float mass) const {
    // For an infinite plane, we use a very large inertia tensor
    // In practice, planes are usually static objects
    float largeValue = 1e6f * mass;
    return glm::mat3(
        largeValue, 0.0f, 0.0f,
        0.0f, largeValue, 0.0f,
        0.0f, 0.0f, largeValue
    );
}

glm::vec3 Plane::getBoundingBoxMin() const {
    glm::vec2 scaled = m_dimensions * glm::vec2(m_scale.x, m_scale.z);
    return glm::vec3(-scaled.x * 0.5f, -0.01f, -scaled.y * 0.5f);
}

glm::vec3 Plane::getBoundingBoxMax() const {
    glm::vec2 scaled = m_dimensions * glm::vec2(m_scale.x, m_scale.z);
    return glm::vec3(scaled.x * 0.5f, 0.01f, scaled.y * 0.5f);
}

glm::vec3 Plane::getCenter() const {
    return glm::vec3(0.0f);
}

bool Plane::containsPoint(const glm::vec3& point) const {
    glm::vec2 scaled = m_dimensions * glm::vec2(m_scale.x, m_scale.z);
    glm::vec2 half = scaled * 0.5f;
    
    // Check if point is within plane bounds (assuming horizontal plane)
    return (point.x >= -half.x && point.x <= half.x &&
            point.z >= -half.y && point.z <= half.y &&
            std::abs(point.y) <= 0.01f); // Small tolerance for Y
}

void Plane::setDimensions(const glm::vec2& dimensions) {
    m_dimensions = dimensions;
    m_meshGenerated = false; // Regenerate mesh
}

void Plane::setNormal(const glm::vec3& normal) {
    m_normal = glm::normalize(normal);
    m_meshGenerated = false; // Regenerate mesh
}

const std::vector<float>& Plane::getVertices() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_vertices;
}

const std::vector<float>& Plane::getNormals() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_normals;
}

const std::vector<unsigned int>& Plane::getIndices() const {
    if (!m_meshGenerated) {
        generateMesh();
    }
    return m_indices;
}

void Plane::generateMesh() const {
    m_vertices.clear();
    m_normals.clear();
    m_indices.clear();
    
    glm::vec2 scaled = m_dimensions * glm::vec2(m_scale.x, m_scale.z);
    float w = scaled.x * 0.5f, d = scaled.y * 0.5f;
    
    // Generate 4 vertices for the plane
    std::vector<glm::vec3> positions = {
        {-w, 0.0f, -d}, // Bottom-left
        { w, 0.0f, -d}, // Bottom-right
        { w, 0.0f,  d}, // Top-right
        {-w, 0.0f,  d}  // Top-left
    };
    
    // Add vertices
    for (const auto& pos : positions) {
        m_vertices.push_back(pos.x);
        m_vertices.push_back(pos.y);
        m_vertices.push_back(pos.z);
    }
    
    // Add normals (all the same for a flat plane)
    for (int i = 0; i < 4; ++i) {
        m_normals.push_back(m_normal.x);
        m_normals.push_back(m_normal.y);
        m_normals.push_back(m_normal.z);
    }
    
    // Generate indices (2 triangles)
    m_indices = {
        0, 1, 2, // First triangle
        0, 2, 3  // Second triangle
    };
    
    m_meshGenerated = true;
}
