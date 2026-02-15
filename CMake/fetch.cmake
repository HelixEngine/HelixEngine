include(FetchContent)

# 全局选项
option(FORCE_REBUILD_DEPS "Force rebuild all dependencies" OFF)

# 设置 FetchContent 基础目录
if(NOT DEFINED FETCHCONTENT_BASE_DIR)
    set(FETCHCONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/_deps")
endif()

function(fetch_git_dependency)
    set(options "")
    set(oneValueArgs
            NAME
            GIT_REPO
            GIT_TAG
            GIT_BRANCH
            SSH_URL
            HTTPS_URL
            ARCHIVE_URL
            SOURCE_DIR
            BINARY_DIR
    )
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

    if(NOT ARG_NAME)
        message(FATAL_ERROR "fetch_git_dependency: NAME is required")
    endif()

    string(TOLOWER "${ARG_NAME}" LOWER_NAME)

    # 处理 GIT_TAG / GIT_BRANCH
    if(ARG_GIT_BRANCH AND NOT ARG_GIT_TAG)
        set(ARG_GIT_TAG ${ARG_GIT_BRANCH})
    endif()

    if(NOT ARG_GIT_TAG)
        set(ARG_GIT_TAG "HEAD")
        set(USE_DEFAULT_BRANCH TRUE)
    else()
        set(USE_DEFAULT_BRANCH FALSE)
    endif()

    # 确定目录
    if(ARG_SOURCE_DIR)
        set(FINAL_SOURCE_DIR "${ARG_SOURCE_DIR}")
    else()
        set(FINAL_SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/${LOWER_NAME}-src")
    endif()

    if(ARG_BINARY_DIR)
        set(FINAL_BINARY_DIR "${ARG_BINARY_DIR}")
    else()
        set(FINAL_BINARY_DIR "${FETCHCONTENT_BASE_DIR}/${LOWER_NAME}-build")
    endif()

    # 生成 URL
    if(ARG_GIT_REPO)
        string(REGEX REPLACE "^https?://github\\.com/" "" REPO_PATH ${ARG_GIT_REPO})
        string(REGEX REPLACE "^github\\.com/" "" REPO_PATH ${REPO_PATH})
        string(REGEX REPLACE "\\.git$" "" REPO_PATH ${REPO_PATH})

        if(NOT ARG_SSH_URL)
            set(ARG_SSH_URL "git@github.com:${REPO_PATH}.git")
        endif()
        if(NOT ARG_HTTPS_URL)
            # 修复：移除 URL 中的空格
            set(ARG_HTTPS_URL "https://github.com/${REPO_PATH}.git")
        endif()
        if(NOT USE_DEFAULT_BRANCH AND NOT ARG_ARCHIVE_URL)
            # 修复：移除 URL 中的空格
            set(ARG_ARCHIVE_URL "https://github.com/${REPO_PATH}/archive/refs/tags/${ARG_GIT_TAG}.tar.gz")
        endif()
    endif()

    # 准备目录
    file(MAKE_DIRECTORY "${FINAL_SOURCE_DIR}")
    file(MAKE_DIRECTORY "${FINAL_BINARY_DIR}")

    set(FETCHCONTENT_SOURCE_DIR_${LOWER_NAME} "${FINAL_SOURCE_DIR}" CACHE INTERNAL "" FORCE)
    set(FETCHCONTENT_BINARY_DIR_${LOWER_NAME} "${FINAL_BINARY_DIR}" CACHE INTERNAL "" FORCE)

    # 检测 SSH
    set(TRY_SSH TRUE)
    if(DEFINED ENV{CMAKE_DISABLE_SSH})
        set(TRY_SSH FALSE)
    else()
        find_program(SSH_EXECUTABLE ssh)
        if(SSH_EXECUTABLE)
            execute_process(
                    COMMAND ${SSH_EXECUTABLE} -o ConnectTimeout=5 -o BatchMode=yes git@github.com
                    RESULT_VARIABLE SSH_TEST_RESULT
                    OUTPUT_QUIET ERROR_QUIET
            )
            if(NOT SSH_TEST_RESULT EQUAL 1 AND NOT SSH_TEST_RESULT EQUAL 0)
                set(TRY_SSH FALSE)
            endif()
        else()
            set(TRY_SSH FALSE)
        endif()
    endif()

    set(FETCHCONTENT_UPDATES_DISCONNECTED_${LOWER_NAME} ON)

    macro(try_fetch source_type)
        message(STATUS "${ARG_NAME}: Trying ${source_type}...")
        FetchContent_MakeAvailable(${ARG_NAME})
        message(STATUS "${ARG_NAME}: Successfully fetched via ${source_type}")
    endmacro()

    # 策略 1: SSH
    if(TRY_SSH AND ARG_SSH_URL)
        FetchContent_Declare(
                ${ARG_NAME}
                GIT_REPOSITORY ${ARG_SSH_URL}
                GIT_TAG ${ARG_GIT_TAG}
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
                SOURCE_DIR "${FINAL_SOURCE_DIR}"
                BINARY_DIR "${FINAL_BINARY_DIR}"
                OVERRIDE_FIND_PACKAGE
                EXCLUDE_FROM_ALL
        )
        try_fetch("SSH")
        return()
    endif()

    # 策略 2: HTTPS
    if(ARG_HTTPS_URL)
        FetchContent_Declare(
                ${ARG_NAME}
                GIT_REPOSITORY ${ARG_HTTPS_URL}
                GIT_TAG ${ARG_GIT_TAG}
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
                SOURCE_DIR "${FINAL_SOURCE_DIR}"
                BINARY_DIR "${FINAL_BINARY_DIR}"
                OVERRIDE_FIND_PACKAGE
                EXCLUDE_FROM_ALL
        )
        try_fetch("HTTPS")
        return()
    endif()

    # 策略 3: Archive
    if(ARG_ARCHIVE_URL AND NOT USE_DEFAULT_BRANCH)
        FetchContent_Declare(
                ${ARG_NAME}
                URL ${ARG_ARCHIVE_URL}
                DOWNLOAD_EXTRACT_TIMESTAMP TRUE
                SOURCE_DIR "${FINAL_SOURCE_DIR}"
                BINARY_DIR "${FINAL_BINARY_DIR}"
                OVERRIDE_FIND_PACKAGE
                EXCLUDE_FROM_ALL
        )
        try_fetch("Archive")
        return()
    endif()

    message(FATAL_ERROR "${ARG_NAME}: Failed to fetch from all available sources")
endfunction()