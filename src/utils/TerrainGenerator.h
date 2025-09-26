#pragma once

#include <vector>
#include <glm/glm.hpp>

// Generates procedural terrain using Perlin-like noise
class TerrainGenerator {
public:
    struct TerrainData {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> colors;
        std::vector<unsigned int> indices;
        int width, height;
    };
    
    // Generate terrain with specified parameters
    static TerrainData generateTerrain(int width, int height, float scale, float heightScale, float roughness);
    
    // Generate height at specific position using noise
    static float getHeight(float x, float z, float scale, float heightScale, float roughness);
    
    // Generate normal vector for lighting
    static glm::vec3 calculateNormal(float x, float z, float scale, float heightScale, float roughness);
    
    // Get terrain color based on height and slope
    static glm::vec3 getTerrainColor(float height, float slope);

private:
    // Simple noise function (Perlin-like)
    static float noise(float x, float z);
    static float smoothNoise(float x, float z);
    static float interpolate(float a, float b, float blend);
};
