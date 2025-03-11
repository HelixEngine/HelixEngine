cmake_minimum_required(VERSION 3.8)
project(HelixEngine VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HELIX_ENGINE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/HelixEngine/include)
set(HELIX_UTIL_NAME HelixUtil)

file(GLOB HELIX_UTIL_INCLUDE HelixEngine/include/HelixEngine/Util/*.hpp)
file(GLOB HELIX_UTIL_SOURCES HelixEngine/src/Util/*.cpp)

add_library(${HELIX_UTIL_NAME} STATIC ${HELIX_UTIL_INCLUDE} ${HELIX_UTIL_SOURCES})
target_include_directories(${HELIX_UTIL_NAME} PUBLIC ${HELIX_ENGINE_INCLUDE_PATH} HelixEngine/extern/fast_io/include)

add_subdirectory(HelixEngine/Essence)

if (NOT DEFINED HELIX_ENGINE_TEST)
    set(HELIX_ENGINE_TEST ON)
endif ()

# Qt Config
if (NOT DEFINED HELIX_ENGINE_QT_SDK_PATH)
    set(HELIX_ENGINE_QT_SDK_PATH "D:/Qt/6.8.1/msvc2022_64/")
endif ()

# 有需要的自己往里面加Qt路径
list(APPEND CMAKE_PREFIX_PATH "${HELIX_ENGINE_QT_SDK_PATH}/lib/cmake/")
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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
target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Widgets Essence ${HELIX_UTIL_NAME})

function(helix_engine_windeployqt6 target_name)
    if (WIN32)
        add_custom_command(TARGET ${target_name} POST_BUILD
                COMMAND "${HELIX_ENGINE_QT_SDK_PATH}/bin/windeployqt6.exe"
                --verbose 0  # 可选参数，静默模式
                --no-compiler-runtime
                --no-quick-import
                "$<TARGET_FILE:${target_name}>"
        )
    endif ()
endfunction()

if (HELIX_ENGINE_TEST)
    add_executable(CommonTest test/test.cpp)
    target_link_libraries(CommonTest PRIVATE ${PROJECT_NAME} Essence-Vulkan)
    helix_engine_windeployqt6(CommonTest)
endif ()