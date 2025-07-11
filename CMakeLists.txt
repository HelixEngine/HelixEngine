cmake_minimum_required(VERSION 3.8)
project(HelixEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GLAD_DEBUG ON)
endif ()
add_subdirectory(HelixEngine/extern/glad)

set(SDL_STATIC ON CACHE BOOL "Enable SDL Static Library")
set(SDL_SHARED OFF CACHE BOOL "Disable SDL Shared Library")
set(SDL_EXAMPLES OFF CACHE BOOL "Disable SDL Examples")
set(SDL_TESTS OFF CACHE BOOL "Disable SDL Tests")
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Disable SDL Test Library")
set(SDL_RENDER OFF CACHE BOOL "Disable SDL Render")
add_subdirectory(HelixEngine/extern/SDL3.2.10)

find_package(SailC++ CONFIG REQUIRED)

#set(BUILD_LIST core imgcodecs CACHE STRING "List of OpenCV modules to build")
#set(BUILD_DOCS OFF CACHE BOOL "Disable docs")
#set(BUILD_EXAMPLES OFF CACHE BOOL "Disable examples")
#set(BUILD_TESTS OFF CACHE BOOL "Disable tests")
#set(BUILD_PERF_TESTS OFF CACHE BOOL "Disable performance tests")
#set(BUILD_opencv_highgui OFF CACHE BOOL "Disable highgui module")
#set(WITH_IPP OFF CACHE BOOL "Disable Intel IPP")
#set(ENABLE_LIBJPEG_TURBO_SIMD OFF CACHE BOOL "Disable libjpeg-turbo SIMD")
#add_subdirectory(HelixEngine/extern/opencv)

set(HELIX_ENGINE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/HelixEngine/include)
set(HELIX_UTIL_NAME HelixUtil)

file(GLOB HELIX_UTIL_INCLUDE HelixEngine/include/HelixEngine/Util/*.hpp)
file(GLOB HELIX_UTIL_SOURCES HelixEngine/src/Util/*.cpp)

add_library(${HELIX_UTIL_NAME} STATIC ${HELIX_UTIL_INCLUDE} ${HELIX_UTIL_SOURCES})
target_include_directories(${HELIX_UTIL_NAME} PUBLIC ${HELIX_ENGINE_INCLUDE_PATH} HelixEngine/extern/fast_io/include)

if (NOT DEFINED HELIX_ENGINE_TEST)
    set(HELIX_ENGINE_TEST ON)
endif ()

file(GLOB_RECURSE HELIX_ENGINE_INCLUDE
        "HelixEngine/include/*.hpp"
        "HelixEngine/include/*.h"
        EXCLUDE HelixEngine/include/HelixEngine/Util
)

file(GLOB_RECURSE HELIX_ENGINE_SOURCES
        "HelixEngine/src/*.cpp"
        EXCLUDE HelixEngine/src/Util
)
add_library(${PROJECT_NAME} STATIC ${HELIX_ENGINE_SOURCES} ${HELIX_ENGINE_INCLUDE})
target_include_directories(${PROJECT_NAME} PUBLIC HelixEngine/include "HelixEngine/extern/ktm/include" #"HelixEngine/extern/opencv-include"
)
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3-static glad #opencv_core opencv_imgcodecs
        SAIL::sail-c++ ${HELIX_UTIL_NAME})

#set(HELIX_VULKAN_BACKEND ON)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE HELIX_DEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE HELIX_RELEASE)
endif ()

# 后续把Vulkan Backend 的 Headers 管理完善一下
#if (HELIX_VULKAN_BACKEND)
#    find_package(Vulkan REQUIRED)
#    target_link_libraries(${PROJECT_NAME} PUBLIC Vulkan::Vulkan)
#endif ()

if (HELIX_ENGINE_TEST)
    add_executable(HelixEngineTest test/main.cpp)
    target_link_libraries(HelixEngineTest PRIVATE ${PROJECT_NAME})
endif ()