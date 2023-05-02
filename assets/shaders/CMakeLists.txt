# Assets: Shaders 

# -- Settings
set(ASSETS_SHADER_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(ASSETS_SHADER_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE INTERNAL "")
set(
    ASSETS_SHADER_SRC
    "RayTracing.vert"
    "RayTracing.frag"
    "CopyRes.vert"
    "CopyRes.frag"
    "Present.vert"
    "Present.frag"
)

# -- Compiling
# GLSL Compiler (options: glslangValidator / glslc)
set(GLSLC "glslangValidator")
find_program(GLSLC_FOUND NAMES ${GLSLC} REQUIRED)

IF (GLSLC_FOUND)

    function (compile_2_spirv FILE_NAME)
        execute_process(
            COMMAND "${GLSLC}" "-G" "-I${ASSETS_SHADER_INC_DIR}" "${FILE_NAME}" "-o" "${ASSETS_SHADER_BIN_DIR}/${FILE_NAME}.spirv"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULTS_VARIABLE RES_INFO
            )
        
        if (RES_INFO EQUAL "0")
            message ("-- Compiling shader: ${FILE_NAME} --> Done.")
        else(RES_INFO)
            message("-- Compiling shader: ${FILE_NAME} Error!\n${GLSLC} output: ${RES_INFO}")
        endif ()
    endfunction ()

    foreach (FILE ${ASSETS_SHADER_SRC})
        compile_2_spirv(${FILE})
    endforeach ()

ENDIF (GLSLC_FOUND)