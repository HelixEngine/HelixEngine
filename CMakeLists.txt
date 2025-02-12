cmake_minimum_required(VERSION 3.8)
project(HelixEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED HELIX_ENGINE_TEST)
    set(HELIX_ENGINE_TEST ON)
endif ()

file(GLOB_RECURSE HELIX_ENGINE_INCLUDE
        HelixEngine/src/*.ixx
        HelixEngine/src/*.hpp
        HelixEngine/src/*.h
        HelixEngine/src/*.cppm)
file(GLOB_RECURSE HELIX_ENGINE_SOURCES HelixEngine/src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${HELIX_ENGINE_SOURCES} ${HELIX_ENGINE_INCLUDE})
target_include_directories(${PROJECT_NAME} PUBLIC HelixEngine/include)

if (HELIX_ENGINE_TEST)
    add_executable(CommonTest test/test.cpp)
    target_link_libraries(CommonTest PRIVATE ${PROJECT_NAME})
endif ()