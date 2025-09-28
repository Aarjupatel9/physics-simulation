#pragma once

#include <vector>
#include <glm/glm.hpp>

// Forward declare OpenGL types
typedef unsigned int GLuint;

// Manages OpenGL mesh data (VAO, VBO, EBO)
class Mesh {
public:
    Mesh();
    ~Mesh();
    
    // Load mesh data
    void loadVertices(const std::vector<float>& vertices);
    void loadVertices(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    
    // Render the mesh
    void draw() const;
    
    // Get vertex count
    size_t getVertexCount() const { return m_vertexCount; }
    size_t getIndexCount() const { return m_indexCount; }

private:
    GLuint m_VAO;
    GLuint m_VBO;
    GLuint m_EBO;
    
    size_t m_vertexCount;
    size_t m_indexCount;
    bool m_hasIndices;
    
    void cleanup();
};
