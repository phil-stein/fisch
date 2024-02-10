cmake_minimum_required(VERSION 3.20.1)

# set(GAME "-" CACHE STRING "required!, sets the game sources to be compiled")
# set(GAME "-" CACHE STRING "" FORCE)
set(GAME "test")

set(EXT_NAME    "bovengine_external")
project(${EXT_NAME}
      VERSION 0.1
      DESCRIPTION "bovengine external dependencies")

set(LIB_NAME    "bovengine")
project(${LIB_NAME}
      VERSION 0.1
      DESCRIPTION "3d game engine")
      # LANGUAGES CXX)

set(ROOT_DIR ../..)

include(../games/test/CMakeLists.txt)
include(../games/sandbox/CMakeLists.txt)



message("building GAME: ${GAME}")
if(${GAME} STREQUAL "-")
  # message("You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
  message(FATAL_ERROR "You need to pass a GAME value, f.e. 'sandbox', i.e. -DGAME:STRING=sandbox" )
elseif(${GAME} STREQUAL "test")
  set(GAME_SOURCES ${TEST_SOURCES})
elseif(${GAME} STREQUAL "sandbox")
  set(GAME_SOURCES ${SANDBOX_SOURCES})
endif()

# compile externals seperately to disable warnings
add_library(${EXT_NAME}
               ${ROOT_DIR}/_external/GLAD/glad.c
               ${ROOT_DIR}/_external/GLAD/glad.h

               ${ROOT_DIR}/_external/zip/zip.c
               ${ROOT_DIR}/_external/zip/zip.h
               
               ${ROOT_DIR}/_external/stb/stb_ds.h
               ${ROOT_DIR}/_external/stb/stb_image.h
               
               ${ROOT_DIR}/_external/ufbx/ufbx.c
               ${ROOT_DIR}/_external/ufbx/ufbx.h
              )
add_library(${LIB_NAME}
               # -- game sources --
               ${ROOT_DIR}/games/games.h
               ${GAME_SOURCES}
               # ------------------

               ${ROOT_DIR}/src/core/core_data.c
               ${ROOT_DIR}/src/core/core_data.h
               ${ROOT_DIR}/src/core/window.c
               ${ROOT_DIR}/src/core/window.h
               ${ROOT_DIR}/src/core/program.c
               ${ROOT_DIR}/src/core/program.h
               ${ROOT_DIR}/src/core/camera.c
               ${ROOT_DIR}/src/core/camera.h
               ${ROOT_DIR}/src/core/event_sys.c
               ${ROOT_DIR}/src/core/event_sys.h
               ${ROOT_DIR}/src/core/terrain.c
               ${ROOT_DIR}/src/core/terrain.h
               ${ROOT_DIR}/src/core/threadm.c
               ${ROOT_DIR}/src/core/threadm.h

               ${ROOT_DIR}/src/core/ecs/ecs.c
               ${ROOT_DIR}/src/core/ecs/ecs.h
               ${ROOT_DIR}/src/core/ecs/ecs_scripts.h

               ${ROOT_DIR}/src/core/types/types.h
               ${ROOT_DIR}/src/core/types/entity.h
               ${ROOT_DIR}/src/core/types/cubemap.c
               ${ROOT_DIR}/src/core/types/cubemap.h
               ${ROOT_DIR}/src/core/types/framebuffer.c
               ${ROOT_DIR}/src/core/types/framebuffer.h
               ${ROOT_DIR}/src/core/types/material.c
               ${ROOT_DIR}/src/core/types/material.h
               ${ROOT_DIR}/src/core/types/mesh.c
               ${ROOT_DIR}/src/core/types/mesh.h
               ${ROOT_DIR}/src/core/types/shader.c
               ${ROOT_DIR}/src/core/types/shader.h
               ${ROOT_DIR}/src/core/types/texture.c
               ${ROOT_DIR}/src/core/types/texture.h

               ${ROOT_DIR}/src/core/debug/debug_draw.c
               ${ROOT_DIR}/src/core/debug/debug_draw.h
               ${ROOT_DIR}/src/core/debug/debug_timer.c
               ${ROOT_DIR}/src/core/debug/debug_timer.h
               ${ROOT_DIR}/src/core/debug/debug_opengl.h
               
               ${ROOT_DIR}/src/core/io/input.c
               ${ROOT_DIR}/src/core/io/input.h
               ${ROOT_DIR}/src/core/io/file_io.c
               ${ROOT_DIR}/src/core/io/file_io.h
               ${ROOT_DIR}/src/core/io/assetm.c
               ${ROOT_DIR}/src/core/io/assetm.h
               ${ROOT_DIR}/src/core/io/asset_io.c
               ${ROOT_DIR}/src/core/io/asset_io.h
               ${ROOT_DIR}/src/core/io/save_sys/save_sys.c
               ${ROOT_DIR}/src/core/io/save_sys/save_sys.h
               ${ROOT_DIR}/src/core/io/save_sys/save_sys_scene.c
               ${ROOT_DIR}/src/core/io/save_sys/save_sys_terrain.c
               ${ROOT_DIR}/src/core/io/save_sys/save_sys_structures.c

               ${ROOT_DIR}/src/core/renderer/renderer.c
               ${ROOT_DIR}/src/core/renderer/renderer.h
               ${ROOT_DIR}/src/core/renderer/renderer_direct.c
               ${ROOT_DIR}/src/core/renderer/renderer_direct.h
               ${ROOT_DIR}/src/core/renderer/renderer_extra.c
               ${ROOT_DIR}/src/core/renderer/renderer_extra.h

               # ${ROOT_DIR}/src/core/templates/entity_tags.h
               ${ROOT_DIR}/src/core/templates/entity_template.c
               ${ROOT_DIR}/src/core/templates/entity_template.h
               ${ROOT_DIR}/src/core/templates/material_template.c
               ${ROOT_DIR}/src/core/templates/material_template.h
               ${ROOT_DIR}/src/core/templates/shader_template.c
               ${ROOT_DIR}/src/core/templates/shader_template.h
               
               ${ROOT_DIR}/src/global/global.h
               ${ROOT_DIR}/src/global/io_util.h
               ${ROOT_DIR}/src/global/str_util.h

               ${ROOT_DIR}/src/math/math_color.h
               ${ROOT_DIR}/src/math/math_inc.h
               ${ROOT_DIR}/src/math/math_ivec2.h
               ${ROOT_DIR}/src/math/math_m.h
               ${ROOT_DIR}/src/math/math_mat4.h
               ${ROOT_DIR}/src/math/math_rand.h
               ${ROOT_DIR}/src/math/math_space.h
               ${ROOT_DIR}/src/math/math_test.h
               ${ROOT_DIR}/src/math/math_vec2.h
               ${ROOT_DIR}/src/math/math_vec3.h
               ${ROOT_DIR}/src/math/math_vec4.h

               ${ROOT_DIR}/src/serialization/serialization.c
               ${ROOT_DIR}/src/serialization/serialization.h
               
               ${ROOT_DIR}/src/phys/phys_collision.c
               ${ROOT_DIR}/src/phys/phys_collision.h
               ${ROOT_DIR}/src/phys/phys_debug_draw.c
               ${ROOT_DIR}/src/phys/phys_debug_draw.h
               ${ROOT_DIR}/src/phys/phys_dynamics.c
               ${ROOT_DIR}/src/phys/phys_dynamics.h
               ${ROOT_DIR}/src/phys/phys_resolution.c
               ${ROOT_DIR}/src/phys/phys_resolution.h
               ${ROOT_DIR}/src/phys/phys_types.h
               ${ROOT_DIR}/src/phys/phys_world.c
               ${ROOT_DIR}/src/phys/phys_world.h

               # ${ROOT_DIR}/src/mui/mui.c
               # ${ROOT_DIR}/src/mui/mui.h
               # ${ROOT_DIR}/src/text/text.c
               # ${ROOT_DIR}/src/text/text_codes.h
               # ${ROOT_DIR}/src/text/text_draw.c
               # ${ROOT_DIR}/src/text/text_draw.h
               # ${ROOT_DIR}/src/text/text_edit.h
               # ${ROOT_DIR}/src/text/text_inc.h
               
               # @TODO:
               # ${ROOT_DIR}/src/text2/text.c
               # ${ROOT_DIR}/src/text2/text_inc.h
               )


# use target_compile_options instead
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -std=gnu99")

target_include_directories(${EXT_NAME} PRIVATE ${ROOT_DIR}/_external)

target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/src)
target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/apps)
target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/games)
target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/_external)
# target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/_external/FREETYPE)
# target_include_directories(${LIB_NAME} PRIVATE ${ROOT_DIR}/_external/freetype2)

target_compile_features(${EXT_NAME} PRIVATE cxx_std_11)
target_compile_features(${LIB_NAME} PRIVATE cxx_std_11)

target_compile_options(${EXT_NAME} PRIVATE -g -w) # -w disables all warnings for externals
target_compile_options(${LIB_NAME} PRIVATE 
  -g -Wall 
  -DGLOBAL_DEBUG -DEDITOR -DASSETM_NO_ZIP -DDEBUG_TIMER -DDEBUG_OPENGL -DDEBUG_DRAW -DPHYS_DEBUG -DINCLUDE_PLAY_MODE 
  -DASSET_PATH=\"/Workspace/C/fisch/_assets/\" 
  -DPF_PRINT_LOCATION)

# link external files with bovengine
target_link_libraries(${LIB_NAME} ${EXT_NAME})


# if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
#   # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/libglfw3.a)
#   target_link_libraries(${LIB_NAME} ${ROOT_DIR}/_libs/win64/libfreetype.a)
#   # message("!!! -> used .a")
# else() # if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 2016 19")
#   # target_link_libraries(${EDITOR_NAME} ${ROOT_DIR}/_libs/win64/glfw3.lib)
#   target_link_libraries(${LIB_NAME} ${ROOT_DIR}/_libs/win64/freetype.lib)
#   # message("!!! -> used .lib")
# endif()



