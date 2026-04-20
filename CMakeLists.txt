cmake_minimum_required(VERSION 3.13)

project(frame_extrapolation)

find_package(Geode CONFIG REQUIRED)

add_geode_mod(
    ${frame.extrapolation.beecause-s}
    ${1.0.0}
    SOURCES main.cpp
)
