# Assets: Models

set (ASSETS_MODELS_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin CACHE INTERNAL "")

set (
    ASSETS_MODELS_REQUIRED_LIST
    "bunny.obj"
    "light.obj"
    "plane.obj"
)

# -- Copy required models
foreach(MODEL_FILE ${ASSETS_MODELS_REQUIRED_LIST})
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/${MODEL_FILE}
        ${ASSETS_MODELS_BIN_DIR}
    )
endforeach()