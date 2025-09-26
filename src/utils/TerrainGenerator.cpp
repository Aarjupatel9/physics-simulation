#include "TerrainGenerator.h"
#include <cmath>
#include <algorithm>

TerrainGenerator::TerrainData TerrainGenerator::generateTerrain(int width, int height, float scale, float heightScale, float roughness) {
    TerrainData data;
    data.width = width;
    data.height = height;
    
    // Generate vertices
    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            float worldX = (x - width / 2.0f) * scale;
            float worldZ = (z - height / 2.0f) * scale;
            float worldY = getHeight(worldX, worldZ, scale, heightScale, roughness);
            
            // Position
            data.vertices.push_back(worldX);
            data.vertices.push_back(worldY);
            data.vertices.push_back(worldZ);
            
            // Normal
            glm::vec3 normal = calculateNormal(worldX, worldZ, scale, heightScale, roughness);
            data.normals.push_back(normal.x);
            data.normals.push_back(normal.y);
            data.normals.push_back(normal.z);
            
            // Color based on height and slope
            float slope = 1.0f - normal.y; // Slope factor
            glm::vec3 color = getTerrainColor(worldY, slope);
            data.colors.push_back(color.r);
            data.colors.push_back(color.g);
            data.colors.push_back(color.b);
        }
    }
    
    // Generate indices for triangles
    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int topLeft = z * width + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * width + x;
            int bottomRight = bottomLeft + 1;
            
            // First triangle
            data.indices.push_back(topLeft);
            data.indices.push_back(bottomLeft);
            data.indices.push_back(topRight);
            
            // Second triangle
            data.indices.push_back(topRight);
            data.indices.push_back(bottomLeft);
            data.indices.push_back(bottomRight);
        }
    }
    
    return data;
}

float TerrainGenerator::getHeight(float x, float z, float scale, float heightScale, float roughness) {
    // Simple flat terrain for testing - no height variation
    return 0.0f;
}

glm::vec3 TerrainGenerator::calculateNormal(float x, float z, float scale, float heightScale, float roughness) {
    // Flat terrain - all normals point straight up
    return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 TerrainGenerator::getTerrainColor(float height, float slope) {
    // Simple base color for terrain visibility
    // #ebebeb = RGB(235, 235, 235) = (0.92, 0.92, 0.92)
    return glm::vec3(0.92f, 0.92f, 0.92f);
}

float TerrainGenerator::noise(float x, float z) {
    return smoothNoise(x, z);
}

float TerrainGenerator::smoothNoise(float x, float z) {
    // Simple pseudo-random noise
    float n = sin(x * 12.9898f + z * 78.233f) * 43758.5453f;
    n = n - floor(n);
    return n * 2.0f - 1.0f;
}

float TerrainGenerator::interpolate(float a, float b, float blend) {
    float theta = blend * 3.14159265f;
    float f = (1.0f - cos(theta)) * 0.5f;
    return a * (1.0f - f) + b * f;
}
