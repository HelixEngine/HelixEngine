cmake_minimum_required(VERSION 3.8)
project(HelixEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ENV{http_proxy} "http://127.0.0.1:7890")
set(ENV{https_proxy} "http://127.0.0.1:7890")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GLAD_DEBUG ON)
endif ()
add_subdirectory(HelixEngine/extern/glad)

set(SDL_STATIC ON)
add_subdirectory(HelixEngine/extern/SDL3.2.10)
add_subdirectory(HelixEngine/extern/opencv)

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
target_include_directories(${PROJECT_NAME} PUBLIC HelixEngine/include "HelixEngine/extern/ktm/include" "HelixEngine/extern/boost" "HelixEngine/extern/opencv/include")
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3-static glad opencv_core ${HELIX_UTIL_NAME})

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