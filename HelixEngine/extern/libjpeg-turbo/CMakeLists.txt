include(ExternalProject)
include(FetchContent)

# 声明要下载的 GitHub 项目
FetchContent_Declare(
        download_jpeg
        GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
        GIT_TAG 3.1.3
)

# 仅填充（下载）源代码，但不执行添加和构建
FetchContent_GetProperties(download_jpeg)
if(NOT download_jpeg_POPULATED)
    # 这步会执行 clone 或下载，但不会调用 add_subdirectory
    FetchContent_Populate(download_jpeg)
endif(NOT download_jpeg_POPULATED)

if (NOT DEFINED HELIX_LIBJPEG_TURBO_PACKAGE OR NOT HELIX_LIBJPEG_TURBO_PACKAGE)

    if (NOT download_jpeg_BINARY_DIR)
        set(download_jpeg_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif (NOT download_jpeg_BINARY_DIR)

    execute_process(
            COMMAND ${CMAKE_COMMAND}
            -S ${download_jpeg_SOURCE_DIR}
            -B ${download_jpeg_BINARY_DIR}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${download_jpeg_BINARY_DIR}/install
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND_ERROR_IS_FATAL ANY
    )

    execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${download_jpeg_BINARY_DIR} --parallel
            COMMAND_ERROR_IS_FATAL ANY
    )

    execute_process(
            COMMAND ${CMAKE_COMMAND} --install ${download_jpeg_BINARY_DIR}
            COMMAND_ERROR_IS_FATAL ANY
    )

    set(HELIX_LIBJPEG_TURBO_PACKAGE ON CACHE BOOL "" FORCE)
endif (NOT DEFINED HELIX_LIBJPEG_TURBO_PACKAGE OR NOT HELIX_LIBJPEG_TURBO_PACKAGE)

list(APPEND CMAKE_PREFIX_PATH ${download_jpeg_BINARY_DIR}/install)

find_package(libjpeg-turbo REQUIRED)
find_package(JPEG REQUIRED)