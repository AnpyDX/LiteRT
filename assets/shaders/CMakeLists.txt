# Lite RT
# - Assets.Shaders 

# -- Settings
set (GLSLC "glslangValidator") # GLSL Compiler (options: glslangValidator / glslc)
set (LRT_ASSETS_SHADERS_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (LRT_ASSETS_SHADERS_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE INTERNAL "")
execute_process (COMMAND ${CMAKE_COMMAND} -E make_directory ${LRT_ASSETS_SHADERS_BIN_DIR})

set(
    LRT_ASSETS_SHADERS_SRC
    "RayTracing.vert"
    "RayTracing.frag"
    "CopyRes.vert"
    "CopyRes.frag"
    "Present.vert"
    "Present.frag"
)


# -- Compiling
find_program (GLSLC_FOUND NAMES ${GLSLC} REQUIRED)

IF (GLSLC_FOUND)

    function (compile_2_spirv FILE_NAME)
        execute_process(
            COMMAND "${GLSLC}" "-G" "-I${LRT_ASSETS_SHADERS_INC_DIR}" "${FILE_NAME}" "-o" "${LRT_ASSETS_SHADERS_BIN_DIR}/${FILE_NAME}.spirv"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULTS_VARIABLE RES_INFO
            )
        
        if (RES_INFO EQUAL "0")
            message ("-- Compiling shader: ${FILE_NAME} --> Done.")
        else (RES_INFO)
            message("-- Compiling shader: ${FILE_NAME} Error!\n${GLSLC} output: ${RES_INFO}")
        endif ()
    endfunction ()

    foreach (FILE ${LRT_ASSETS_SHADERS_SRC})
        compile_2_spirv (${FILE})
    endforeach ()

ELSE (GLSLC_FOUND)
    message (FATAL_ERROR "Cannot found GLSL compiler! Please install glslangValidator or glslc then try again!")

ENDIF (GLSLC_FOUND)