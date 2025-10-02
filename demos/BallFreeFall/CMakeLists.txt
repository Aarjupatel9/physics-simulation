# BallFreeFall CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Create the BallFreeFall executable
add_executable(BallFreeFall
    main.cpp
    BallFreeFallScene.cpp
)

# Link against the RealityCore library
target_link_libraries(BallFreeFall RealityCore)

# Set include directories
target_include_directories(BallFreeFall PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/engine/src
)

# Set C++ standard
set_target_properties(BallFreeFall PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
