#include "bullet/BulletRigidBody.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

BulletRigidBody::BulletRigidBody(btCollisionShape* shape, float mass, 
                                const glm::vec3& position, const glm::vec3& rotation)
    : m_rigidBody(nullptr)
    , m_collisionShape(shape)
    , m_motionState(nullptr)
    , m_mass(mass)
    , m_isStatic(mass == 0.0f)
{
    if (!shape) {
        std::cerr << "BulletRigidBody::BulletRigidBody: Invalid collision shape!" << std::endl;
        return;
    }
    
    // Create motion state
    m_transform.setIdentity();
    m_transform.setOrigin(glmToBullet(position));
    
    // Convert Euler angles to quaternion
    glm::quat rotationQuat = glm::quat(glm::radians(rotation));
    m_transform.setRotation(glmToBullet(rotationQuat));
    
    m_motionState = new btDefaultMotionState(m_transform);
    
    // Calculate inertia
    btVector3 inertia(0, 0, 0);
    if (mass > 0.0f) {
        shape->calculateLocalInertia(mass, inertia);
    }
    
    // Create rigid body construction info
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, shape, inertia);
    
    // Create rigid body
    m_rigidBody = new btRigidBody(rbInfo);
    
    // Set initial properties
    if (m_isStatic) {
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    } else {
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
    }
}

BulletRigidBody::~BulletRigidBody() {
    if (m_rigidBody) {
        delete m_rigidBody;
        m_rigidBody = nullptr;
    }
    
    if (m_motionState) {
        delete m_motionState;
        m_motionState = nullptr;
    }
}

glm::vec3 BulletRigidBody::getPosition() const {
    if (!m_rigidBody) {
        return glm::vec3(0.0f);
    }
    
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    return bulletToGlm(transform.getOrigin());
}

void BulletRigidBody::setPosition(const glm::vec3& position) {
    if (!m_rigidBody) {
        return;
    }
    
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    transform.setOrigin(glmToBullet(position));
    m_rigidBody->setWorldTransform(transform);
    m_motionState->setWorldTransform(transform);
}

glm::vec3 BulletRigidBody::getRotation() const {
    if (!m_rigidBody) {
        return glm::vec3(0.0f);
    }
    
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    glm::quat quat = bulletToGlm(transform.getRotation());
    return glm::degrees(glm::eulerAngles(quat));
}

void BulletRigidBody::setRotation(const glm::vec3& rotation) {
    if (!m_rigidBody) {
        return;
    }
    
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    
    glm::quat rotationQuat = glm::quat(glm::radians(rotation));
    transform.setRotation(glmToBullet(rotationQuat));
    
    m_rigidBody->setWorldTransform(transform);
    m_motionState->setWorldTransform(transform);
}

glm::mat4 BulletRigidBody::getTransform() const {
    if (!m_rigidBody) {
        return glm::mat4(1.0f);
    }
    
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    return bulletToGlm(transform);
}

void BulletRigidBody::setTransform(const glm::mat4& transform) {
    if (!m_rigidBody) {
        return;
    }
    
    btTransform bulletTransform = glmToBullet(transform);
    m_rigidBody->setWorldTransform(bulletTransform);
    m_motionState->setWorldTransform(bulletTransform);
}

glm::vec3 BulletRigidBody::getLinearVelocity() const {
    if (!m_rigidBody) {
        return glm::vec3(0.0f);
    }
    
    return bulletToGlm(m_rigidBody->getLinearVelocity());
}

void BulletRigidBody::setLinearVelocity(const glm::vec3& velocity) {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->setLinearVelocity(glmToBullet(velocity));
}

glm::vec3 BulletRigidBody::getAngularVelocity() const {
    if (!m_rigidBody) {
        return glm::vec3(0.0f);
    }
    
    return bulletToGlm(m_rigidBody->getAngularVelocity());
}

void BulletRigidBody::setAngularVelocity(const glm::vec3& velocity) {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->setAngularVelocity(glmToBullet(velocity));
}

void BulletRigidBody::addForce(const glm::vec3& force) {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->applyCentralForce(glmToBullet(force));
}

void BulletRigidBody::addForceAtPoint(const glm::vec3& force, const glm::vec3& point) {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->applyForce(glmToBullet(force), glmToBullet(point));
}

void BulletRigidBody::addTorque(const glm::vec3& torque) {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->applyTorque(glmToBullet(torque));
}

void BulletRigidBody::applyImpulse(const glm::vec3& impulse) {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->applyCentralImpulse(glmToBullet(impulse));
}

void BulletRigidBody::applyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& point) {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->applyImpulse(glmToBullet(impulse), glmToBullet(point));
}

float BulletRigidBody::getMass() const {
    return m_mass;
}

void BulletRigidBody::setMass(float mass) {
    if (!m_rigidBody) {
        return;
    }
    
    m_mass = mass;
    m_isStatic = (mass == 0.0f);
    
    // Calculate new inertia
    btVector3 inertia(0, 0, 0);
    if (mass > 0.0f) {
        m_collisionShape->calculateLocalInertia(mass, inertia);
    }
    
    m_rigidBody->setMassProps(mass, inertia);
    
    // Update collision flags
    if (m_isStatic) {
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    } else {
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
    }
}

bool BulletRigidBody::isStatic() const {
    return m_isStatic;
}

void BulletRigidBody::setStatic(bool isStatic) {
    if (!m_rigidBody) {
        return;
    }
    
    m_isStatic = isStatic;
    
    if (isStatic) {
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
        m_rigidBody->setMassProps(0.0f, btVector3(0, 0, 0));
    } else {
        m_rigidBody->setCollisionFlags(m_rigidBody->getCollisionFlags() & ~btCollisionObject::CF_STATIC_OBJECT);
        // Restore mass if it was previously set
        if (m_mass > 0.0f) {
            setMass(m_mass);
        }
    }
}

bool BulletRigidBody::isGravityEnabled() const {
    if (!m_rigidBody) {
        return false;
    }
    
    return !(m_rigidBody->getFlags() & BT_DISABLE_WORLD_GRAVITY);
}

void BulletRigidBody::setGravityEnabled(bool enabled) {
    if (!m_rigidBody) {
        return;
    }
    
    if (enabled) {
        m_rigidBody->setFlags(m_rigidBody->getFlags() & ~BT_DISABLE_WORLD_GRAVITY);
    } else {
        m_rigidBody->setFlags(m_rigidBody->getFlags() | BT_DISABLE_WORLD_GRAVITY);
    }
}

void BulletRigidBody::enableCCD(float motionThreshold, float sweptSphereRadius) {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->setCcdMotionThreshold(motionThreshold);
    m_rigidBody->setCcdSweptSphereRadius(sweptSphereRadius);
}

void BulletRigidBody::disableCCD() {
    if (!m_rigidBody) {
        return;
    }
    
    m_rigidBody->setCcdMotionThreshold(0.0f);
    m_rigidBody->setCcdSweptSphereRadius(0.0f);
}

bool BulletRigidBody::isCCDEnabled() const {
    if (!m_rigidBody) {
        return false;
    }
    
    return m_rigidBody->getCcdMotionThreshold() > 0.0f;
}

// Conversion functions
btVector3 BulletRigidBody::glmToBullet(const glm::vec3& glmVec) const {
    return btVector3(glmVec.x, glmVec.y, glmVec.z);
}

glm::vec3 BulletRigidBody::bulletToGlm(const btVector3& bulletVec) const {
    return glm::vec3(bulletVec.x(), bulletVec.y(), bulletVec.z());
}

btQuaternion BulletRigidBody::glmToBullet(const glm::quat& glmQuat) const {
    return btQuaternion(glmQuat.x, glmQuat.y, glmQuat.z, glmQuat.w);
}

glm::quat BulletRigidBody::bulletToGlm(const btQuaternion& bulletQuat) const {
    return glm::quat(bulletQuat.w(), bulletQuat.x(), bulletQuat.y(), bulletQuat.z());
}

btTransform BulletRigidBody::glmToBullet(const glm::mat4& glmMat) const {
    btTransform transform;
    transform.setFromOpenGLMatrix(glm::value_ptr(glmMat));
    return transform;
}

glm::mat4 BulletRigidBody::bulletToGlm(const btTransform& bulletTransform) const {
    glm::mat4 mat;
    bulletTransform.getOpenGLMatrix(glm::value_ptr(mat));
    return mat;
}
