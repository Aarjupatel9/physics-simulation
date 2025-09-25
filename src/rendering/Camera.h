#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

// Free-fly camera with mouse look and keyboard movement
class Camera {
public:
    Camera();
    
    // Update camera based on input
    void update(GLFWwindow* window, float deltaTime);
    
    // Get view matrix
    glm::mat4 getViewMatrix() const;
    
    // Get projection matrix
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    
    // Camera properties
    glm::vec3 getPosition() const { return m_position; }
    void setPosition(const glm::vec3& position) { m_position = position; }
    
    float getFOV() const { return m_fov; }
    void setFOV(float fov) { m_fov = fov; }
    
    // Movement settings
    void setMoveSpeed(float speed) { m_moveSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { m_mouseSensitivity = sensitivity; }
    
    // Control state
    void setControlsEnabled(bool enabled);
    bool isControlsEnabled() const { return m_controlsEnabled; }
    
    // GLFW callbacks (static) - public for external access
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    glm::vec3 m_position;
    float m_yaw;
    float m_pitch;
    float m_fov;
    
    float m_moveSpeed;
    float m_mouseSensitivity;
    
    bool m_controlsEnabled;
    bool m_firstMouse;
    double m_lastMouseX;
    double m_lastMouseY;
    
    glm::vec3 computeFront() const;
    
    // Static instance for callbacks
    static Camera* s_instance;
};
