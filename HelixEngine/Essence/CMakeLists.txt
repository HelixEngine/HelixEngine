set(ESSENCE_NAME Essence)
set(ESSENCE_CORE_INCLUDE_PATH include)
set(ESSENCE_VULKAN_INCLUDE_PATH ${ESSENCE_CORE_INCLUDE_PATH} Vulkan/include)

set(ESSENCE_VULKAN ON)
set(ESSENCE_VULKAN_NAME Essence-Vulkan)

file(GLOB_RECURSE ESSENCE_INCLUDE "include/*.hpp")
file(GLOB_RECURSE ESSENCE_SOURCES "src/*.cpp")

add_library(${ESSENCE_NAME} STATIC ${ESSENCE_INCLUDE} ${ESSENCE_SOURCES})
target_include_directories(${ESSENCE_NAME} PUBLIC ${ESSENCE_CORE_INCLUDE_PATH})
target_link_libraries(${ESSENCE_NAME} PUBLIC ${HELIX_UTIL_NAME})

#if (LINUX)
#    # 尝试获取XDG_SESSION_TYPE环境变量
#    if (DEFINED ENV{XDG_SESSION_TYPE})
#        set(XDG_SESSION_TYPE $ENV{XDG_SESSION_TYPE})
#    endif ()
#
#    # 尝试获取WAYLAND_DISPLAY和DISPLAY环境变量
#    if (DEFINED ENV{WAYLAND_DISPLAY})
#        set(WAYLAND_DISPLAY $ENV{WAYLAND_DISPLAY})
#    endif ()
#
#    if (DEFINED ENV{DISPLAY})
#        set(DISPLAY $ENV{DISPLAY})
#    endif ()
#
#    # 判断当前使用的平台
#    if (XDG_SESSION_TYPE STREQUAL "wayland" OR DEFINED ENV{WAYLAND_DISPLAY})
#        set(WSI_TYPE "Wayland")
#    elseif (XDG_SESSION_TYPE STREQUAL "x11" OR DEFINED ENV{DISPLAY})
#        set(WSI_TYPE "XCB")
#    else ()
#        # 使用loginctl命令检查会话类型
#        find_program(LOGINCTL NAMES loginctl)
#        if (LOGINCTL)
#            execute_process(
#                    COMMAND bash -c "loginctl show-session $(loginctl | grep -E '^[0-9]+ ' | awk '{print $1}') -p Type"
#                    OUTPUT_VARIABLE SESSION_TYPE
#                    ERROR_QUIET
#                    OUTPUT_STRIP_TRAILING_WHITESPACE
#            )
#            if (SESSION_TYPE MATCHES "Type=wayland")
#                set(WSI_TYPE "Wayland")
#            elseif (SESSION_TYPE MATCHES "Type=x11")
#                set(WSI_TYPE "XCB")
#            endif ()
#        endif ()
#    endif ()
#
#    compile_definitions(ESSENCE_LINUX)
#    if (WSI_TYPE EQUAL "Wayland")
#        compile_definitions(ESSENCE_LINUX_WAYLAND)
#    elseif (WSI_TYPE EQUAL "XCB")
#        compile_definitions(ESSENCE_LINUX_XCB)
#    endif ()
#elseif (WIN32)
#    compile_definitions(ESSENCE_WIN32)
#elseif (ANDROID)
#    compile_definitions(ESSENCE_ANDROID)
#elseif (APPLE)
#    compile_definitions(ESSENCE_APPLE)
#endif ()

if (ESSENCE_VULKAN)
    find_package(Vulkan REQUIRED)

    if (WIN32 OR LINUX)
        file(GLOB_RECURSE ESSENCE_VULKAN_INCLUDE "Vulkan/include/*.hpp")
        file(GLOB_RECURSE ESSENCE_VULKAN_SOURCES "Vulkan/src/*.cpp")

        add_library(${ESSENCE_VULKAN_NAME} STATIC ${ESSENCE_VULKAN_INCLUDE} ${ESSENCE_VULKAN_SOURCES})

        target_include_directories(${ESSENCE_VULKAN_NAME} PUBLIC ${ESSENCE_VULKAN_INCLUDE_PATH})

        target_link_libraries(${ESSENCE_VULKAN_NAME} PUBLIC ${HELIX_UTIL_NAME})

        target_link_libraries(${ESSENCE_VULKAN_NAME} PUBLIC Vulkan::Vulkan)
        target_link_libraries(${ESSENCE_NAME} PRIVATE ${ESSENCE_VULKAN_NAME})
    endif ()
    #...
endif ()