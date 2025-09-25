#pragma once

#include <GLFW/glfw3.h>

// Abstract base class for all physics scenarios
class BaseScenario {
public:
    BaseScenario() = default;
    virtual ~BaseScenario() = default;
    
    // Initialize the scenario
    virtual bool initialize(GLFWwindow* window) = 0;
    
    // Update scenario logic
    virtual void update(float deltaTime) = 0;
    
    // Render the scenario
    virtual void render() = 0;
    
    // Cleanup resources
    virtual void cleanup() = 0;
    
    // Get scenario name
    virtual const char* getName() const = 0;
    
    // Get scenario description
    virtual const char* getDescription() const = 0;
};
