#include "FPSRenderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <numeric>

FPSRenderer::FPSRenderer() {
    m_lastFrameTime = std::chrono::high_resolution_clock::now();
    m_metrics.fpsHistory.resize(PerformanceMetrics::HISTORY_SIZE, 0.0f);
    m_metrics.frameTimeHistory.resize(PerformanceMetrics::HISTORY_SIZE, 0.0f);
}

FPSRenderer::~FPSRenderer() {
    cleanup();
}

bool FPSRenderer::initialize() {
    // Create simple shader for text rendering
    if (!createShader()) {
        std::cerr << "Failed to create FPS renderer shader" << std::endl;
        return false;
    }
    
    // Create VAO/VBO for rendering
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    
    // Set up vertex attributes (position only)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    
    m_fontLoaded = true; // Simple font system is always "loaded"
    return true;
}

void FPSRenderer::update(float deltaTime, int objectCount, int collisionChecks, int drawCalls, int trianglesRendered) {
    updateMetrics(deltaTime, objectCount, collisionChecks, drawCalls, trianglesRendered);
}

void FPSRenderer::render(const glm::mat4& view, const glm::mat4& projection) {
    if (!m_displayEnabled) return;
    
    // Debug output removed - display is working
    
    // Save current OpenGL state
    GLboolean depthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean blendEnabled = glIsEnabled(GL_BLEND);
    GLint currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    
    // Enable blending for UI elements
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Disable depth testing for UI elements
    glDisable(GL_DEPTH_TEST);
    
    // Use our shader
    glUseProgram(m_shaderProgram);
    
    // Set up orthographic projection for 2D rendering (ignore passed matrices)
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float width = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);
    
    glm::mat4 ortho = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(ortho));
    
    // Render background - smaller size for just FPS
    float bgWidth = 120.0f * m_scale;
    float bgHeight = 40.0f * m_scale;
    renderBackground(m_position.x, m_position.y, bgWidth, bgHeight, glm::vec3(0.1f, 0.1f, 0.1f)); // Dark background
    
    // Render only FPS
    float xOffset = m_position.x + 15.0f * m_scale;
    float yOffset = m_position.y + 15.0f * m_scale;
    
    // FPS with color coding based on displayed FPS
    glm::vec3 fpsColor = getPerformanceColor(m_displayedFPS);
    renderText("FPS: " + std::to_string(static_cast<int>(m_displayedFPS)), xOffset, yOffset, fpsColor);
    
    // Restore OpenGL state
    if (depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
    
    if (!blendEnabled) {
        glDisable(GL_BLEND);
    }
    
    glUseProgram(currentProgram);
}

void FPSRenderer::updateMetrics(float deltaTime, int objectCount, int collisionChecks, int drawCalls, int trianglesRendered) {
    // Update basic metrics
    m_metrics.fps = (deltaTime > 0.0f) ? (1.0f / deltaTime) : 0.0f;
    m_metrics.frameTime = deltaTime;
    m_metrics.objectCount = objectCount;
    m_metrics.collisionChecks = collisionChecks;
    m_metrics.drawCalls = drawCalls;
    m_metrics.trianglesRendered = trianglesRendered;
    
    // Update rolling averages
    m_metrics.fpsHistory.erase(m_metrics.fpsHistory.begin());
    m_metrics.fpsHistory.push_back(m_metrics.fps);
    
    m_metrics.frameTimeHistory.erase(m_metrics.frameTimeHistory.begin());
    m_metrics.frameTimeHistory.push_back(m_metrics.frameTime);
    
    // Track main loop FPS (same as console)
    m_mainLoopFrameCount += 1.0f;
    m_mainLoopTimeAccumulator += deltaTime;
    
    // Update displayed FPS every second
    m_fpsUpdateTimer += deltaTime;
    if (m_fpsUpdateTimer >= m_fpsUpdateInterval) {
        // Calculate main loop FPS (same as console)
        if (m_mainLoopTimeAccumulator > 0.0f) {
            m_mainLoopFPS = m_mainLoopFrameCount / m_mainLoopTimeAccumulator;
            m_displayedFPS = std::round(m_mainLoopFPS); // Round to nearest integer
        }
        
        // Reset counters
        m_mainLoopFrameCount = 0.0f;
        m_mainLoopTimeAccumulator = 0.0f;
        m_fpsUpdateTimer = 0.0f;
    }
    
    // Calculate frame time statistics
    if (!m_metrics.frameTimeHistory.empty()) {
        m_metrics.averageFrameTime = std::accumulate(m_metrics.frameTimeHistory.begin(), m_metrics.frameTimeHistory.end(), 0.0f) / m_metrics.frameTimeHistory.size();
        m_metrics.minFrameTime = *std::min_element(m_metrics.frameTimeHistory.begin(), m_metrics.frameTimeHistory.end());
        m_metrics.maxFrameTime = *std::max_element(m_metrics.frameTimeHistory.begin(), m_metrics.frameTimeHistory.end());
    }
    
    // Update system metrics
    updateMemoryUsage();
    updateGPUUsage();
    updateCPUUsage();
}

void FPSRenderer::updateMemoryUsage() {
    // Simplified memory usage calculation
    m_metrics.memoryUsage = m_metrics.objectCount * sizeof(void*) * 100; // Rough estimate
}

void FPSRenderer::updateGPUUsage() {
    // Simplified GPU usage calculation based on rendering load
    m_metrics.gpuUsage = std::min(100.0f, (m_metrics.drawCalls * 0.5f) + (m_metrics.trianglesRendered * 0.001f));
    m_metrics.gpuMemoryUsage = m_metrics.trianglesRendered * 32.0f; // Rough estimate: 32 bytes per triangle
}

void FPSRenderer::updateCPUUsage() {
    // Simplified CPU usage calculation based on physics load
    m_metrics.cpuUsage = std::min(100.0f, (m_metrics.objectCount * 0.1f) + (m_metrics.collisionChecks * 0.05f));
}

std::string FPSRenderer::formatNumber(float value, int decimals) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(decimals) << value;
    return oss.str();
}

std::string FPSRenderer::formatBytes(size_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unit = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unit < 3) {
        size /= 1024.0;
        unit++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << size << " " << units[unit];
    return oss.str();
}

glm::vec3 FPSRenderer::getPerformanceColor(float fps) {
    if (fps >= m_fpsTarget * 0.9f) {
        return glm::vec3(0.0f, 1.0f, 0.0f); // Green
    } else if (fps >= m_fpsTarget * 0.6f) {
        return glm::vec3(1.0f, 1.0f, 0.0f); // Yellow
    } else if (fps >= m_fpsTarget * 0.3f) {
        return glm::vec3(1.0f, 0.5f, 0.0f); // Orange
    } else {
        return glm::vec3(1.0f, 0.0f, 0.0f); // Red
    }
}

bool FPSRenderer::createShader() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec2 aPos;
        
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * vec4(aPos, 0.0, 1.0);
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        uniform vec3 textColor;
        
        void main() {
            FragColor = vec4(textColor, 0.8);
        }
    )";
    
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Check for compilation errors
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
        return false;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
        return false;
    }
    
    // Create shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        return false;
    }
    
    // Clean up shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return true;
}

// Font creation removed - using simple shape-based rendering

void FPSRenderer::renderText(const std::string& text, float x, float y, const glm::vec3& color) {
    if (!m_fontLoaded) return;
    
    float cursorX = x;
    float cursorY = y;
    float charWidth = 8.0f * m_scale;
    float charHeight = 12.0f * m_scale;
    float charSpacing = 2.0f * m_scale;
    
    // Render each character
    for (char c : text) {
        if (c == '\n') {
            cursorX = x;
            cursorY += charHeight + charSpacing;
            continue;
        }
        
        renderChar(c, cursorX, cursorY, color);
        cursorX += charWidth + charSpacing;
    }
}

void FPSRenderer::renderChar(char c, float x, float y, const glm::vec3& color) {
    float charWidth = 6.0f * m_scale;
    float charHeight = 10.0f * m_scale;
    float thickness = 1.0f * m_scale;
    
    // Set color
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "textColor"), 1, glm::value_ptr(color));
    
    // Draw character using simple rectangles
    switch (c) {
        case 'F':
            // F: top, left, middle horizontal
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x, y, thickness, charHeight); // left
            renderRect(x, y + charHeight/2, charWidth*0.6f, thickness); // middle
            break;
        case 'P':
            // P: left, top, right, middle horizontal
            renderRect(x, y, thickness, charHeight); // left
            renderRect(x, y, charWidth*0.6f, thickness); // top
            renderRect(x + charWidth*0.6f, y, thickness, charHeight/2 + thickness); // right
            renderRect(x, y + charHeight/2, charWidth*0.6f, thickness); // middle
            break;
        case 'S':
            // S: top, left top, middle, right bottom, bottom
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x, y, thickness, charHeight/2); // left top
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            renderRect(x + charWidth - thickness, y + charHeight/2, thickness, charHeight/2); // right bottom
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case ':':
            // Colon: two dots
            renderRect(x + charWidth/2 - thickness/2, y + charHeight/3, thickness, thickness);
            renderRect(x + charWidth/2 - thickness/2, y + charHeight*2/3, thickness, thickness);
            break;
        case ' ':
            // Space: nothing
            break;
        case '0':
            // 0: circle with gap
            renderRect(x, y, thickness, charHeight); // left
            renderRect(x + charWidth - thickness, y, thickness, charHeight); // right
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case '1':
            // 1: vertical line with small top
            renderRect(x + charWidth/2 - thickness/2, y, thickness, charHeight);
            renderRect(x, y, charWidth/2, thickness);
            break;
        case '2':
            // 2: top, right top, middle, left bottom, bottom
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x + charWidth - thickness, y, thickness, charHeight/2); // right top
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            renderRect(x, y + charHeight/2, thickness, charHeight/2); // left bottom
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case '3':
            // 3: top, right, middle, right, bottom
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x + charWidth - thickness, y, thickness, charHeight); // right
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case '4':
            // 4: left top, right, middle
            renderRect(x, y, thickness, charHeight/2); // left top
            renderRect(x + charWidth - thickness, y, thickness, charHeight); // right
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            break;
        case '5':
            // 5: top, left top, middle, right bottom, bottom
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x, y, thickness, charHeight/2); // left top
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            renderRect(x + charWidth - thickness, y + charHeight/2, thickness, charHeight/2); // right bottom
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case '6':
            // 6: left, top, middle, right bottom, bottom
            renderRect(x, y, thickness, charHeight); // left
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            renderRect(x + charWidth - thickness, y + charHeight/2, thickness, charHeight/2); // right bottom
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case '7':
            // 7: top, right
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x + charWidth - thickness, y, thickness, charHeight); // right
            break;
        case '8':
            // 8: left, top, right, middle, bottom
            renderRect(x, y, thickness, charHeight); // left
            renderRect(x + charWidth - thickness, y, thickness, charHeight); // right
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case '9':
            // 9: top, left top, right, middle, bottom
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x, y, thickness, charHeight/2); // left top
            renderRect(x + charWidth - thickness, y, thickness, charHeight); // right
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case '.':
            // Period: small square
            renderRect(x + charWidth/2 - thickness/2, y + charHeight - thickness, thickness, thickness);
            break;
        case 'm':
            // m: left, top, middle vertical, right
            renderRect(x, y, thickness, charHeight); // left
            renderRect(x, y, charWidth*0.3f, thickness); // top left
            renderRect(x + charWidth*0.3f, y, thickness, charHeight); // middle
            renderRect(x + charWidth*0.6f, y, charWidth*0.4f, thickness); // top right
            renderRect(x + charWidth - thickness, y, thickness, charHeight); // right
            break;
        case 's':
            // s: top, left top, middle, right bottom, bottom
            renderRect(x, y, charWidth, thickness); // top
            renderRect(x, y, thickness, charHeight/2); // left top
            renderRect(x, y + charHeight/2, charWidth, thickness); // middle
            renderRect(x + charWidth - thickness, y + charHeight/2, thickness, charHeight/2); // right bottom
            renderRect(x, y + charHeight - thickness, charWidth, thickness); // bottom
            break;
        case '%':
            // %: circle with slash
            renderRect(x, y, thickness, thickness); // top left dot
            renderRect(x + charWidth - thickness, y + charHeight - thickness, thickness, thickness); // bottom right dot
            // Simple slash approximation
            for (int i = 0; i < charHeight; i++) {
                if (i % 2 == 0) {
                    renderRect(x + i * charWidth / charHeight, y + i, thickness, thickness);
                }
            }
            break;
        default:
            // Unknown character: render a small square
            renderRect(x + charWidth/2 - thickness/2, y + charHeight/2 - thickness/2, thickness, thickness);
            break;
    }
}

void FPSRenderer::renderRect(float x, float y, float width, float height) {
    // Create rectangle vertices
    float vertices[] = {
        // First triangle
        x, y,                    // Top-left
        x + width, y,            // Top-right
        x, y + height,           // Bottom-left
        
        // Second triangle
        x + width, y,            // Top-right
        x + width, y + height,   // Bottom-right
        x, y + height            // Bottom-left
    };
    
    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    // Render
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void FPSRenderer::renderBackground(float x, float y, float width, float height, const glm::vec3& color) {
    // Create background triangles
    float vertices[] = {
        // First triangle
        x, y,                    // Top-left
        x + width, y,            // Top-right
        x, y + height,           // Bottom-left
        
        // Second triangle
        x + width, y,            // Top-right
        x + width, y + height,   // Bottom-right
        x, y + height            // Bottom-left
    };
    
    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    
    // Set background color (with transparency)
    glm::vec3 bgColor = color * 0.7f; // Make it semi-transparent
    glUniform3fv(glGetUniformLocation(m_shaderProgram, "textColor"), 1, glm::value_ptr(bgColor));
    
    // Render
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void FPSRenderer::cleanup() {
    if (m_shaderProgram) {
        glDeleteProgram(m_shaderProgram);
        m_shaderProgram = 0;
    }
    
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    
    // Font texture cleanup removed - using simple shape-based rendering
}