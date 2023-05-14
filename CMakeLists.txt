cmake_minimum_required (VERSION 3.2)
project ("LiteRT")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/exec-dir/)

# Dependencies
add_subdirectory ("ext")
add_subdirectory ("assets")

# - Copy Shaders & Models
execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LRT_ASSETS_SHADERS_BIN_DIR} ${EXECUTABLE_OUTPUT_PATH}/shaders
)
execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LRT_ASSETS_MODELS_BIN_DIR} ${EXECUTABLE_OUTPUT_PATH}/models
)

# Sources
aux_source_directory (src SOURCE_FILES)
add_executable ("LiteRT" main.cpp ${SOURCE_FILES} ${LRT_EXT_GLAD_SRC})

target_include_directories(
    "LiteRT" PUBLIC
    include/
    ${LRT_EXT_GLAD_INC_DIR}
    ${LRT_EXT_GLFW_INC_DIR}
    ${LRT_EXT_GLM_INC_DIR}
)

# Libraries
if (UNIX)
    target_link_libraries(
        "LiteRT" glfw GL X11 Xrandr Xi
    )
elseif (WIN32)
    target_link_libraries(
        "LiteRT" glfw opengl32
    )
else ()
    message(FATAL_ERROR "Current platfrom is not supported!")
endif ()