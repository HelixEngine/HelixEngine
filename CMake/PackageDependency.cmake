find_package(Git REQUIRED)

set(PACKAGE_DEPENDENCY_ROOT ${CMAKE_BINARY_DIR}/PackageDependency)

function(installPackage package_name)
    set(options REINSTALL_FORCE SKIP_GIT_CLONE)
    set(oneValueArgs
            GIT_REPO
            GIT_BRANCH
            GIT_TAG
    )
    set(multiValueArgs "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    string(TOUPPER ${package_name} UPPER_PACKAGE_NAME)
    set(PACKAGE_CACHE_NAME "HELIX_${UPPER_PACKAGE_NAME}_PACKAGE")
    set(PACKAGE_DIR ${PACKAGE_DEPENDENCY_ROOT}/${package_name})
    set(GIT_REPO_DIR ${PACKAGE_DIR}/GitRepo)
    set(BINARY_DIR ${PACKAGE_DIR}/Bin)
    set(INSTALL_DIR ${PACKAGE_DIR}/Install)
    set(CMAKE_PREFIX_PATH
            ${CMAKE_PREFIX_PATH} ${INSTALL_DIR}
            PARENT_SCOPE
    )
    if (NOT ARG_REINSTALL_FORCE AND DEFINED ${PACKAGE_CACHE_NAME} AND ${${PACKAGE_CACHE_NAME}})
        return()
    endif ()

    message(STATUS "Installing ${package_name} Package...")

    file(REMOVE_RECURSE ${BINARY_DIR})
    file(REMOVE_RECURSE ${INSTALL_DIR})
    file(MAKE_DIRECTORY ${GIT_REPO_DIR})
    file(MAKE_DIRECTORY ${BINARY_DIR})
    file(MAKE_DIRECTORY ${INSTALL_DIR})
    if (NOT ARG_SKIP_GIT_CLONE)
        file(REMOVE_RECURSE ${GIT_REPO_DIR})
        file(MAKE_DIRECTORY ${GIT_REPO_DIR})

        if(ARG_GIT_BRANCH AND NOT ARG_GIT_TAG)
            set(ARG_GIT_TAG ${ARG_GIT_BRANCH})
        endif()

        if(NOT ARG_GIT_TAG)
            set(ARG_GIT_TAG "HEAD")
            set(USE_DEFAULT_BRANCH TRUE)
        else()
            set(USE_DEFAULT_BRANCH FALSE)
        endif()

        if (ARG_GIT_REPO)
            string(REGEX REPLACE "^https?://github\\.com/" "" REPO_PATH ${ARG_GIT_REPO})
            string(REGEX REPLACE "^github\\.com/" "" REPO_PATH ${REPO_PATH})
            string(REGEX REPLACE "\\.git$" "" REPO_PATH ${REPO_PATH})

            if(NOT ARG_SSH_URL)
                set(ARG_SSH_URL "git@github.com:${REPO_PATH}.git")
            endif()
            if(NOT ARG_HTTPS_URL)
                set(ARG_HTTPS_URL "https://github.com/${REPO_PATH}.git")
            endif()
        endif ()

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

        macro(git_error_process operation result error)
            if(NOT ${result} EQUAL 0)
                message(FATAL_ERROR "Git ${operation} ${package_name} error: ${error}")
            endif()
        endmacro()

        macro(git_fetch connect_type)
            message(STATUS "Fetching from ${ARG_${connect_type}_URL}...")
            execute_process(
                    COMMAND ${GIT_EXECUTABLE} init
                    WORKING_DIRECTORY ${GIT_REPO_DIR}
                    RESULT_VARIABLE GIT_RESULT
                    ERROR_VARIABLE GIT_ERROR
            )
            git_error_process("init" "${GIT_RESULT}" "${GIT_ERROR}")
            execute_process(
                    COMMAND ${GIT_EXECUTABLE} remote add origin ${ARG_${connect_type}_URL}
                    WORKING_DIRECTORY ${GIT_REPO_DIR}
                    RESULT_VARIABLE GIT_RESULT
                    ERROR_VARIABLE GIT_ERROR
            )
            git_error_process("remote add" "${GIT_RESULT}" "${GIT_ERROR}")

            execute_process(
                    COMMAND ${GIT_EXECUTABLE} fetch --depth 1 --progress origin ${ARG_GIT_TAG}
                    WORKING_DIRECTORY ${GIT_REPO_DIR}
                    RESULT_VARIABLE GIT_RESULT
                    ERROR_VARIABLE GIT_ERROR
            )
            git_error_process("fetch" "${GIT_RESULT}" "${GIT_ERROR}")

            execute_process(
                    COMMAND ${GIT_EXECUTABLE} checkout FETCH_HEAD
                    WORKING_DIRECTORY ${GIT_REPO_DIR}
                    RESULT_VARIABLE GIT_RESULT
                    ERROR_VARIABLE GIT_ERROR
            )
            git_error_process("checkout" "${GIT_RESULT}" "${GIT_ERROR}")

        endmacro()

        set(FETCH_SUCCESS OFF)

        if (TRY_SSH AND ARG_SSH_URL AND NOT FETCH_SUCCESS)
            git_fetch("SSH")
            set(FETCH_SUCCESS ON)
        endif ()

        if(ARG_HTTPS_URL AND NOT FETCH_SUCCESS)
            git_fetch("HTTPS")
            set(FETCH_SUCCESS ON)
        endif ()
    endif ()

    execute_process(
            COMMAND ${CMAKE_COMMAND}
            -S ${GIT_REPO_DIR}
            -B ${BINARY_DIR}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY ${BINARY_DIR}
            COMMAND_ERROR_IS_FATAL ANY
    )

    execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${BINARY_DIR} --parallel
            COMMAND_ERROR_IS_FATAL ANY
    )

    execute_process(
            COMMAND ${CMAKE_COMMAND} --install ${BINARY_DIR}
            COMMAND_ERROR_IS_FATAL ANY
    )

    message(STATUS "Successfully installed ${package_name} Package")

    set(${PACKAGE_CACHE_NAME} ON CACHE BOOL "" FORCE)
endfunction()
