cmake_minimum_required(VERSION 3.13)

# This will automatically use the "name" from your mod.json
project(frame_extrapolation)

find_package(Geode REQUIRED)

# Standard Geode mod setup
add_geode_mod(
    ${PROJECT_ID}
    ${PROJECT_VERSION}
    SOURCES main.cpp
)

# Optional: Add any extra include directories if your Module.hpp is in a specific folder
# target_include_directories(${PROJECT_ID} PUBLIC .)
