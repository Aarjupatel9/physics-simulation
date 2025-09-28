#include "Mesh.h"
#include <glad/glad.h>
#include <iostream>

Mesh::Mesh() : m_VAO(0), m_VBO(0), m_EBO(0), m_vertexCount(0), m_indexCount(0), m_hasIndices(false) {}

Mesh::~Mesh() {
    cleanup();
}

void Mesh::loadVertices(const std::vector<float>& vertices) {
    cleanup();
    
    m_vertexCount = vertices.size() / 3; // Assuming 3 components per vertex
    m_hasIndices = false;
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void Mesh::loadVertices(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    cleanup();
    
    m_vertexCount = vertices.size() / 3; // Assuming 3 components per vertex
    m_indexCount = indices.size();
    m_hasIndices = true;
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Position attribute (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(m_VAO);
    if (m_hasIndices) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertexCount));
    }
    glBindVertexArray(0);
}

void Mesh::cleanup() {
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
    m_vertexCount = 0;
    m_indexCount = 0;
    m_hasIndices = false;
}
