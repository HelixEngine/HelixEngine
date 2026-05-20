file(GLOB_RECURSE PREPROCESSOR_SOURCE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PREPROCESSOR_SOURCE_FILES})
add_executable(ShaderPreprocessor ${PREPROCESSOR_SOURCE_FILES})

target_include_directories(ShaderPreprocessor PUBLIC ${PROJECT_SOURCE_DIR})

set_target_properties(ShaderPreprocessor PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:ShaderPreprocessor>
)

target_link_libraries(ShaderPreprocessor PRIVATE ${PROJECT_NAME} glslang::glslang)