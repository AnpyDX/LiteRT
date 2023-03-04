cmake_minimum_required(VERSION 3.2)
project("LiteRT")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory("ext")

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
elseif (APPLE)
    message(FATAL_ERROR "macOS is not supported!")
endif()