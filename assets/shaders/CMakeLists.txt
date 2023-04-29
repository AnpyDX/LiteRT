# Assets: Shaders 
cmake_minimum_required(VERSION 3.22)
project("shaders")

# -- Settings
set(SHADER_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SHADER_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE INTERNAL "")
set(
    SHADER_SRC
    "PathTracing.vert"
    "PathTracing.frag"
)

# -- Compiling
# GLSL Compiler (options: glslangValidator / glslc)
set(GLSLC "glslangValidator")
find_program(GLSLC_FOUND NAMES ${GLSLC} REQUIRED)

IF (GLSLC_FOUND)

    function (compile_2_spirv FILE_NAME)
        execute_process(
            COMMAND "${GLSLC}" "-G" "-I${SHADER_INC_DIR}" "${FILE_NAME}" "-o" "${SHADER_BIN_DIR}/spirv-shaders/${FILE_NAME}.spirv"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULTS_VARIABLE RES_INFO
            )
        
        if (RES_INFO EQUAL "0")
            message ("-- Compiling shader: ${FILE_NAME} --> Done.")
        else(RES_INFO)
            message("-- Compiling shader: ${FILE_NAME} Error!\n${GLSLC} output: ${RES_INFO}")
        endif ()
    endfunction ()

    foreach (FILE ${SHADER_SRC})
        compile_2_spirv(${FILE})
    endforeach ()

ENDIF (GLSLC_FOUND)