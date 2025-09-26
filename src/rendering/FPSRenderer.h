#pragma once

#include <string>
#include <vector>
#include <chrono>
#include <iostream>
#include <map>
#include <glm/glm.hpp>

typedef unsigned int GLuint;
typedef int GLint;

class FPSRenderer {
public:
    FPSRenderer();
    ~FPSRenderer();
    
    // Initialize the FPS renderer
    bool initialize();
    
    // Update performance metrics (call every frame)
    void update(float deltaTime, int objectCount, int collisionChecks, int drawCalls = 0, int trianglesRendered = 0);
    
    // Render the performance display
    void render(const glm::mat4& view, const glm::mat4& projection);
    
    // Toggle display on/off
    void toggleDisplay() { 
        m_displayEnabled = !m_displayEnabled;
    }
    bool isDisplayEnabled() const { return m_displayEnabled; }
    
    // Set display position
    void setPosition(const glm::vec2& position) { m_position = position; }
    void setScale(float scale) { m_scale = scale; }
    
    // Performance thresholds
    void setFPSTarget(float target) { m_fpsTarget = target; }
    
private:
    // Performance tracking
    struct PerformanceMetrics {
        // Core performance
        float fps = 0.0f;
        float frameTime = 0.0f;
        float physicsTime = 0.0f;
        float renderTime = 0.0f;
        
        // System metrics
        int objectCount = 0;
        int collisionChecks = 0;
        size_t memoryUsage = 0;
        float gpuUsage = 0.0f;
        float cpuUsage = 0.0f;
        
        // Advanced metrics
        int drawCalls = 0;
        int trianglesRendered = 0;
        float gpuMemoryUsage = 0.0f;
        float averageFrameTime = 0.0f;
        float minFrameTime = 0.0f;
        float maxFrameTime = 0.0f;
        
        // Rolling averages
        std::vector<float> fpsHistory;
        std::vector<float> frameTimeHistory;
        static constexpr size_t HISTORY_SIZE = 60; // 1 second at 60 FPS
    };
    
    PerformanceMetrics m_metrics;
    
    // Display settings
    bool m_displayEnabled = false;  // Start with display disabled
    glm::vec2 m_position = glm::vec2(20.0f, 20.0f); // Top-left corner
    float m_scale = 2.0f;  // Make text larger
    float m_fpsTarget = 60.0f;
    
    // FPS update timing
    float m_fpsUpdateInterval = 1.0f; // Update FPS every 1 second
    float m_fpsUpdateTimer = 0.0f;
    float m_displayedFPS = 0.0f; // The FPS value shown on screen
    
    // Main loop FPS tracking (same as console)
    float m_mainLoopFPS = 0.0f;
    float m_mainLoopFrameCount = 0.0f;
    float m_mainLoopTimeAccumulator = 0.0f;
    
    // Timing
    std::chrono::high_resolution_clock::time_point m_lastFrameTime;
    std::chrono::high_resolution_clock::time_point m_physicsStartTime;
    std::chrono::high_resolution_clock::time_point m_renderStartTime;
    
    // Rendering
    GLuint m_shaderProgram;
    GLuint m_VAO, m_VBO;
    
    // Simple text rendering using basic shapes
    bool m_fontLoaded = false;
    
    // Helper methods
    void updateMetrics(float deltaTime, int objectCount, int collisionChecks, int drawCalls, int trianglesRendered);
    void updateMemoryUsage();
    void updateGPUUsage();
    void updateCPUUsage();
    std::string formatNumber(float value, int decimals = 1);
    std::string formatBytes(size_t bytes);
    glm::vec3 getPerformanceColor(float fps);
    
    // Rendering helpers
    bool createShader();
    void renderText(const std::string& text, float x, float y, const glm::vec3& color);
    void renderBackground(float x, float y, float width, float height, const glm::vec3& color);
    void renderChar(char c, float x, float y, const glm::vec3& color);
    void renderRect(float x, float y, float width, float height);
    
    // Cleanup
    void cleanup();
};
