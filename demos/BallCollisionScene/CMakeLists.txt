# BallCollisionScene CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Demo name
set(DEMO_NAME BallCollisionDemo)

# Source files
set(DEMO_SOURCES
    main.cpp
    BallCollisionScene.cpp
)

# Create executable
add_executable(${DEMO_NAME} ${DEMO_SOURCES})

# Link to PhysicsEngine library
target_link_libraries(${DEMO_NAME} PRIVATE PhysicsEngine)

# Include directories
target_include_directories(${DEMO_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/engine/include
)

# Set output directory
set_target_properties(${DEMO_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/demos
)
