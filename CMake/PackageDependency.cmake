cmake_minimum_required(VERSION 3.21)

set(PACKAGE_DEPENDENCY_ROOT ${CMAKE_BINARY_DIR}/PackageDependency)
set(SUBDIRECTORY_PACKAGE "")

function(installPackageSource package_name)
    set(options "")
    set(oneValueArgs
            GIT_REPO
            GIT_BRANCH
            GIT_TAG
    )
    set(multiValueArgs CMAKE_ARGS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(ARG_UNPARSED_ARGUMENTS)
        message(WARNING "Unparsed arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()
    find_package(Git REQUIRED)

    set(PACKAGE_DIR ${PACKAGE_DEPENDENCY_ROOT}/${package_name})
    set(GIT_REPO_DIR ${PACKAGE_DIR}/GitRepo)

    file(REMOVE_RECURSE ${GIT_REPO_DIR})
    file(MAKE_DIRECTORY ${GIT_REPO_DIR})

    if(ARG_GIT_BRANCH AND NOT ARG_GIT_TAG)
        set(ARG_GIT_TAG ${ARG_GIT_BRANCH})
    endif()

    if(NOT ARG_GIT_TAG)
        message(FATAL_ERROR "No Git Tag or Branch input to clone ${ARG_GIT_REPO} for ${package_name}")
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

    macro(git_clone connect_type)
        message(STATUS "Cloning from ${ARG_${connect_type}_URL}...")
        execute_process(
                COMMAND ${GIT_EXECUTABLE} clone
                --depth 1
                --single-branch
                --branch ${ARG_GIT_TAG}
                ${ARG_${connect_type}_URL}
                ${GIT_REPO_DIR}
                WORKING_DIRECTORY ${PACKAGE_DIR}
                RESULT_VARIABLE GIT_RESULT
                ERROR_VARIABLE GIT_ERROR
        )
        if(NOT ${GIT_RESULT} EQUAL 0)
            message(FATAL_ERROR "Git clone ${package_name} error: ${GIT_ERROR}")
        endif()
        file(REMOVE_RECURSE ${GIT_REPO_DIR}/.git)
    endmacro()

    set(CLONE_SUCCESS OFF)

    if (TRY_SSH AND ARG_SSH_URL AND NOT CLONE_SUCCESS)
        git_clone("SSH")
        set(CLONE_SUCCESS ON)
    endif ()

    if(ARG_HTTPS_URL AND NOT CLONE_SUCCESS)
        git_clone("HTTPS")
        set(CLONE_SUCCESS ON)
    endif ()
endfunction()

function(installPackage package_name)
    set(options REINSTALL_FORCE SKIP_INSTALL_SOURCE AS_SUBDIRECTORY)
    set(oneValueArgs
            GIT_REPO
            GIT_BRANCH
            GIT_TAG
    )
    set(multiValueArgs CMAKE_ARGS)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(ARG_UNPARSED_ARGUMENTS)
        message(WARNING "Unparsed arguments: ${ARG_UNPARSED_ARGUMENTS}")
    endif()

    string(TOUPPER ${package_name} UPPER_PACKAGE_NAME)
    set(PACKAGE_CACHE_NAME "HELIX_${UPPER_PACKAGE_NAME}_PACKAGE")
    set(PACKAGE_DIR ${PACKAGE_DEPENDENCY_ROOT}/${package_name})
    set(GIT_REPO_DIR ${PACKAGE_DIR}/GitRepo)
    set(BINARY_DIR ${PACKAGE_DIR}/Bin)
    set(INSTALL_DIR ${PACKAGE_DIR}/Install)
    if (ARG_AS_SUBDIRECTORY)
        set(SUBDIRECTORY_PACKAGE ${SUBDIRECTORY_PACKAGE} ${GIT_REPO_DIR} PARENT_SCOPE)
    else ()
        set(CMAKE_PREFIX_PATH
                ${CMAKE_PREFIX_PATH} ${INSTALL_DIR}
                PARENT_SCOPE
        )
    endif ()
    if (NOT ARG_REINSTALL_FORCE AND DEFINED ${PACKAGE_CACHE_NAME})
        if (${${PACKAGE_CACHE_NAME}})
            if (ARG_AS_SUBDIRECTORY)
                add_subdirectory(${GIT_REPO_DIR})
            endif ()
            return()
        endif ()
    endif ()

    message(STATUS "Installing ${package_name} Package...")

    file(REMOVE_RECURSE ${BINARY_DIR})
    file(REMOVE_RECURSE ${INSTALL_DIR})
    file(MAKE_DIRECTORY ${GIT_REPO_DIR})
    file(MAKE_DIRECTORY ${BINARY_DIR})
    file(MAKE_DIRECTORY ${INSTALL_DIR})
    if (NOT ARG_SKIP_INSTALL_SOURCE)
        installPackageSource(
                ${package_name}
                GIT_REPO "${ARG_GIT_REPO}"
                GIT_TAG "${ARG_GIT_TAG}"
                GIT_BRANCH "${ARG_GIT_BRANCH}"
        )
    endif ()

    if (ARG_AS_SUBDIRECTORY)
        message(STATUS "Being added as subdirectory...")
        add_subdirectory(${GIT_REPO_DIR})
        set(SUBDIRECTORY_PACKAGE ${SUBDIRECTORY_PACKAGE} ${GIT_REPO_DIR})
    else ()
        message(STATUS "Generating Binary Package...")
        execute_process(
                COMMAND ${CMAKE_COMMAND}
                -S ${GIT_REPO_DIR}
                -B ${BINARY_DIR}
                -G ${CMAKE_GENERATOR}
                "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"
                "-DCMAKE_INSTALL_PREFIX=${INSTALL_DIR}"
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                ${ARG_CMAKE_ARGS}
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
    endif ()

    set(${PACKAGE_CACHE_NAME} ON CACHE BOOL "" FORCE)
endfunction()

function(deplyPackageRuntime target)
    if (NOT WIN32)
        return()
    endif ()

    #add_custom_command(TARGET ${target} POST_BUILD
    #        COMMAND ${CMAKE_COMMAND} -E echo "Copying Package Runtime For ${target} To  $<TARGET_FILE_DIR:${target}>:"
    #        COMMAND ${CMAKE_COMMAND} -E echo "  $<TARGET_RUNTIME_DLLS:${target}>"
    #        COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #        $<TARGET_RUNTIME_DLLS:${target}>
    #        $<TARGET_FILE_DIR:${target}>
    #        COMMAND_EXPAND_LISTS
    #)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(DUMPBIN_EXECUTABLE "${CMAKE_SOURCE_DIR}/CMake/Tool/x64/dumpbin.exe")
    else ()
        set(DUMPBIN_EXECUTABLE "${CMAKE_SOURCE_DIR}/CMake/Tool/x86/dumpbin.exe")
    endif()

    set(PACKAGE_DEPENDENCY_CMAKE ${CMAKE_CURRENT_FUNCTION_LIST_FILE})
    set(DEPS_FILE_DIR "${CMAKE_CURRENT_BINARY_DIR}/PkgDepAnalysis")
    file(TOUCH ${DEPENDENCY_OUTPUT_FILE})

    set(COPY_SCRIPT "${DEPS_FILE_DIR}/parse_deps_${target}.cmake")
    configure_file(${CMAKE_CURRENT_FUNCTION_LIST_DIR}/Tool/CopyDependencies.cmake.in ${COPY_SCRIPT} @ONLY)

    add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -DTARGET_FILE=$<TARGET_FILE:${target}> -DCOPY_DST=$<TARGET_FILE_DIR:${target}> -P "${COPY_SCRIPT}"
            VERBATIM
    )

endfunction()

# Get Dependencies from file Arg by Dumpbin
function(_get_file_dependencies file dependencies dumpbin_executable destination)
    execute_process(
            COMMAND ${dumpbin_executable} /DEPENDENTS "${file}"
            OUTPUT_VARIABLE dump_result
    )

    string(REPLACE "\n" ";" lines "${dump_result}")
    list(FILTER lines EXCLUDE REGEX "^Dump of file")
    list(TRANSFORM lines STRIP)
    list(FILTER lines INCLUDE REGEX "\\.dll$")
    set(dlls ${lines})

    list(REMOVE_DUPLICATES dlls)
    set(${dependencies} ${dlls} PARENT_SCOPE)
endfunction()

function(_find_dependency_paths
        dependencies search_paths
        dependency_paths founded_dependencies)

    set(common_system_dll "^(kernel32|user32|gdi32|advapi32|shell32|ole32|oleaut32|ntdll|winspool|ws2_32|version|setupapi|mpr|winmm|imm32|crypt32|iphlpapi|rpcrt4|secur32|shlwapi|urlmon|wininet|wintrust|psapi|dbghelp|imagehlp|wtsapi32|wow64|api-ms-win-|ext-ms-win-|msvcp|vcruntime|concrt|d3dcompiler_|ucrtbased|ucrt).*\\.dll$")
    #Avoid to find again
    set(dll_path_list "${${dependency_paths}}")
    set(dll_list "${${founded_dependencies}}")
    list(REMOVE_ITEM dependencies ${dll_list})
    foreach (dep ${dependencies})
        string(TOLOWER ${dep} lower_name)
        if (lower_name MATCHES ${common_system_dll})
            continue()
        endif ()
        foreach (path ${search_paths})
            file(GLOB_RECURSE founded_files ${path}/**/${dep})
            if (founded_files)
                list(GET founded_files 0 dll)
                list(APPEND dll_path_list ${dll})
                list(APPEND dll_list ${dep})
                break()
            endif ()
        endforeach ()
    endforeach ()

    set(${dependency_paths} ${dll_path_list} PARENT_SCOPE)
    set(${founded_dependencies} ${dll_list} PARENT_SCOPE)
endfunction()

function(_get_all_dependency_paths
        file # in
        dumpbin_executable # in
        deps_file_dst # in
        search_paths # in
        founded_deps_paths # inout
        founded_deps # inout
)
    _get_file_dependencies(${file} deps ${dumpbin_executable} ${deps_file_dst})
    set(dll_paths ${${founded_deps_paths}})
    set(dlls ${${founded_deps}})

    _find_dependency_paths("${deps}" "${search_paths}" dll_paths dlls)
    set(need_process_deps ${dll_paths})
    list(REMOVE_ITEM need_process_deps ${${founded_deps_paths}})

    set(sub_dll_paths ${dll_paths})
    set(sub_dlls ${dlls})
    foreach (sub_dep_path ${need_process_deps})
        _get_all_dependency_paths(
                ${sub_dep_path}
                ${dumpbin_executable}
                ${deps_file_dst}
                "${search_paths}"
                sub_dll_paths
                sub_dlls
        )
    endforeach ()
    set(${founded_deps_paths} "${sub_dll_paths}" PARENT_SCOPE)
    set(${founded_deps} "${sub_dlls}" PARENT_SCOPE)
endfunction()

function(_copy_all_dependencies
        file # in
        dumpbin_executable # in
        deps_file_dst # in
        copy_dst # in
        search_paths # in
)
    _get_all_dependency_paths(${file} ${dumpbin_executable} ${deps_file_dst} "${search_paths}" dll_list dll_tmp)
    message(STATUS "Founded Package Dependency DLLs: ")
    foreach (dll ${dll_list})
        message(STATUS "    ${dll}")
        get_filename_component(dll_name ${dll} NAME)
        file(COPY_FILE ${dll} ${copy_dst}/${dll_name} ONLY_IF_DIFFERENT)
    endforeach ()
    message(STATUS "Copy DLLs to ${copy_dst}")
endfunction()