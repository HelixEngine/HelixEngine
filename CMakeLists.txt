cmake_minimum_required(VERSION 3.28)
project(HelixEngine VERSION 0.1.0)
include(CMake/fetch.cmake)
include(CMake/PackageDependency.cmake)

#MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GLAD_DEBUG ON)
endif ()
add_subdirectory(HelixEngine/extern/glad)

#add_subdirectory(HelixEngine/extern/Helicon)

installPackage(
        SDL3
        GIT_REPO https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.0
)
find_package(SDL3 REQUIRED)

#fetch_git_dependency(
#        NAME SDL3
#        GIT_REPO https://github.com/libsdl-org/SDL.git
#        GIT_TAG release-3.4.0
#)

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
fetch_git_dependency(
        NAME zlib
        GIT_REPO https://github.com/madler/zlib.git
        GIT_TAG v1.3.1
)

# 确保 ZLIB::ZLIB 存在
if(NOT TARGET ZLIB::ZLIB)
    if(TARGET zlibstatic)
        add_library(ZLIB::ZLIB ALIAS zlibstatic)
    elseif(TARGET zlib)
        add_library(ZLIB::ZLIB ALIAS zlib)
    endif()
endif()
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

#fetch_git_dependency(
#        NAME OpenJPEG
#        GIT_REPO https://github.com/uclouvain/openjpeg.git
#        GIT_TAG v2.5.4
#)
#FetchContent_GetProperties(OpenJPEG)
#include_directories(${openjpeg_SOURCE_DIR}/src/lib/openjp2)
#include_directories(${openjpeg_BINARY_DIR}/src/lib/openjp2)

#libjpeg-turbo
add_subdirectory(HelixEngine/extern/libjpeg-turbo)

set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)
fetch_git_dependency(
        NAME PNG
        GIT_REPO https://github.com/pnggroup/libpng.git
        GIT_TAG v1.6.55
)
set(SKIP_INSTALL_ALL OFF CACHE BOOL "" FORCE)
FetchContent_GetProperties(PNG)
include_directories(${png_SOURCE_DIR})
include_directories(${png_BINARY_DIR})


set(SAIL_ENABLE_CODECS "png;jpeg" CACHE STRING "" FORCE)
fetch_git_dependency(
        NAME SAIL
        GIT_REPO https://github.com/HappySeaFox/sail.git
        GIT_TAG v0.9.10
)

fetch_git_dependency(
        NAME boostcore
        GIT_REPO https://github.com/boostorg/core.git
        GIT_TAG boost-1.90.0
)

fetch_git_dependency(
        NAME boostconfig
        GIT_REPO https://github.com/boostorg/config.git
        GIT_TAG boost-1.90.0
)

fetch_git_dependency(
        NAME boostassert
        GIT_REPO https://github.com/boostorg/assert.git
        GIT_TAG boost-1.90.0
)

fetch_git_dependency(
        NAME boostcontainer_hash
        GIT_REPO https://github.com/boostorg/container_hash.git
        GIT_TAG boost-1.90.0
)

fetch_git_dependency(
        NAME boostdescribe
        GIT_REPO https://github.com/boostorg/describe.git
        GIT_TAG boost-1.90.0
)

fetch_git_dependency(
        NAME boostthrow_exception
        GIT_REPO https://github.com/boostorg/throw_exception.git
        GIT_TAG boost-1.90.0
)

fetch_git_dependency(
        NAME boostmp11
        GIT_REPO https://github.com/boostorg/mp11.git
        GIT_TAG boost-1.90.0
)

fetch_git_dependency(
        NAME Helicon
        GIT_REPO https://github.com/CoronaEngine/Helicon.git
)
include_directories(
        ${SAIL_BINARY_DIR}/include
)

#SAIL-C++
#find_package(SailC++ CONFIG REQUIRED)

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
target_include_directories(${PROJECT_NAME} PUBLIC HelixEngine/include "HelixEngine/extern/ktm/include")
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3 glad png_static sail-c++ Helicon ${HELIX_UTIL_NAME})

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