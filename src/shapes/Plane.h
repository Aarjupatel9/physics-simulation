#pragma once

#include "../core/BaseShape.h"
#include <glm/glm.hpp>

class Plane : public BaseShape {
public:
    // Constructor: width and depth in meters
    Plane(float width = 10.0f, float depth = 10.0f);
    virtual ~Plane() = default;
    
    // BaseShape interface
    float getVolume() const override { return 0.0f; } // Planes have no volume
    glm::mat3 getInertiaTensor(float mass) const override;
    glm::vec3 getBoundingBoxMin() const override;
    glm::vec3 getBoundingBoxMax() const override;
    
    const std::vector<float>& getVertices() const override;
    const std::vector<float>& getNormals() const override;
    const std::vector<unsigned int>& getIndices() const override;
    
    const char* getTypeName() const override { return "Plane"; }
    glm::vec3 getCenter() const override;
    bool containsPoint(const glm::vec3& point) const override;
    
    // Plane-specific
    glm::vec2 getDimensions() const { return m_dimensions; }
    void setDimensions(const glm::vec2& dimensions);
    glm::vec3 getNormal() const { return m_normal; }
    void setNormal(const glm::vec3& normal);
    
private:
    glm::vec2 m_dimensions; // width, depth in meters
    glm::vec3 m_normal; // plane normal (default: up)
    
    // Cached mesh data
    mutable std::vector<float> m_vertices;
    mutable std::vector<float> m_normals;
    mutable std::vector<unsigned int> m_indices;
    mutable bool m_meshGenerated = false;
    
    void generateMesh() const;
};
