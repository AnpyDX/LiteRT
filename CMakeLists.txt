cmake_minimum_required(VERSION 3.2)
project("RayTracing")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

aux_source_directory(. SOURCE_FILES)

add_executable("RayTracing" ${SOURCE_FILES})

target_include_directories(
    "RayTracing" PUBLIC
    include/
    )

target_link_directories(
    "RayTracing" PUBLIC
    lib/
    )

#target_link_libraries(
#    "RayTracing"
#
#    )
