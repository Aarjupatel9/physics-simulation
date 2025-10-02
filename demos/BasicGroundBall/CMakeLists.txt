# BasicGroundBall CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Create the BasicGroundBall executable
add_executable(BasicGroundBall
    main.cpp
    BasicGroundBallScene.cpp
)

# Link against the RealityCore library
target_link_libraries(BasicGroundBall RealityCore)

# Set include directories
target_include_directories(BasicGroundBall PRIVATE
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/engine/src
)

# Set C++ standard
set_target_properties(BasicGroundBall PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
