cmake_minimum_required(VERSION 3.20.1)
include(../compile_options.cmake)
include(../fisch.cmake)

# set(GAME "-" CACHE STRING "required!, sets the game sources to be compiled")
# set(GAME "-" CACHE STRING "" FORCE)
set(GAME "test")

# set(EXT_NAME    "fisch_external")
# project(${EXT_NAME}
#       VERSION 0.1
#       DESCRIPTION "fisch external dependencies")

set(GAME_NAME    "fisch_game_${GAME}")
project(${GAME_NAME}
      VERSION 0.1
      DESCRIPTION "fisch game sources")

set(LIB_NAME    "fisch")
project(${LIB_NAME}
      VERSION 0.1
      DESCRIPTION "3d game engine")
      # LANGUAGES CXX)

set(FISCH_GAME_ROOT_DIR ../..)

set(ROOT_DIR ../..)
include(../games/test.cmake)
include(../games/sandbox.cmake)



message("building GAME: ${GAME}")
if(${GAME} STREQUAL "-")
  # message("You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
  message(FATAL_ERROR "You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
elseif(${GAME} STREQUAL "test")
  set(GAME_SOURCES ${TEST_SOURCES})
elseif(${GAME} STREQUAL "sandbox")
  set(GAME_SOURCES ${SANDBOX_SOURCES})
endif()

# # compile externals seperately to disable warnings
# add_library(${EXT_NAME}
#                ${FISCH_GAME_ROOT_DIR}/_external/GLAD/glad.c
#                ${FISCH_GAME_ROOT_DIR}/_external/GLAD/glad.h
# 
#                ${FISCH_GAME_ROOT_DIR}/_external/zip/zip.c
#                ${FISCH_GAME_ROOT_DIR}/_external/zip/zip.h
#                
#                ${FISCH_GAME_ROOT_DIR}/_external/stb/stb_ds.h
#                ${FISCH_GAME_ROOT_DIR}/_external/stb/stb_image.h
#                
#                ${FISCH_GAME_ROOT_DIR}/_external/ufbx/ufbx.c
#                ${FISCH_GAME_ROOT_DIR}/_external/ufbx/ufbx.h
#               )
add_library(${GAME_NAME}
               ${FISCH_GAME_ROOT_DIR}/games/games.h
               ${GAME_SOURCES}
               )
             # set(SOURCE_FILES
             #                ${FISCH_GAME_ROOT_DIR}/src/core/core_data.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/core_data.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/window.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/window.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/program.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/program.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/camera.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/camera.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/event_sys.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/event_sys.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/terrain.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/terrain.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/threadm.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/threadm.h
             # 
             #                ${FISCH_GAME_ROOT_DIR}/src/core/state/state.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/state/state.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/state/state_scripts.h
             # 
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/types.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/entity.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/cubemap.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/cubemap.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/framebuffer.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/framebuffer.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/material.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/material.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/mesh.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/mesh.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/shader.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/shader.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/texture.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/types/texture.h
             # 
             #                ${FISCH_GAME_ROOT_DIR}/src/core/debug/debug_draw.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/debug/debug_draw.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/debug/debug_timer.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/debug/debug_timer.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/debug/debug_opengl.h
             #                
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/input.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/input.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/file_io.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/file_io.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/assetm.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/assetm.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/asset_io.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/asset_io.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/save_sys/save_sys.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/save_sys/save_sys.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/save_sys/save_sys_scene.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/save_sys/save_sys_terrain.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/io/save_sys/save_sys_structures.c
             # 
             #                ${FISCH_GAME_ROOT_DIR}/src/core/renderer/renderer.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/renderer/renderer.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/renderer/renderer_direct.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/renderer/renderer_direct.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/renderer/renderer_extra.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/renderer/renderer_extra.h
             # 
             #                # ${FISCH_GAME_ROOT_DIR}/src/core/templates/entity_tags.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/templates/entity_template.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/templates/entity_template.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/templates/material_template.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/templates/material_template.h
             #                ${FISCH_GAME_ROOT_DIR}/src/core/templates/shader_template.c
             #                ${FISCH_GAME_ROOT_DIR}/src/core/templates/shader_template.h
             # 
             #                # ${FISCH_GAME_ROOT_DIR}/src/core/text/text.c
             #                # ${FISCH_GAME_ROOT_DIR}/src/core/text/text_inc.h
             #                
             #                ${FISCH_GAME_ROOT_DIR}/src/global/global.h
             #                ${FISCH_GAME_ROOT_DIR}/src/global/io_util.h
             #                ${FISCH_GAME_ROOT_DIR}/src/global/str_util.h
             # 
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_color.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_inc.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_ivec2.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_m.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_mat4.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_rand.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_space.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_test.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_vec2.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_vec3.h
             #                ${FISCH_GAME_ROOT_DIR}/src/math/math_vec4.h
             # 
             #                ${FISCH_GAME_ROOT_DIR}/src/serialization/serialization.c
             #                ${FISCH_GAME_ROOT_DIR}/src/serialization/serialization.h
             #                
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_collision.c
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_collision.h
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_debug_draw.c
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_debug_draw.h
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_dynamics.c
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_dynamics.h
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_resolution.c
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_resolution.h
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_ray.c
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_ray.h
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_types.h
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_world.c
             #                ${FISCH_GAME_ROOT_DIR}/src/phys/phys_world.h
             # 
             #                ${FISCH_GAME_ROOT_DIR}/src/mui/mui.c
             #                ${FISCH_GAME_ROOT_DIR}/src/mui/mui.h
             #                ${FISCH_GAME_ROOT_DIR}/src/text/text.c
             #                ${FISCH_GAME_ROOT_DIR}/src/text/text_codes.h
             #                ${FISCH_GAME_ROOT_DIR}/src/text/text_draw.c
             #                ${FISCH_GAME_ROOT_DIR}/src/text/text_draw.h
             #                ${FISCH_GAME_ROOT_DIR}/src/text/text_edit.h
             #                ${FISCH_GAME_ROOT_DIR}/src/text/text_inc.h
             #                )

add_library(${LIB_NAME} ${FISCH_SOURCE_FILES})


# target_include_directories(${EXT_NAME} PRIVATE ${FISCH_GAME_ROOT_DIR}/_external)

target_include_directories(${LIB_NAME}        PRIVATE ${CUSTOM_INCLUDE_DIRS})

target_include_directories(${GAME_NAME}        PRIVATE ${CUSTOM_INCLUDE_DIRS})

target_compile_features(${LIB_NAME}         PRIVATE ${CUSTOM_COMPILE_FEATURES})
target_compile_features(${GAME_NAME}        PRIVATE ${CUSTOM_COMPILE_FEATURES})

target_compile_options(${LIB_NAME}          PRIVATE ${CUSTOM_COMPILE_OPTIONS})
target_compile_options(${GAME_NAME}         PRIVATE ${CUSTOM_COMPILE_OPTIONS})
 
# link external files and game with fisch
target_link_libraries(${GAME_NAME} ${LIB_NAME})
target_link_libraries(${LIB_NAME}  ${GAME_NAME})

add_subdirectory(${FISCH_GAME_ROOT_DIR}/_build/fisch_external ${FISCH_GAME_ROOT_DIR}/_build/fisch_external)
target_link_libraries(${LIB_NAME} fisch_external)


# if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
#   # target_link_libraries(${EDITOR_NAME} ${FISCH_GAME_ROOT_DIR}/_libs/win64/libglfw3.a)
#   target_link_libraries(${LIB_NAME} ${FISCH_GAME_ROOT_DIR}/_libs/win64/libfreetype.a)
#   # message("!!! -> used .a")
# else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
#   # target_link_libraries(${EDITOR_NAME} ${FISCH_GAME_ROOT_DIR}/_libs/win64/glfw3.lib)
#   target_link_libraries(${LIB_NAME} ${FISCH_GAME_ROOT_DIR}/_libs/win64/freetype.lib)
#   # message("!!! -> used .lib")
# endif()



