#pragma once

#include "../core/BaseShape.h"
#include <glm/glm.hpp>

class Box : public BaseShape {
public:
    // Constructor: width, height, depth in meters
    Box(float width = 1.0f, float height = 1.0f, float depth = 1.0f);
    virtual ~Box() = default;
    
    // BaseShape interface
    float getVolume() const override;
    glm::mat3 getInertiaTensor(float mass) const override;
    glm::vec3 getBoundingBoxMin() const override;
    glm::vec3 getBoundingBoxMax() const override;
    
    const std::vector<float>& getVertices() const override;
    const std::vector<float>& getNormals() const override;
    const std::vector<unsigned int>& getIndices() const override;
    
    const char* getTypeName() const override { return "Box"; }
    glm::vec3 getCenter() const override;
    bool containsPoint(const glm::vec3& point) const override;
    
    // Box-specific
    glm::vec3 getDimensions() const { return m_dimensions; }
    void setDimensions(const glm::vec3& dimensions);
    
private:
    glm::vec3 m_dimensions; // width, height, depth in meters
    
    // Cached mesh data
    mutable std::vector<float> m_vertices;
    mutable std::vector<float> m_normals;
    mutable std::vector<unsigned int> m_indices;
    mutable bool m_meshGenerated = false;
    
    void generateMesh() const;
};
