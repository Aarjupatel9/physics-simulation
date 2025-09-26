#pragma once

#include "../core/BaseShape.h"
#include <glm/glm.hpp>

class Sphere : public BaseShape {
public:
    // Constructor: radius in meters, segments for mesh resolution
    Sphere(float radius = 0.5f, int segments = 32);
    virtual ~Sphere() = default;
    
    // BaseShape interface
    float getVolume() const override;
    glm::mat3 getInertiaTensor(float mass) const override;
    glm::vec3 getBoundingBoxMin() const override;
    glm::vec3 getBoundingBoxMax() const override;
    
    const std::vector<float>& getVertices() const override;
    const std::vector<float>& getNormals() const override;
    const std::vector<unsigned int>& getIndices() const override;
    
    const char* getTypeName() const override { return "Sphere"; }
    glm::vec3 getCenter() const override;
    bool containsPoint(const glm::vec3& point) const override;
    
    // Sphere-specific
    float getRadius() const { return m_radius; }
    void setRadius(float radius);
    int getSegments() const { return m_segments; }
    void setSegments(int segments);
    
private:
    float m_radius; // radius in meters
    int m_segments; // mesh resolution
    
    // Cached mesh data
    mutable std::vector<float> m_vertices;
    mutable std::vector<float> m_normals;
    mutable std::vector<unsigned int> m_indices;
    mutable bool m_meshGenerated = false;
    
    void generateMesh() const;
};
