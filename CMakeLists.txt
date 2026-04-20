cmake_minimum_required(VERSION 3.13)

project(frame_extrapolation)

find_package(Geode CONFIG REQUIRED)

add_geode_mod(
    ${PROJECT_ID}
    ${PROJECT_VERSION}
    SOURCES main.cpp
)
