#pragma once

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <vector>

/**
 * BulletCollisionShapes - Factory class for creating Bullet Physics collision shapes
 * 
 * This class provides static methods to create various collision shapes
 * for use with Bullet Physics rigid bodies.
 * 
 * Supported Shapes:
 * - Box (AABB)
 * - Sphere
 * - Cylinder
 * - Capsule
 * - Plane
 * - Convex Hull
 * - Triangle Mesh
 * - Compound Shapes
 */
class BulletCollisionShapes {
public:
    // Basic shapes
    /**
     * Create a box collision shape
     * @param halfExtents Half extents of the box (width/2, height/2, depth/2)
     * @return Pointer to btBoxShape
     */
    static btBoxShape* CreateBox(const glm::vec3& halfExtents);
    
    /**
     * Create a sphere collision shape
     * @param radius Radius of the sphere
     * @return Pointer to btSphereShape
     */
    static btSphereShape* CreateSphere(float radius);
    
    /**
     * Create a cylinder collision shape
     * @param halfExtents Half extents of the cylinder (radius, height/2, radius)
     * @return Pointer to btCylinderShape
     */
    static btCylinderShape* CreateCylinder(const glm::vec3& halfExtents);
    
    /**
     * Create a capsule collision shape
     * @param radius Radius of the capsule
     * @param height Height of the capsule
     * @return Pointer to btCapsuleShape
     */
    static btCapsuleShape* CreateCapsule(float radius, float height);
    
    /**
     * Create a plane collision shape
     * @param normal Normal vector of the plane
     * @param constant Distance from origin along normal
     * @return Pointer to btStaticPlaneShape
     */
    static btStaticPlaneShape* CreatePlane(const glm::vec3& normal, float constant);
    
    // Advanced shapes
    /**
     * Create a convex hull collision shape
     * @param vertices Vector of vertices defining the convex hull
     * @return Pointer to btConvexHullShape
     */
    static btConvexHullShape* CreateConvexHull(const std::vector<glm::vec3>& vertices);
    
    /**
     * Create a triangle mesh collision shape
     * @param vertices Vector of vertices
     * @param triangles Vector of triangle indices
     * @return Pointer to btBvhTriangleMeshShape
     */
    static btBvhTriangleMeshShape* CreateTriangleMesh(const std::vector<glm::vec3>& vertices, 
                                                      const std::vector<glm::ivec3>& triangles);
    
    // Compound shapes
    /**
     * Create a compound collision shape
     * @return Pointer to btCompoundShape
     */
    static btCompoundShape* CreateCompoundShape();
    
    /**
     * Add a child shape to a compound shape
     * @param compound Compound shape to add child to
     * @param childShape Child shape to add
     * @param offset Offset of the child shape
     * @param rotation Rotation of the child shape (Euler angles in degrees)
     */
    static void AddChildShape(btCompoundShape* compound, btCollisionShape* childShape, 
                             const glm::vec3& offset, const glm::vec3& rotation);
    
    // Utility functions
    /**
     * Delete a collision shape (proper cleanup)
     * @param shape Shape to delete
     */
    static void DeleteShape(btCollisionShape* shape);
    
    /**
     * Calculate the volume of a collision shape
     * @param shape Shape to calculate volume for
     * @return Volume of the shape
     */
    static float CalculateVolume(btCollisionShape* shape);
    
    /**
     * Calculate the inertia tensor for a collision shape
     * @param shape Shape to calculate inertia for
     * @param mass Mass of the object
     * @return Inertia tensor
     */
    static glm::vec3 CalculateInertia(btCollisionShape* shape, float mass);
    
    // Shape validation
    /**
     * Check if a collision shape is valid
     * @param shape Shape to validate
     * @return True if shape is valid
     */
    static bool IsValidShape(btCollisionShape* shape);
    
    /**
     * Get the bounding box of a collision shape
     * @param shape Shape to get bounds for
     * @param transform Transform to apply to the shape
     * @return Bounding box (min, max)
     */
    static std::pair<glm::vec3, glm::vec3> GetBoundingBox(btCollisionShape* shape, 
                                                         const glm::mat4& transform = glm::mat4(1.0f));
    
private:
    /**
     * Convert GLM vector to Bullet vector
     * @param glmVec GLM vector
     * @return Bullet vector
     */
    static btVector3 glmToBullet(const glm::vec3& glmVec);
    
    /**
     * Convert Bullet vector to GLM vector
     * @param bulletVec Bullet vector
     * @return GLM vector
     */
    static glm::vec3 bulletToGlm(const btVector3& bulletVec);
    
    /**
     * Convert GLM quaternion to Bullet quaternion
     * @param glmQuat GLM quaternion
     * @return Bullet quaternion
     */
    static btQuaternion glmToBullet(const glm::quat& glmQuat);
    
    /**
     * Convert Bullet quaternion to GLM quaternion
     * @param bulletQuat Bullet quaternion
     * @return GLM quaternion
     */
    static glm::quat bulletToGlm(const btQuaternion& bulletQuat);
    
    /**
     * Convert GLM matrix to Bullet transform
     * @param glmMat GLM matrix
     * @return Bullet transform
     */
    static btTransform glmToBullet(const glm::mat4& glmMat);
};
