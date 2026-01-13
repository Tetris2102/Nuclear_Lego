cmake_minimum_required(VERSION 3.12)
project(nuclear_lego LANGUAGES CXX)

find_package(OpenMP REQUIRED)

set(PROJ_NAME nuclear_lego)

set(SOURCES
    src/vector3.cpp
    src/helpers.cpp
    src/xstable.cpp
    src/material.cpp
    src/isotopeSample.cpp
    src/data.cpp
    src/voxel.cpp
    src/world.cpp
    src/pi5_main.cpp
)

add_executable(${PROJ_NAME} ${SOURCES})

target_include_directories(${PROJ_NAME} PRIVATE include src)

if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJ_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
