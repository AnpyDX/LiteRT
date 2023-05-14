# Lite RT
# - Assets.Models

set (LRT_ASSETS_MODELS_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE INTERNAL "")
execute_process (COMMAND ${CMAKE_COMMAND} -E make_directory ${LRT_ASSETS_MODELS_BIN_DIR})

set(
    LRT_ASSETS_REQUIRED_MODELS
    "bunny.obj"
    "light.obj"
    "plane.obj"
)

# -- Copy required models
foreach (MODEL_FILE ${LRT_ASSETS_REQUIRED_MODELS})
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_FILE}
        ${LRT_ASSETS_MODELS_BIN_DIR}
    )
endforeach ()