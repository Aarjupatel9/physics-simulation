#pragma once

#include <vector>

// Utility class for generating common 3D meshes
class MeshGenerator {
public:
    // Generate a cube mesh (vertices only)
    static std::vector<float> generateCube();
    
    // Generate a UV sphere mesh
    static std::vector<float> generateSphere(unsigned int longitudeSegments, unsigned int latitudeSegments, float radius);
    
    // Generate a ground plane mesh
    static std::vector<float> generateGroundPlane();
    
    // Generate a plane mesh
    static std::vector<float> generatePlane(float width, float height);
};
