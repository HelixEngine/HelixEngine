cmake_minimum_required(VERSION 3.8)
project(HelixEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT DEFINED HELIX_ENGINE_TEST)
    set(HELIX_ENGINE_TEST ON)
endif ()

set(SDL_STATIC ON)
add_subdirectory(HelixEngine/extern/SDL3.2.4)

file(GLOB_RECURSE HELIX_ENGINE_INCLUDE
        "HelixEngine/include/*.hpp"
        "HelixEngine/include/*.h")
file(GLOB_RECURSE HELIX_ENGINE_SOURCES "HelixEngine/src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${HELIX_ENGINE_SOURCES} ${HELIX_ENGINE_INCLUDE})
target_include_directories(${PROJECT_NAME} PUBLIC HelixEngine/include "HelixEngine/extern/ktm2/include")
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static)

if (HELIX_ENGINE_TEST)
    add_executable(CommonTest test/test.cpp)
    target_link_libraries(CommonTest PRIVATE ${PROJECT_NAME})
endif ()