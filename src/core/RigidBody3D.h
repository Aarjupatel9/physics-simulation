#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include "BaseShape.h"
#include "PhysicsConstants.h"

class RigidBody3D {
public:
    // Constructor with shape and mass
    RigidBody3D(std::unique_ptr<BaseShape> shape, float mass = Physics::DEFAULT_MASS);
    virtual ~RigidBody3D() = default;

    // Geometric properties
    std::unique_ptr<BaseShape> m_shape;
    glm::vec3 m_scale = glm::vec3(1.0f);
    
    // Kinematic properties
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_linearVelocity;
    glm::vec3 m_angularVelocity;
    
    // Dynamic properties
    float m_mass;
    float m_inverseMass;
    glm::mat3 m_inertiaTensor;
    glm::mat3 m_inverseInertiaTensor;
    
    // Material properties
    float m_density = Physics::DEFAULT_DENSITY;
    float m_friction = Physics::DEFAULT_FRICTION;
    float m_restitution = Physics::DEFAULT_RESTITUTION;
    float m_linearDamping = Physics::DEFAULT_LINEAR_DAMPING;
    float m_angularDamping = Physics::DEFAULT_ANGULAR_DAMPING;
    
    // State properties
    bool m_isStatic = false;
    bool m_gravityEnabled = true;
    bool m_sleeping = false;
    
    // Force accumulators
    glm::vec3 m_force;
    glm::vec3 m_torque;
    
    // Getters
    const BaseShape* getShape() const { return m_shape.get(); }
    glm::vec3 getPosition() const { return m_position; }
    glm::quat getRotation() const { return m_rotation; }
    glm::vec3 getLinearVelocity() const { return m_linearVelocity; }
    glm::vec3 getAngularVelocity() const { return m_angularVelocity; }
    float getMass() const { return m_mass; }
    float getInverseMass() const { return m_inverseMass; }
    bool isStatic() const { return m_isStatic; }
    bool isGravityEnabled() const { return m_gravityEnabled; }
    bool isSleeping() const { return m_sleeping; }
    
    // Setters
    void setPosition(const glm::vec3& position) { m_position = position; }
    void setRotation(const glm::quat& rotation) { m_rotation = rotation; }
    void setLinearVelocity(const glm::vec3& velocity) { m_linearVelocity = velocity; }
    void setAngularVelocity(const glm::vec3& velocity) { m_angularVelocity = velocity; }
    void setMass(float mass);
    void setStatic(bool isStatic);
    void setGravityEnabled(bool enabled) { m_gravityEnabled = enabled; }
    void setScale(const glm::vec3& scale);
    
    // Material property setters
    void setDensity(float density);
    void setFriction(float friction) { m_friction = friction; }
    void setRestitution(float restitution) { m_restitution = restitution; }
    void setLinearDamping(float damping) { m_linearDamping = damping; }
    void setAngularDamping(float damping) { m_angularDamping = damping; }
    
    // Physics methods
    void addForce(const glm::vec3& force);
    void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);
    void addTorque(const glm::vec3& torque);
    void clearAccumulators();
    void integrate(float dt);
    
    // Collision detection
    bool checkGroundCollision(float groundY);
    void resolveGroundCollision(float groundY);
    
    // Utility methods
    glm::mat4 getTransformMatrix() const;
    glm::vec3 getCenterOfMass() const;
    void wakeUp();
    void putToSleep();
    
private:
    void updateInertiaTensor();
    void updateInverseMass();
};
