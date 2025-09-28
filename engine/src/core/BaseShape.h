#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

class BaseShape {
public:
    virtual ~BaseShape() = default;
    
    // Geometric properties
    virtual float getVolume() const = 0;
    virtual glm::mat3 getInertiaTensor(float mass) const = 0;
    virtual glm::vec3 getBoundingBoxMin() const = 0;
    virtual glm::vec3 getBoundingBoxMax() const = 0;
    
    // Rendering data
    virtual const std::vector<float>& getVertices() const = 0;
    virtual const std::vector<float>& getNormals() const = 0;
    virtual const std::vector<unsigned int>& getIndices() const = 0;
    
    // Shape type
    virtual const char* getTypeName() const = 0;
    
    // Utility
    virtual glm::vec3 getCenter() const = 0;
    virtual bool containsPoint(const glm::vec3& point) const = 0;
    
    // Scale support
    virtual void setScale(const glm::vec3& scale) { m_scale = scale; }
    virtual glm::vec3 getScale() const { return m_scale; }
    
protected:
    glm::vec3 m_scale = glm::vec3(1.0f);
};
