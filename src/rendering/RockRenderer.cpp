#include "RockRenderer.h"
#include "Terrain.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <iostream>

RockRenderer::RockRenderer() : m_VAO(0), m_VBO(0), m_EBO(0), m_instanceVBO(0), m_shaderProgram(0) {}

RockRenderer::~RockRenderer() {
    cleanup();
}

bool RockRenderer::initialize() {
    createRockGeometry();
    
    if (!createShader()) {
        return false;
    }
    
    return true;
}

void RockRenderer::generateRocks(const Terrain& terrain, int rockCount) {
    m_instances.clear();
    m_instances.reserve(rockCount);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(-12.0f, 12.0f);
    std::uniform_real_distribution<float> scaleDist(0.1f, 0.4f);
    std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
    
    for (int i = 0; i < rockCount; ++i) {
        float x = posDist(gen);
        float z = posDist(gen);
        float y = terrain.getHeightAt(x, z);
        
        // Only place rocks on suitable terrain (not too steep, not too low)
        if (y > -0.3f && y < 0.8f) {
            RockInstance instance;
            instance.position = glm::vec3(x, y, z);
            instance.scale = scaleDist(gen);
            instance.rotation = rotDist(gen);
            
            // Vary rock color based on height
            float grayIntensity = 0.3f + (y + 0.3f) * 0.3f;
            instance.color = glm::vec3(grayIntensity, grayIntensity * 0.9f, grayIntensity * 0.8f);
            
            m_instances.push_back(instance);
        }
    }
    
    std::cout << "Generated " << m_instances.size() << " rock instances" << std::endl;
    
    // Update instance buffer
    if (!m_instances.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, m_instances.size() * sizeof(RockInstance), m_instances.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void RockRenderer::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDir) {
    if (m_instances.empty()) return;
    
    glUseProgram(m_shaderProgram);
    
    // Set matrices
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    // Set lighting
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "lightDir"), 1, glm::value_ptr(lightDir));
    glUniform3f(glGetUniformLocation(m_shaderProgram, "lightColor"), 1.0f, 1.0f, 0.9f);
    glUniform3f(glGetUniformLocation(m_shaderProgram, "ambientColor"), 0.2f, 0.2f, 0.2f);
    
    // Bind VAO
    glBindVertexArray(m_VAO);
    
    // Render all instances
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_rockIndices.size()), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(m_instances.size()));
    
    glBindVertexArray(0);
}

void RockRenderer::clear() {
    m_instances.clear();
}

void RockRenderer::createRockGeometry() {
    // Simple cube geometry for rocks
    m_rockVertices = {
        // Front face
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        // Back face
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f
    };
    
    m_rockIndices = {
        // Front face
        0, 1, 2,  2, 3, 0,
        // Back face
        4, 5, 6,  6, 7, 4,
        // Left face
        7, 3, 0,  0, 4, 7,
        // Right face
        1, 5, 6,  6, 2, 1,
        // Top face
        3, 2, 6,  6, 7, 3,
        // Bottom face
        0, 1, 5,  5, 4, 0
    };
    
    // Create VAO and VBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_instanceVBO);
    
    glBindVertexArray(m_VAO);
    
    // Upload rock vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_rockVertices.size() * sizeof(float), m_rockVertices.data(), GL_STATIC_DRAW);
    
    // Upload indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_rockIndices.size() * sizeof(unsigned int), m_rockIndices.data(), GL_STATIC_DRAW);
    
    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Setup instance data
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    
    // Instance position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RockInstance), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    
    // Instance scale
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(RockInstance), (void*)offsetof(RockInstance, scale));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    
    // Instance rotation
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(RockInstance), (void*)offsetof(RockInstance, rotation));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    
    // Instance color
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(RockInstance), (void*)offsetof(RockInstance, color));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
    
    glBindVertexArray(0);
}

bool RockRenderer::createShader() {
    const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aInstancePos;
layout (location = 2) in float aInstanceScale;
layout (location = 3) in float aInstanceRot;
layout (location = 4) in vec3 aInstanceColor;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientColor;

out vec3 fragColor;

void main() {
    // Apply instance transformations
    float cosRot = cos(aInstanceRot);
    float sinRot = sin(aInstanceRot);
    
    vec3 rotatedPos = vec3(
        aPos.x * cosRot - aPos.z * sinRot,
        aPos.y,
        aPos.x * sinRot + aPos.z * cosRot
    );
    
    vec3 worldPos = aInstancePos + rotatedPos * aInstanceScale;
    
    // Simple lighting
    vec3 normal = normalize(rotatedPos); // Approximate normal
    float diff = max(dot(normal, normalize(-lightDir)), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 ambient = ambientColor;
    
    fragColor = aInstanceColor * (ambient + diffuse);
    
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

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Rock vertex shader compilation failed: " << infoLog << std::endl;
        return false;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Rock fragment shader compilation failed: " << infoLog << std::endl;
        return false;
    }
    
    // Link shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(m_shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Rock shader program linking failed: " << infoLog << std::endl;
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

void RockRenderer::cleanup() {
    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
    if (m_instanceVBO) {
        glDeleteBuffers(1, &m_instanceVBO);
        m_instanceVBO = 0;
    }
    if (m_EBO) {
        glDeleteBuffers(1, &m_EBO);
        m_EBO = 0;
    }
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}