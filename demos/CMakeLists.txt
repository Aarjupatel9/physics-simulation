# Demos CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Find the RealityCore library (it should be built by the engine subdirectory)
# We don't need find_package since we're building it in the same project

# Add subdirectories for each demo
add_subdirectory(BallCollisionScene)
add_subdirectory(BasicDemoScene)
add_subdirectory(TerrainScene)
add_subdirectory(BallFreeFall)
add_subdirectory(BasicGroundBall)
add_subdirectory(BallCollision2)
