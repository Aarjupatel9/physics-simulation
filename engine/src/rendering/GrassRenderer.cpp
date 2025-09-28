#include "GrassRenderer.h"
#include "Terrain.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>
#include <iostream>

GrassRenderer::GrassRenderer() : m_VAO(0), m_VBO(0), m_EBO(0), m_instanceVBO(0), m_shaderProgram(0) {}

GrassRenderer::~GrassRenderer() {
    cleanup();
}

bool GrassRenderer::initialize() {
    createGrassBlade();
    
    if (!createShader()) {
        return false;
    }
    
    return true;
}

void GrassRenderer::generateGrass(const Terrain& terrain, int grassCount) {
    m_instances.clear();
    m_instances.reserve(grassCount);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> posDist(-15.0f, 15.0f);
    std::uniform_real_distribution<float> scaleDist(0.3f, 1.2f);
    std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
    
    for (int i = 0; i < grassCount; ++i) {
        float x = posDist(gen);
        float z = posDist(gen);
        float y = terrain.getHeightAt(x, z);
        
        // Only place grass on suitable terrain (not too steep, not too low)
        if (y > -0.5f && y < 1.0f) {
            GrassInstance instance;
            instance.position = glm::vec3(x, y, z);
            instance.scale = scaleDist(gen);
            instance.rotation = rotDist(gen);
            
            // Vary grass color based on height
            float greenIntensity = 0.3f + (y + 0.5f) * 0.4f;
            instance.color = glm::vec3(0.1f, greenIntensity, 0.1f);
            
            m_instances.push_back(instance);
        }
    }
    
    std::cout << "Generated " << m_instances.size() << " grass instances" << std::endl;
    
    // Update instance buffer
    if (!m_instances.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, m_instances.size() * sizeof(GrassInstance), m_instances.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void GrassRenderer::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& lightDir) {
    if (m_instances.empty()) return;
    
    glUseProgram(m_shaderProgram);
    
    // Set matrices
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    // Set lighting
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "lightDir"), 1, glm::value_ptr(lightDir));
    glUniform3f(glGetUniformLocation(m_shaderProgram, "lightColor"), 1.0f, 1.0f, 0.9f);
    glUniform3f(glGetUniformLocation(m_shaderProgram, "ambientColor"), 0.2f, 0.3f, 0.2f);
    
    // Bind VAO
    glBindVertexArray(m_VAO);
    
    // Render all instances
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(m_grassIndices.size()), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(m_instances.size()));
    
    glBindVertexArray(0);
}

void GrassRenderer::clear() {
    m_instances.clear();
}

void GrassRenderer::createGrassBlade() {
    // Simple grass blade geometry (2 triangles)
    m_grassVertices = {
        // Position (x, y, z)
        0.0f, 0.0f, 0.0f,  // Bottom center
        0.0f, 1.0f, 0.0f,  // Top center
        -0.1f, 0.0f, 0.0f, // Bottom left
        0.1f, 0.0f, 0.0f,  // Bottom right
        -0.05f, 0.7f, 0.0f, // Mid left
        0.05f, 0.7f, 0.0f   // Mid right
    };
    
    m_grassIndices = {
        0, 2, 4,  // Left triangle
        0, 4, 1,  // Left triangle
        0, 1, 5,  // Right triangle
        0, 5, 3   // Right triangle
    };
    
    // Create VAO and VBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_instanceVBO);
    
    glBindVertexArray(m_VAO);
    
    // Upload grass blade vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_grassVertices.size() * sizeof(float), m_grassVertices.data(), GL_STATIC_DRAW);
    
    // Upload indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_grassIndices.size() * sizeof(unsigned int), m_grassIndices.data(), GL_STATIC_DRAW);
    
    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Setup instance data (will be updated when grass is generated)
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    
    // Instance position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GrassInstance), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1, 1);
    
    // Instance scale
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GrassInstance), (void*)offsetof(GrassInstance, scale));
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    
    // Instance rotation
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GrassInstance), (void*)offsetof(GrassInstance, rotation));
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    
    // Instance color
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GrassInstance), (void*)offsetof(GrassInstance, color));
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
    
    glBindVertexArray(0);
}

bool GrassRenderer::createShader() {
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
    vec3 normal = vec3(0.0, 1.0, 0.0); // Grass blades point up
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
        std::cerr << "Grass vertex shader compilation failed: " << infoLog << std::endl;
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
        std::cerr << "Grass fragment shader compilation failed: " << infoLog << std::endl;
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
        std::cerr << "Grass shader program linking failed: " << infoLog << std::endl;
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

void GrassRenderer::cleanup() {
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