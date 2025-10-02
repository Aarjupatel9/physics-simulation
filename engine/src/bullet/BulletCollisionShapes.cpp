#include "bullet/BulletCollisionShapes.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

btBoxShape* BulletCollisionShapes::CreateBox(const glm::vec3& halfExtents) {
    btBoxShape* shape = new btBoxShape(glmToBullet(halfExtents));
    shape->setMargin(0.01f); // Increased collision margin for better contact resolution
    std::cout << "DEBUG: Created box shape with halfExtents(" << halfExtents.x << ", " << halfExtents.y << ", " << halfExtents.z 
              << ") margin=" << shape->getMargin() << std::endl;
    return shape;
}

btSphereShape* BulletCollisionShapes::CreateSphere(float radius) {
    btSphereShape* shape = new btSphereShape(radius);
    // Don't override margin for spheres - let Bullet handle it naturally
    std::cout << "DEBUG: Created sphere shape with radius=" << radius 
              << " margin=" << shape->getMargin() << std::endl;
    return shape;
}

btCylinderShape* BulletCollisionShapes::CreateCylinder(const glm::vec3& halfExtents) {
    return new btCylinderShape(glmToBullet(halfExtents));
}

btCapsuleShape* BulletCollisionShapes::CreateCapsule(float radius, float height) {
    return new btCapsuleShape(radius, height);
}

btStaticPlaneShape* BulletCollisionShapes::CreatePlane(const glm::vec3& normal, float constant) {
    btStaticPlaneShape* shape = new btStaticPlaneShape(glmToBullet(normal), constant);
    shape->setMargin(0.01f); // Set collision margin
    return shape;
}

btConvexHullShape* BulletCollisionShapes::CreateConvexHull(const std::vector<glm::vec3>& vertices) {
    if (vertices.empty()) {
        std::cerr << "BulletCollisionShapes::CreateConvexHull: No vertices provided!" << std::endl;
        return nullptr;
    }
    
    btConvexHullShape* hull = new btConvexHullShape();
    
    for (const auto& vertex : vertices) {
        hull->addPoint(glmToBullet(vertex));
    }
    
    // Optimize the hull
    hull->optimizeConvexHull();
    
    return hull;
}

btBvhTriangleMeshShape* BulletCollisionShapes::CreateTriangleMesh(const std::vector<glm::vec3>& vertices, 
                                                                  const std::vector<glm::ivec3>& triangles) {
    if (vertices.empty() || triangles.empty()) {
        std::cerr << "BulletCollisionShapes::CreateTriangleMesh: Invalid input data!" << std::endl;
        return nullptr;
    }
    
    btTriangleMesh* mesh = new btTriangleMesh();
    
    for (const auto& triangle : triangles) {
        if (triangle.x >= 0 && triangle.x < vertices.size() &&
            triangle.y >= 0 && triangle.y < vertices.size() &&
            triangle.z >= 0 && triangle.z < vertices.size()) {
            
            mesh->addTriangle(
                glmToBullet(vertices[triangle.x]),
                glmToBullet(vertices[triangle.y]),
                glmToBullet(vertices[triangle.z])
            );
        }
    }
    
    return new btBvhTriangleMeshShape(mesh, true);
}

btCompoundShape* BulletCollisionShapes::CreateCompoundShape() {
    return new btCompoundShape();
}

void BulletCollisionShapes::AddChildShape(btCompoundShape* compound, btCollisionShape* childShape, 
                                         const glm::vec3& offset, const glm::vec3& rotation) {
    if (!compound || !childShape) {
        std::cerr << "BulletCollisionShapes::AddChildShape: Invalid parameters!" << std::endl;
        return;
    }
    
    btTransform childTransform;
    childTransform.setIdentity();
    childTransform.setOrigin(glmToBullet(offset));
    
    // Convert Euler angles to quaternion
    glm::quat rotationQuat = glm::quat(glm::radians(rotation));
    childTransform.setRotation(glmToBullet(rotationQuat));
    
    compound->addChildShape(childTransform, childShape);
}

void BulletCollisionShapes::DeleteShape(btCollisionShape* shape) {
    if (shape) {
        delete shape;
    }
}

float BulletCollisionShapes::CalculateVolume(btCollisionShape* shape) {
    if (!shape) {
        return 0.0f;
    }
    
    // This is a simplified volume calculation
    // For more accurate results, you might need to implement specific calculations for each shape type
    
    switch (shape->getShapeType()) {
        case BOX_SHAPE_PROXYTYPE: {
            btBoxShape* box = static_cast<btBoxShape*>(shape);
            btVector3 halfExtents = box->getHalfExtentsWithMargin();
            return 8.0f * halfExtents.x() * halfExtents.y() * halfExtents.z();
        }
        case SPHERE_SHAPE_PROXYTYPE: {
            btSphereShape* sphere = static_cast<btSphereShape*>(shape);
            float radius = sphere->getRadius();
            return (4.0f / 3.0f) * 3.14159f * radius * radius * radius;
        }
        case CYLINDER_SHAPE_PROXYTYPE: {
            btCylinderShape* cylinder = static_cast<btCylinderShape*>(shape);
            btVector3 halfExtents = cylinder->getHalfExtentsWithMargin();
            return 3.14159f * halfExtents.x() * halfExtents.x() * 2.0f * halfExtents.y();
        }
        case CAPSULE_SHAPE_PROXYTYPE: {
            btCapsuleShape* capsule = static_cast<btCapsuleShape*>(shape);
            float radius = capsule->getRadius();
            float height = capsule->getHalfHeight() * 2.0f;
            float sphereVolume = (4.0f / 3.0f) * 3.14159f * radius * radius * radius;
            float cylinderVolume = 3.14159f * radius * radius * height;
            return sphereVolume + cylinderVolume;
        }
        default:
            return 0.0f;
    }
}

glm::vec3 BulletCollisionShapes::CalculateInertia(btCollisionShape* shape, float mass) {
    if (!shape || mass <= 0.0f) {
        return glm::vec3(0.0f);
    }
    
    btVector3 inertia(0, 0, 0);
    shape->calculateLocalInertia(mass, inertia);
    
    return bulletToGlm(inertia);
}

bool BulletCollisionShapes::IsValidShape(btCollisionShape* shape) {
    if (!shape) {
        return false;
    }
    
    // Basic validation - check if shape type is valid
    int shapeType = shape->getShapeType();
    return (shapeType >= 0 && shapeType < MAX_BROADPHASE_COLLISION_TYPES);
}

std::pair<glm::vec3, glm::vec3> BulletCollisionShapes::GetBoundingBox(btCollisionShape* shape, 
                                                                     const glm::mat4& transform) {
    if (!shape) {
        return {glm::vec3(0.0f), glm::vec3(0.0f)};
    }
    
    btTransform bulletTransform = glmToBullet(transform);
    btVector3 minAabb, maxAabb;
    shape->getAabb(bulletTransform, minAabb, maxAabb);
    
    return {bulletToGlm(minAabb), bulletToGlm(maxAabb)};
}

// Conversion functions
btVector3 BulletCollisionShapes::glmToBullet(const glm::vec3& glmVec) {
    return btVector3(glmVec.x, glmVec.y, glmVec.z);
}

glm::vec3 BulletCollisionShapes::bulletToGlm(const btVector3& bulletVec) {
    return glm::vec3(bulletVec.x(), bulletVec.y(), bulletVec.z());
}

btQuaternion BulletCollisionShapes::glmToBullet(const glm::quat& glmQuat) {
    return btQuaternion(glmQuat.x, glmQuat.y, glmQuat.z, glmQuat.w);
}

glm::quat BulletCollisionShapes::bulletToGlm(const btQuaternion& bulletQuat) {
    return glm::quat(bulletQuat.w(), bulletQuat.x(), bulletQuat.y(), bulletQuat.z());
}

btTransform BulletCollisionShapes::glmToBullet(const glm::mat4& glmMat) {
    btTransform transform;
    transform.setFromOpenGLMatrix(glm::value_ptr(glmMat));
    return transform;
}
