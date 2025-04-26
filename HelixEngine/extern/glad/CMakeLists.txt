file(GLOB_RECURSE GLAD_SOURCES
        "gl4.6/*.c"
        "gl4.6/*.h"
)

file(GLOB_RECURSE GLAD_DEBUG_SOURCES
        "gl4.6-debug/*.c"
        "gl4.6-debug/*.h"
)

if (NOT DEFINED GLAD_DEBUG)
    set(GLAD_DEBUG OFF)
endif ()

if (GLAD_DEBUG)
    add_library(glad STATIC ${GLAD_DEBUG_SOURCES})
    target_include_directories(glad PUBLIC gl4.6-debug/include)
else ()
    add_library(glad STATIC ${GLAD_SOURCES})
    target_include_directories(glad PUBLIC gl4.6/include)
endif ()