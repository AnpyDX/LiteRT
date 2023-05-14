# Lite RT
# - Ext (3rd Party)

set (LRT_EXT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# -- GLAD
set (LRT_EXT_GLAD_INC_DIR ${LRT_EXT_DIR}/glad/include CACHE INTERNAL "")
set (LRT_EXT_GLAD_SRC ${LRT_EXT_DIR}/glad/src/glad.c CACHE INTERNAL "")

# -- GLFW
add_subdirectory (glfw)
option (GLFW_BUILD_EXAMPLES OFF)
option (GLFW_BUILD_TESTS OFF)
option (GLFW_BUILD_DOCS OFF)
option (GLFW_INSTALL OFF)
option (GLFW_VULKAN_STATIC OFF)
set (LRT_EXT_GLFW_INC_DIR ${LRT_EXT_DIR}/glfw/include CACHE INTERNAL "")

# -- GLM
set (LRT_EXT_GLM_INC_DIR ${LRT_EXT_DIR}/glm/include CACHE INTERNAL "")