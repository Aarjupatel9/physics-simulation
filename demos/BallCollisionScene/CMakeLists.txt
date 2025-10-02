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

# Link to RealityCore library
target_link_libraries(${DEMO_NAME} PRIVATE RealityCore)

# Include directories
target_include_directories(${DEMO_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/engine/include
)

# Set C++ standard and RPATH (output directory handled by root CMakeLists.txt)
set_target_properties(${DEMO_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Set RPATH to find library in ../lib/
if(APPLE)
    set_target_properties(${DEMO_NAME} PROPERTIES
        INSTALL_RPATH "@executable_path/../lib"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
elseif(UNIX)
    set_target_properties(${DEMO_NAME} PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()
