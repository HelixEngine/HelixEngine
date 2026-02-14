include(FetchContent)

# 全局选项
option(FORCE_REBUILD_DEPS "Force rebuild all dependencies" OFF)

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
    )
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "" ${ARGN})

    # 参数校验
    if(NOT ARG_NAME)
        message(FATAL_ERROR "fetch_git_dependency: NAME is required")
    endif()

    # 转换为小写（FetchContent 内部使用小写）
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

    # 生成 URL
    if(ARG_GIT_REPO)
        string(REGEX REPLACE "^https?://github\\.com/" "" REPO_PATH ${ARG_GIT_REPO})
        string(REGEX REPLACE "^github\\.com/" "" REPO_PATH ${REPO_PATH})
        string(REGEX REPLACE "\\.git$" "" REPO_PATH ${REPO_PATH})

        if(NOT ARG_SSH_URL)
            set(ARG_SSH_URL "git@github.com:${REPO_PATH}.git")
        endif()
        if(NOT ARG_HTTPS_URL)
            set(ARG_HTTPS_URL "https://github.com/${REPO_PATH}.git")
        endif()
        if(NOT USE_DEFAULT_BRANCH AND NOT ARG_ARCHIVE_URL)
            set(ARG_ARCHIVE_URL "https://github.com/${REPO_PATH}/archive/refs/tags/${ARG_GIT_TAG}.tar.gz")
        endif()
    endif()

    # 检查是否已存在（缓存）
    if(NOT FORCE_REBUILD_DEPS)
        FetchContent_GetProperties(${LOWER_NAME})
        if(${LOWER_NAME}_POPULATED)
            message(STATUS "${ARG_NAME}: Already populated at ${${LOWER_NAME}_SOURCE_DIR}, skipping")
            return()
        endif()
    endif()

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

    # 设置不自动更新
    set(FETCHCONTENT_UPDATES_DISCONNECTED_${LOWER_NAME} ON)

    # 检查成功宏（同时检查大小写）
    macro(check_success result_var)
        FetchContent_GetProperties(${ARG_NAME})
        set(UPPER_POPULATED FALSE)
        set(LOWER_POPULATED FALSE)

        if(DEFINED ${ARG_NAME}_POPULATED)
            set(UPPER_POPULATED ${${ARG_NAME}_POPULATED})
        endif()
        if(DEFINED ${LOWER_NAME}_POPULATED)
            set(LOWER_POPULATED ${${LOWER_NAME}_POPULATED})
        endif()

        if(UPPER_POPULATED OR LOWER_POPULATED)
            set(${result_var} TRUE)
        else()
            set(${result_var} FALSE)
        endif()
    endmacro()

    # 尝试获取宏
    macro(try_fetch source_type)
        message(STATUS "${ARG_NAME}: Trying ${source_type}...")
        FetchContent_MakeAvailable(${ARG_NAME})
        check_success(SUCCESS)
        if(SUCCESS)
            message(STATUS "${ARG_NAME}: Successfully fetched via ${source_type}")
            return()
        endif()
    endmacro()

    # 策略 1: SSH
    if(TRY_SSH AND ARG_SSH_URL)
        FetchContent_Declare(
                ${ARG_NAME}
                GIT_REPOSITORY ${ARG_SSH_URL}
                GIT_TAG ${ARG_GIT_TAG}
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
        )
        try_fetch("SSH")
    endif()

    # 策略 2: HTTPS
    if(ARG_HTTPS_URL)
        FetchContent_Declare(
                ${ARG_NAME}
                GIT_REPOSITORY ${ARG_HTTPS_URL}
                GIT_TAG ${ARG_GIT_TAG}
                GIT_SHALLOW TRUE
                GIT_PROGRESS TRUE
        )
        try_fetch("HTTPS")
    endif()

    # 策略 3: Archive
    if(ARG_ARCHIVE_URL AND NOT USE_DEFAULT_BRANCH)
        FetchContent_Declare(
                ${ARG_NAME}
                URL ${ARG_ARCHIVE_URL}
                DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        )
        try_fetch("Archive")
    endif()

    message(FATAL_ERROR "${ARG_NAME}: Failed to fetch from all available sources")
endfunction()