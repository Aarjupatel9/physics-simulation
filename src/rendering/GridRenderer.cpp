#include "GridRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

GridRenderer::GridRenderer() : m_VAO(0), m_VBO(0), m_shaderProgram(0), m_color(0.3f, 0.3f, 0.3f) {}

GridRenderer::~GridRenderer() {
    cleanup();
}

bool GridRenderer::initialize(float size, int divisions) {
    createGrid(size, divisions);
    
    if (!createShader()) {
        return false;
    }
    
    return true;
}

void GridRenderer::render(const glm::mat4& view, const glm::mat4& projection) {
    glUseProgram(m_shaderProgram);
    
    // Set matrices
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    // Set color
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "color"), 1, glm::value_ptr(m_color));
    
    // Make lines thicker for better visibility
    glLineWidth(2.0f);
    
    // Render grid lines
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(m_vertices.size() / 3));
    glBindVertexArray(0);
    
    // Reset line width
    glLineWidth(1.0f);
}

void GridRenderer::createGrid(float size, int divisions) {
    m_vertices.clear();
    
    float step = size / divisions;
    float halfSize = size * 0.5f;
    
    // Create a simple square outline first to test
    // Bottom edge
    m_vertices.push_back(-halfSize); m_vertices.push_back(0.01f); m_vertices.push_back(-halfSize);
    m_vertices.push_back(halfSize);  m_vertices.push_back(0.01f); m_vertices.push_back(-halfSize);
    
    // Right edge  
    m_vertices.push_back(halfSize);  m_vertices.push_back(0.01f); m_vertices.push_back(-halfSize);
    m_vertices.push_back(halfSize);  m_vertices.push_back(0.01f); m_vertices.push_back(halfSize);
    
    // Top edge
    m_vertices.push_back(halfSize);  m_vertices.push_back(0.01f); m_vertices.push_back(halfSize);
    m_vertices.push_back(-halfSize); m_vertices.push_back(0.01f); m_vertices.push_back(halfSize);
    
    // Left edge
    m_vertices.push_back(-halfSize); m_vertices.push_back(0.01f); m_vertices.push_back(halfSize);
    m_vertices.push_back(-halfSize); m_vertices.push_back(0.01f); m_vertices.push_back(-halfSize);
    
    // Add center lines
    // Vertical center line
    m_vertices.push_back(0.0f); m_vertices.push_back(0.01f); m_vertices.push_back(-halfSize);
    m_vertices.push_back(0.0f); m_vertices.push_back(0.01f); m_vertices.push_back(halfSize);
    
    // Horizontal center line
    m_vertices.push_back(-halfSize); m_vertices.push_back(0.01f); m_vertices.push_back(0.0f);
    m_vertices.push_back(halfSize);  m_vertices.push_back(0.01f); m_vertices.push_back(0.0f);
    
    // Create VAO and VBO
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
    
    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

bool GridRenderer::createShader() {
    const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
}
)";

    const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0);
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
        std::cerr << "Grid vertex shader compilation failed: " << infoLog << std::endl;
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
        std::cerr << "Grid fragment shader compilation failed: " << infoLog << std::endl;
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
        std::cerr << "Grid shader program linking failed: " << infoLog << std::endl;
        return false;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

void GridRenderer::cleanup() {
    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
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