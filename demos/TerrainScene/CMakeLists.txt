# TerrainScene CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Demo name
set(DEMO_NAME TerrainDemo)

# Source files
set(DEMO_SOURCES
    main.cpp
    TerrainScene.cpp
)

# Create executable
add_executable(${DEMO_NAME} ${DEMO_SOURCES})

# Link to RealityCore library
target_link_libraries(${DEMO_NAME} PRIVATE RealityCore)

# Include directories
target_include_directories(${DEMO_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/engine/include
)

# Set output directory
set_target_properties(${DEMO_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/demos
)
