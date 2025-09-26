#include "RigidBody3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <algorithm>

// Constructor with shape and mass
RigidBody3D::RigidBody3D(std::unique_ptr<BaseShape> shape, float mass) 
    : m_shape(std::move(shape)), m_mass(mass) {
    
    // Initialize position and orientation
    m_position = glm::vec3(0.0f);
    m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
    m_linearVelocity = glm::vec3(0.0f);
    m_angularVelocity = glm::vec3(0.0f);
    
    // Initialize forces
    m_force = glm::vec3(0.0f);
    m_torque = glm::vec3(0.0f);
    
    // Update mass and inertia
    updateInverseMass();
    updateInertiaTensor();
}

void RigidBody3D::setMass(float mass) {
    m_mass = mass;
    updateInverseMass();
    updateInertiaTensor();
}

void RigidBody3D::setStatic(bool isStatic) {
    m_isStatic = isStatic;
    if (isStatic) {
        m_mass = 0.0f;
        m_linearVelocity = glm::vec3(0.0f);
        m_angularVelocity = glm::vec3(0.0f);
    }
    updateInverseMass();
    updateInertiaTensor();
}

void RigidBody3D::setScale(const glm::vec3& scale) {
    m_scale = scale;
    if (m_shape) {
        m_shape->setScale(scale);
    }
    updateInertiaTensor();
}

void RigidBody3D::setDensity(float density) {
    m_density = density;
    if (m_shape) {
        m_mass = m_shape->getVolume() * density;
        updateInverseMass();
        updateInertiaTensor();
    }
}

void RigidBody3D::addForce(const glm::vec3& force) {
    if (!m_isStatic) {
        m_force += force;
        wakeUp();
    }
}

void RigidBody3D::addForceAtPoint(const glm::vec3& force, const glm::vec3& point) {
    if (!m_isStatic) {
        m_force += force;
        m_torque += glm::cross(point - m_position, force);
        wakeUp();
    }
}

void RigidBody3D::addTorque(const glm::vec3& torque) {
    if (!m_isStatic) {
        m_torque += torque;
        wakeUp();
    }
}

void RigidBody3D::clearAccumulators() {
    m_force = glm::vec3(0.0f);
    m_torque = glm::vec3(0.0f);
}

void RigidBody3D::integrate(float dt) {
    if (m_isStatic || m_sleeping) return;

    // Apply damping
    m_linearVelocity *= m_linearDamping;
    m_angularVelocity *= m_angularDamping;

    // --- Linear Motion ---
    glm::vec3 linearAcceleration = m_force * m_inverseMass;
    m_linearVelocity += linearAcceleration * dt;
    m_position += m_linearVelocity * dt;

    // --- Angular Motion ---
    glm::vec3 angularAcceleration = m_inverseInertiaTensor * m_torque;
    m_angularVelocity += angularAcceleration * dt;
    
    // Update rotation quaternion
    glm::quat deltaRotation = glm::quat(0.0f, m_angularVelocity.x, m_angularVelocity.y, m_angularVelocity.z) * m_rotation;
    m_rotation += deltaRotation * (dt * 0.5f);
    m_rotation = glm::normalize(m_rotation);

    // Clear forces for the next frame
    clearAccumulators();
    
    // Check for sleeping conditions
    float velocityThreshold = 0.01f;
    float angularVelocityThreshold = 0.01f;
    if (glm::length(m_linearVelocity) < velocityThreshold && 
        glm::length(m_angularVelocity) < angularVelocityThreshold) {
        // Could implement sleeping logic here
    }
}

bool RigidBody3D::checkGroundCollision(float groundY) {
    if (!m_shape) return false;
    
    glm::vec3 bboxMin = m_shape->getBoundingBoxMin();
    glm::vec3 bboxMax = m_shape->getBoundingBoxMax();
    
    // Transform bounding box to world space
    glm::mat4 transform = getTransformMatrix();
    glm::vec3 worldMin = transform * glm::vec4(bboxMin, 1.0f);
    glm::vec3 worldMax = transform * glm::vec4(bboxMax, 1.0f);
    
    return worldMin.y <= groundY;
}

void RigidBody3D::resolveGroundCollision(float groundY) {
    if (m_isStatic) return;
    
    if (!m_shape) return;
    
    glm::vec3 bboxMin = m_shape->getBoundingBoxMin();
    glm::vec3 bboxMax = m_shape->getBoundingBoxMax();
    
    // Transform bounding box to world space
    glm::mat4 transform = getTransformMatrix();
    glm::vec3 worldMin = transform * glm::vec4(bboxMin, 1.0f);
    glm::vec3 worldMax = transform * glm::vec4(bboxMax, 1.0f);
    
    // Position correction
    if (worldMin.y < groundY) {
        float penetration = groundY - worldMin.y;
        m_position.y += penetration;
    }
    
    // Velocity reflection
    if (m_linearVelocity.y < 0.0f) {
        m_linearVelocity.y = -m_linearVelocity.y * m_restitution;
    }
    
    // Apply friction
    m_linearVelocity.x *= (1.0f - m_friction);
    m_linearVelocity.z *= (1.0f - m_friction);
    
    wakeUp();
}

glm::mat4 RigidBody3D::getTransformMatrix() const {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 rotation = glm::mat4_cast(m_rotation);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
    
    return translation * rotation * scale;
}

glm::vec3 RigidBody3D::getCenterOfMass() const {
    return m_position; // Assuming center of mass is at position
}

void RigidBody3D::wakeUp() {
    m_sleeping = false;
}

void RigidBody3D::putToSleep() {
    m_sleeping = true;
    m_linearVelocity = glm::vec3(0.0f);
    m_angularVelocity = glm::vec3(0.0f);
    clearAccumulators();
}

void RigidBody3D::updateInertiaTensor() {
    if (!m_shape || m_isStatic) {
        m_inertiaTensor = glm::mat3(0.0f);
        m_inverseInertiaTensor = glm::mat3(0.0f);
        return;
    }
    
    m_inertiaTensor = m_shape->getInertiaTensor(m_mass);
    m_inverseInertiaTensor = glm::inverse(m_inertiaTensor);
}

void RigidBody3D::updateInverseMass() {
    if (m_mass <= 0.0f || m_isStatic) {
        m_inverseMass = 0.0f;
    } else {
        m_inverseMass = 1.0f / m_mass;
    }
}
