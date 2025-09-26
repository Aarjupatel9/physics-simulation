#include "Terrain.h"
#include "../utils/TerrainGenerator.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Terrain::Terrain() : m_width(0), m_height(0), m_scale(0.1f) {}

Terrain::~Terrain() = default;

bool Terrain::initialize(int width, int height, float scale, float heightScale, float roughness) {
    m_width = width;
    m_height = height;
    m_scale = scale;
    m_bounds = glm::vec2(width * scale, height * scale);
    
    // Generate terrain data
    auto terrainData = TerrainGenerator::generateTerrain(width, height, scale, heightScale, roughness);
    m_heightData = std::vector<float>(terrainData.vertices.size() / 3);
    
    // Extract height data
    for (size_t i = 0; i < terrainData.vertices.size(); i += 3) {
        m_heightData[i / 3] = terrainData.vertices[i + 1]; // Y component
    }
    
    // Create mesh with vertices, normals, colors, and indices
    m_mesh = std::make_unique<Mesh>();
    
    // Combine vertices, normals, and colors into interleaved format
    std::vector<float> interleavedData;
    for (size_t i = 0; i < terrainData.vertices.size(); i += 3) {
        size_t idx = i / 3;
        
        // Position
        interleavedData.push_back(terrainData.vertices[i]);
        interleavedData.push_back(terrainData.vertices[i + 1]);
        interleavedData.push_back(terrainData.vertices[i + 2]);
        
        // Normal
        interleavedData.push_back(terrainData.normals[i]);
        interleavedData.push_back(terrainData.normals[i + 1]);
        interleavedData.push_back(terrainData.normals[i + 2]);
        
        // Color
        interleavedData.push_back(terrainData.colors[i]);
        interleavedData.push_back(terrainData.colors[i + 1]);
        interleavedData.push_back(terrainData.colors[i + 2]);
    }
    
    // Store terrain data for rendering
    m_terrainVAO = 0;
    m_terrainVBO = 0;
    m_terrainEBO = 0;
    m_indexCount = terrainData.indices.size();
    
    // Create VAO manually for interleaved data
    glGenVertexArrays(1, &m_terrainVAO);
    glGenBuffers(1, &m_terrainVBO);
    glGenBuffers(1, &m_terrainEBO);
    
    glBindVertexArray(m_terrainVAO);
    
    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(float), interleavedData.data(), GL_STATIC_DRAW);
    
    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, terrainData.indices.size() * sizeof(unsigned int), terrainData.indices.data(), GL_STATIC_DRAW);
    
    // Set vertex attributes (9 floats per vertex: 3 pos + 3 normal + 3 color)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
    
    // Create shader
    if (!createShader()) {
        return false;
    }
    
    return true;
}

void Terrain::render(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDir) {
    m_shader->use();
    
    // Set matrices
    m_shader->setUniform("model", model);
    m_shader->setUniform("view", view);
    m_shader->setUniform("projection", projection);
    
    // Set lighting
    m_shader->setUniform("lightDir", lightDir);
    m_shader->setUniform("lightColor", glm::vec3(1.0f, 1.0f, 0.9f));
    m_shader->setUniform("ambientColor", glm::vec3(0.3f, 0.4f, 0.5f));
    
    // Render terrain
    glBindVertexArray(m_terrainVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

float Terrain::getHeightAt(float worldX, float worldZ) const {
    glm::ivec2 coords = worldToHeightmap(worldX, worldZ);
    
    if (coords.x < 0 || coords.x >= m_width - 1 || coords.y < 0 || coords.y >= m_height - 1) {
        return 0.0f;
    }
    
    // Bilinear interpolation
    float fx = (worldX + m_bounds.x * 0.5f) / m_scale - coords.x;
    float fz = (worldZ + m_bounds.y * 0.5f) / m_scale - coords.y;
    
    int idx1 = coords.y * m_width + coords.x;
    int idx2 = idx1 + 1;
    int idx3 = (coords.y + 1) * m_width + coords.x;
    int idx4 = idx3 + 1;
    
    float h1 = m_heightData[idx1];
    float h2 = m_heightData[idx2];
    float h3 = m_heightData[idx3];
    float h4 = m_heightData[idx4];
    
    float h12 = h1 * (1.0f - fx) + h2 * fx;
    float h34 = h3 * (1.0f - fx) + h4 * fx;
    
    return h12 * (1.0f - fz) + h34 * fz;
}

glm::ivec2 Terrain::worldToHeightmap(float worldX, float worldZ) const {
    int x = static_cast<int>((worldX + m_bounds.x * 0.5f) / m_scale);
    int z = static_cast<int>((worldZ + m_bounds.y * 0.5f) / m_scale);
    return glm::ivec2(x, z);
}

bool Terrain::createShader() {
    const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

out vec3 fragColor;

void main() {
    vec3 worldPos = vec3(model * vec4(aPos, 1.0));
    vec3 worldNormal = normalize(mat3(transpose(inverse(model))) * aNormal);
    
    // Simple lighting
    float diff = max(dot(worldNormal, normalize(-lightDir)), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = ambientColor;
    
    fragColor = aColor * (ambient + diffuse);
    
    gl_Position = projection * view * vec4(worldPos, 1.0);
}
)";

    const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 fragColor;

void main() {
    FragColor = vec4(fragColor, 1.0);
}
)";

    m_shader = std::make_unique<Shader>();
    return m_shader->loadFromSource(vertexShaderSource, fragmentShaderSource);
}
