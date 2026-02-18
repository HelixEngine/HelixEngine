cmake_minimum_required(VERSION 3.28)
project(HelixEngine VERSION 0.1.0)
include(CMake/PackageDependency.cmake)

#MESSAGE(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(GLAD_DEBUG ON)
endif ()
add_subdirectory(HelixEngine/extern/glad)

installPackage(
        SDL3
        GIT_REPO https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.0
)
find_package(SDL3 REQUIRED)

installPackage(
        zlib
        GIT_REPO https://github.com/madler/zlib.git
        GIT_TAG v1.3.1
)
find_package(ZLIB REQUIRED)

installPackage(
        OpenJPEG
        GIT_REPO https://github.com/uclouvain/openjpeg.git
        GIT_TAG v2.5.4
)
find_package(OpenJPEG REQUIRED)

installPackage(
        libjpeg_turbo
        GIT_REPO https://github.com/libjpeg-turbo/libjpeg-turbo.git
        GIT_TAG 3.1.3
)
find_package(libjpeg-turbo REQUIRED)
find_package(JPEG REQUIRED)

installPackage(
        libpng
        GIT_REPO https://github.com/pnggroup/libpng.git
        GIT_TAG v1.6.55
)
find_package(PNG REQUIRED)

installPackage(
        sail
        GIT_REPO https://github.com/HappySeaFox/sail.git
        GIT_TAG v0.9.10
        CMAKE_ARGS
        -DSAIL_COMBINE_CODECS=ON
        -DSAIL_BUILD_APPS=OFF
        -DSAIL_BUILD_EXAMPLES=OFF
)
find_package(SailC++ REQUIRED)

installPackage(
        boostcore
        GIT_REPO https://github.com/boostorg/core.git
        GIT_TAG boost-1.90.0
        AS_SUBDIRECTORY
)

installPackage(
        boostconfig
        GIT_REPO https://github.com/boostorg/config.git
        GIT_TAG boost-1.90.0
        AS_SUBDIRECTORY
)

installPackage(
        boostassert
        GIT_REPO https://github.com/boostorg/assert.git
        GIT_TAG boost-1.90.0
        AS_SUBDIRECTORY
)

installPackage(
        boostcontainer_hash
        GIT_REPO https://github.com/boostorg/container_hash.git
        GIT_TAG boost-1.90.0
        AS_SUBDIRECTORY
)

installPackage(
        boostdescribe
        GIT_REPO https://github.com/boostorg/describe.git
        GIT_TAG boost-1.90.0
        AS_SUBDIRECTORY
)

installPackage(
        boostthrow_exception
        GIT_REPO https://github.com/boostorg/throw_exception.git
        GIT_TAG boost-1.90.0
        AS_SUBDIRECTORY
)

installPackage(
        boostmp11
        GIT_REPO https://github.com/boostorg/mp11.git
        GIT_TAG boost-1.90.0
        AS_SUBDIRECTORY
)

installPackage(
        Helicon
        GIT_REPO https://github.com/CoronaEngine/Helicon.git
        GIT_BRANCH main
        AS_SUBDIRECTORY
)

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
target_link_libraries(${PROJECT_NAME} PUBLIC
        SDL3::SDL3 # Media Libraries
        glad # Render Libraries
        SAIL::sail-c++ # C++ Image Processing Library
        openjp2 libjpeg-turbo::jpeg PNG::PNG # SAIL codecs
        Helicon # Embedded Shader Library
        ${HELIX_UTIL_NAME} # Util For HelixEngine
)

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
    deplyPackageRuntime(HelixEngineTest)
    helicon_install_runtime_deps(HelixEngineTest)

    add_custom_command(
            TARGET HelixEngineTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/test/Resource
            $<TARGET_FILE_DIR:HelixEngineTest>/Resource
    )
endif ()