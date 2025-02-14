cmake_minimum_required(VERSION 3.8)
project(HelixEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPERIMENTAL_CXX_MODULES ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

if (NOT DEFINED HELIX_ENGINE_TEST)
    set(HELIX_ENGINE_TEST ON)
endif ()

if (NOT DEFINED HELIX_ENGINE_QT_SDK_PATH)
    set(HELIX_ENGINE_QT_SDK_PATH "D:/Qt/6.8.1/msvc2022_64/")
endif ()

set(SDL_STATIC ON)
add_subdirectory(HelixEngine/extern/SDL3.2.4)

# 有需要的自己往里面加Qt路径
list(APPEND CMAKE_PREFIX_PATH "${HELIX_ENGINE_QT_SDK_PATH}/lib/cmake/")
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE HELIX_ENGINE_INCLUDE
        "HelixEngine/include/*.hpp"
        "HelixEngine/include/*.h")

file(GLOB_RECURSE HELIX_ENGINE_SOURCES "HelixEngine/src/*.cpp")
add_library(${PROJECT_NAME} STATIC ${HELIX_ENGINE_SOURCES} ${HELIX_ENGINE_INCLUDE})
target_include_directories(${PROJECT_NAME} PUBLIC HelixEngine/include "HelixEngine/extern/ktm/include")
target_link_libraries(${PROJECT_NAME} PUBLIC SDL3-static Qt6::Core Qt6::Widgets)

function(helix_engine_windeployqt6 qt_path target_name)
    if (WIN32)
        add_custom_target(
                COMMAND "${qt_path}/bin/windeployqt6.exe" ${CMAKE_BINARY_DIR}/${target_name}.exe
                DEPENDS ${target_name})
    endif ()
endfunction()

if (HELIX_ENGINE_TEST)
    add_executable(CommonTest test/test.cpp)
    target_link_libraries(CommonTest PRIVATE ${PROJECT_NAME} Qt6::Core Qt6::Widgets)
    helix_engine_windeployqt6(${HELIX_ENGINE_QT_SDK_PATH} CommonTest)
endif ()