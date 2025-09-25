#include "MeshGenerator.h"
#include <cmath>

std::vector<float> MeshGenerator::generateCube() {
    return {
        // Front face
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
        // Back face
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,
        // Left face
        -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,
        // Right face
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
        // Top face
        -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
    };
}

std::vector<float> MeshGenerator::generateSphere(unsigned int longitudeSegments, unsigned int latitudeSegments, float radius) {
    std::vector<float> vertices;
    const float pi = 3.14159265358979323846f;
    
    for (unsigned int y = 0; y < latitudeSegments; ++y) {
        float v0 = (float)y / (float)latitudeSegments;
        float v1 = (float)(y + 1) / (float)latitudeSegments;
        float theta0 = v0 * pi;
        float theta1 = v1 * pi;

        for (unsigned int x = 0; x < longitudeSegments; ++x) {
            float u0 = (float)x / (float)longitudeSegments;
            float u1 = (float)(x + 1) / (float)longitudeSegments;
            float phi0 = u0 * 2.0f * pi;
            float phi1 = u1 * 2.0f * pi;

            // Four positions on the sphere quad strip
            float p00_x = radius * sinf(theta0) * cosf(phi0);
            float p00_y = radius * cosf(theta0);
            float p00_z = radius * sinf(theta0) * sinf(phi0);
            
            float p10_x = radius * sinf(theta0) * cosf(phi1);
            float p10_y = radius * cosf(theta0);
            float p10_z = radius * sinf(theta0) * sinf(phi1);
            
            float p01_x = radius * sinf(theta1) * cosf(phi0);
            float p01_y = radius * cosf(theta1);
            float p01_z = radius * sinf(theta1) * sinf(phi0);
            
            float p11_x = radius * sinf(theta1) * cosf(phi1);
            float p11_y = radius * cosf(theta1);
            float p11_z = radius * sinf(theta1) * sinf(phi1);

            // Two triangles: (p00, p10, p11) and (p00, p11, p01)
            vertices.insert(vertices.end(), {p00_x, p00_y, p00_z, p10_x, p10_y, p10_z, p11_x, p11_y, p11_z});
            vertices.insert(vertices.end(), {p00_x, p00_y, p00_z, p11_x, p11_y, p11_z, p01_x, p01_y, p01_z});
        }
    }
    
    return vertices;
}

std::vector<float> MeshGenerator::generateGroundPlane() {
    return {
        -10.0f, -1.0f, -10.0f,  10.0f, -1.0f, -10.0f,  10.0f, -1.0f,  10.0f,
         10.0f, -1.0f,  10.0f, -10.0f, -1.0f,  10.0f, -10.0f, -1.0f, -10.0f
    };
}

std::vector<float> MeshGenerator::generatePlane(float width, float height) {
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    
    return {
        -halfWidth, 0.0f, -halfHeight,  halfWidth, 0.0f, -halfHeight,  halfWidth, 0.0f,  halfHeight,
         halfWidth, 0.0f,  halfHeight, -halfWidth, 0.0f,  halfHeight, -halfWidth, 0.0f, -halfHeight
    };
}
