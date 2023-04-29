cmake_minimum_required(VERSION 3.2)
project("LiteRT")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/exec-app/)

# -- Dependence
add_subdirectory("ext")
add_subdirectory("assets")

execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_BIN_DIR} ${EXECUTABLE_OUTPUT_PATH}
)

# -- Sources
aux_source_directory(src SOURCE_FILES)
add_executable("LiteRT" main.cpp ${SOURCE_FILES} ${LRT_GLAD_SOURCE})

target_include_directories(
    "LiteRT" PUBLIC
    include/
    ${LRT_GLAD_INCLUDE_DIR}
    ${LRT_GLFW_INCLUDE_DIR}
    ${LRT_GLM_INCLUDE_DIR}
    )

if (UNIX)
    target_link_libraries(
        "LiteRT" glfw GL X11 Xrandr Xi
    )
elseif (WIN32)
    target_link_libraries(
        "LiteRT" glfw opengl32
    )
else ()
    message(FATAL_ERROR "Platform is not supported!")
endif ()