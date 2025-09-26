#pragma once

#include "../core/BaseShape.h"
#include <glm/glm.hpp>

class Cylinder : public BaseShape {
public:
    // Constructor: radius and height in meters, segments for mesh resolution
    Cylinder(float radius = 0.5f, float height = 1.0f, int segments = 16);
    virtual ~Cylinder() = default;
    
    // BaseShape interface
    float getVolume() const override;
    glm::mat3 getInertiaTensor(float mass) const override;
    glm::vec3 getBoundingBoxMin() const override;
    glm::vec3 getBoundingBoxMax() const override;
    
    const std::vector<float>& getVertices() const override;
    const std::vector<float>& getNormals() const override;
    const std::vector<unsigned int>& getIndices() const override;
    
    const char* getTypeName() const override { return "Cylinder"; }
    glm::vec3 getCenter() const override;
    bool containsPoint(const glm::vec3& point) const override;
    
    // Cylinder-specific
    float getRadius() const { return m_radius; }
    float getHeight() const { return m_height; }
    void setRadius(float radius);
    void setHeight(float height);
    int getSegments() const { return m_segments; }
    void setSegments(int segments);
    
private:
    float m_radius; // radius in meters
    float m_height; // height in meters
    int m_segments; // mesh resolution
    
    // Cached mesh data
    mutable std::vector<float> m_vertices;
    mutable std::vector<float> m_normals;
    mutable std::vector<unsigned int> m_indices;
    mutable bool m_meshGenerated = false;
    
    void generateMesh() const;
};
