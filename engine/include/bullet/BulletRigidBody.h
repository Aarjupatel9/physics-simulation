#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <memory>

/**
 * BulletRigidBody - Wrapper class for Bullet Physics rigid body
 * 
 * This class provides a clean interface to Bullet Physics' btRigidBody
 * and handles all the necessary Bullet Physics components for a single rigid body.
 * 
 * Features:
 * - Position and rotation management
 * - Linear and angular velocity
 * - Force and torque application
 * - Mass and inertia properties
 * - Static/dynamic object handling
 * - Continuous collision detection (CCD)
 */
class BulletRigidBody {
private:
    btRigidBody* m_rigidBody;
    btCollisionShape* m_collisionShape;
    btMotionState* m_motionState;
    btTransform m_transform;
    
    // Properties
    float m_mass;
    bool m_isStatic;
    
public:
    /**
     * Constructor
     * @param shape Collision shape for the rigid body
     * @param mass Mass of the rigid body (0 for static objects)
     * @param position Initial position
     * @param rotation Initial rotation (Euler angles in degrees)
     */
    BulletRigidBody(btCollisionShape* shape, float mass, 
                    const glm::vec3& position = glm::vec3(0.0f),
                    const glm::vec3& rotation = glm::vec3(0.0f));
    
    /**
     * Destructor
     */
    ~BulletRigidBody();
    
    // Disable copy constructor and assignment operator
    BulletRigidBody(const BulletRigidBody&) = delete;
    BulletRigidBody& operator=(const BulletRigidBody&) = delete;
    
    // Position and rotation
    /**
     * Get the current position
     * @return Current position vector
     */
    glm::vec3 getPosition() const;
    
    /**
     * Set the position
     * @param position New position vector
     */
    void setPosition(const glm::vec3& position);
    
    /**
     * Get the current rotation (Euler angles in degrees)
     * @return Current rotation vector
     */
    glm::vec3 getRotation() const;
    
    /**
     * Set the rotation (Euler angles in degrees)
     * @param rotation New rotation vector
     */
    void setRotation(const glm::vec3& rotation);
    
    /**
     * Get the current transform matrix
     * @return Current transform matrix
     */
    glm::mat4 getTransform() const;
    
    /**
     * Set the transform matrix
     * @param transform New transform matrix
     */
    void setTransform(const glm::mat4& transform);
    
    // Velocity
    /**
     * Get the linear velocity
     * @return Current linear velocity vector
     */
    glm::vec3 getLinearVelocity() const;
    
    /**
     * Set the linear velocity
     * @param velocity New linear velocity vector
     */
    void setLinearVelocity(const glm::vec3& velocity);
    
    /**
     * Get the angular velocity
     * @return Current angular velocity vector
     */
    glm::vec3 getAngularVelocity() const;
    
    /**
     * Set the angular velocity
     * @param velocity New angular velocity vector
     */
    void setAngularVelocity(const glm::vec3& velocity);
    
    // Forces
    /**
     * Add a force to the rigid body
     * @param force Force vector to apply
     */
    void addForce(const glm::vec3& force);
    
    /**
     * Add a force at a specific point
     * @param force Force vector to apply
     * @param point Point where force is applied
     */
    void addForceAtPoint(const glm::vec3& force, const glm::vec3& point);
    
    /**
     * Add a torque to the rigid body
     * @param torque Torque vector to apply
     */
    void addTorque(const glm::vec3& torque);
    
    /**
     * Apply an impulse to the rigid body
     * @param impulse Impulse vector to apply
     */
    void applyImpulse(const glm::vec3& impulse);
    
    /**
     * Apply an impulse at a specific point
     * @param impulse Impulse vector to apply
     * @param point Point where impulse is applied
     */
    void applyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& point);
    
    // Properties
    /**
     * Get the mass of the rigid body
     * @return Mass value
     */
    float getMass() const;
    
    /**
     * Set the mass of the rigid body
     * @param mass New mass value (0 for static objects)
     */
    void setMass(float mass);
    
    /**
     * Check if the rigid body is static
     * @return True if static, false if dynamic
     */
    bool isStatic() const;
    
    /**
     * Set whether the rigid body is static
     * @param isStatic True for static, false for dynamic
     */
    void setStatic(bool isStatic);
    
    /**
     * Check if gravity is enabled
     * @return True if gravity is enabled
     */
    bool isGravityEnabled() const;
    
    /**
     * Enable or disable gravity
     * @param enabled True to enable gravity
     */
    void setGravityEnabled(bool enabled);
    
    // Continuous Collision Detection
    /**
     * Enable continuous collision detection
     * @param motionThreshold Motion threshold for CCD
     * @param sweptSphereRadius Radius of swept sphere for CCD
     */
    void enableCCD(float motionThreshold = 0.1f, float sweptSphereRadius = 0.5f);
    
    /**
     * Disable continuous collision detection
     */
    void disableCCD();
    
    /**
     * Check if CCD is enabled
     * @return True if CCD is enabled
     */
    bool isCCDEnabled() const;
    
    // Bullet-specific access
    /**
     * Get the Bullet rigid body (for advanced usage)
     * @return Pointer to btRigidBody
     */
    btRigidBody* getBulletRigidBody() const { return m_rigidBody; }
    
    /**
     * Get the collision shape (for advanced usage)
     * @return Pointer to btCollisionShape
     */
    btCollisionShape* getCollisionShape() const { return m_collisionShape; }
    
    /**
     * Get the motion state (for advanced usage)
     * @return Pointer to btMotionState
     */
    btMotionState* getMotionState() const { return m_motionState; }
    
private:
    /**
     * Convert GLM vector to Bullet vector
     * @param glmVec GLM vector
     * @return Bullet vector
     */
    btVector3 glmToBullet(const glm::vec3& glmVec) const;
    
    /**
     * Convert Bullet vector to GLM vector
     * @param bulletVec Bullet vector
     * @return GLM vector
     */
    glm::vec3 bulletToGlm(const btVector3& bulletVec) const;
    
    /**
     * Convert GLM quaternion to Bullet quaternion
     * @param glmQuat GLM quaternion
     * @return Bullet quaternion
     */
    btQuaternion glmToBullet(const glm::quat& glmQuat) const;
    
    /**
     * Convert Bullet quaternion to GLM quaternion
     * @param bulletQuat Bullet quaternion
     * @return GLM quaternion
     */
    glm::quat bulletToGlm(const btQuaternion& bulletQuat) const;
    
    /**
     * Convert GLM matrix to Bullet transform
     * @param glmMat GLM matrix
     * @return Bullet transform
     */
    btTransform glmToBullet(const glm::mat4& glmMat) const;
    
    /**
     * Convert Bullet transform to GLM matrix
     * @param bulletTransform Bullet transform
     * @return GLM matrix
     */
    glm::mat4 bulletToGlm(const btTransform& bulletTransform) const;
};
