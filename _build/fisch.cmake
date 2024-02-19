cmake_minimum_required(VERSION 3.20.1)

set(FISCH_ROOT_DIR ../..)

set(FISCH_SOURCE_FILES
               ${FISCH_ROOT_DIR}/src/core/core_data.c
               ${FISCH_ROOT_DIR}/src/core/core_data.h
               ${FISCH_ROOT_DIR}/src/core/window.c
               ${FISCH_ROOT_DIR}/src/core/window.h
               ${FISCH_ROOT_DIR}/src/core/program.c
               ${FISCH_ROOT_DIR}/src/core/program.h
               ${FISCH_ROOT_DIR}/src/core/camera.c
               ${FISCH_ROOT_DIR}/src/core/camera.h
               ${FISCH_ROOT_DIR}/src/core/event_sys.c
               ${FISCH_ROOT_DIR}/src/core/event_sys.h
               ${FISCH_ROOT_DIR}/src/core/terrain.c
               ${FISCH_ROOT_DIR}/src/core/terrain.h
               ${FISCH_ROOT_DIR}/src/core/threadm.c
               ${FISCH_ROOT_DIR}/src/core/threadm.h

               ${FISCH_ROOT_DIR}/src/core/state/state.c
               ${FISCH_ROOT_DIR}/src/core/state/state.h
               ${FISCH_ROOT_DIR}/src/core/state/state_scripts.h

               ${FISCH_ROOT_DIR}/src/core/types/types.h
               ${FISCH_ROOT_DIR}/src/core/types/entity.h
               ${FISCH_ROOT_DIR}/src/core/types/cubemap.c
               ${FISCH_ROOT_DIR}/src/core/types/cubemap.h
               ${FISCH_ROOT_DIR}/src/core/types/framebuffer.c
               ${FISCH_ROOT_DIR}/src/core/types/framebuffer.h
               ${FISCH_ROOT_DIR}/src/core/types/material.c
               ${FISCH_ROOT_DIR}/src/core/types/material.h
               ${FISCH_ROOT_DIR}/src/core/types/mesh.c
               ${FISCH_ROOT_DIR}/src/core/types/mesh.h
               ${FISCH_ROOT_DIR}/src/core/types/shader.c
               ${FISCH_ROOT_DIR}/src/core/types/shader.h
               ${FISCH_ROOT_DIR}/src/core/types/texture.c
               ${FISCH_ROOT_DIR}/src/core/types/texture.h

               ${FISCH_ROOT_DIR}/src/core/debug/debug_draw.c
               ${FISCH_ROOT_DIR}/src/core/debug/debug_draw.h
               ${FISCH_ROOT_DIR}/src/core/debug/debug_timer.c
               ${FISCH_ROOT_DIR}/src/core/debug/debug_timer.h
               ${FISCH_ROOT_DIR}/src/core/debug/debug_opengl.h
               
               ${FISCH_ROOT_DIR}/src/core/io/input.c
               ${FISCH_ROOT_DIR}/src/core/io/input.h
               ${FISCH_ROOT_DIR}/src/core/io/file_io.c
               ${FISCH_ROOT_DIR}/src/core/io/file_io.h
               ${FISCH_ROOT_DIR}/src/core/io/assetm.c
               ${FISCH_ROOT_DIR}/src/core/io/assetm.h
               ${FISCH_ROOT_DIR}/src/core/io/asset_io.c
               ${FISCH_ROOT_DIR}/src/core/io/asset_io.h
               ${FISCH_ROOT_DIR}/src/core/io/save_sys/save_sys.c
               ${FISCH_ROOT_DIR}/src/core/io/save_sys/save_sys.h
               ${FISCH_ROOT_DIR}/src/core/io/save_sys/save_sys_scene.c
               ${FISCH_ROOT_DIR}/src/core/io/save_sys/save_sys_terrain.c
               ${FISCH_ROOT_DIR}/src/core/io/save_sys/save_sys_structures.c

               ${FISCH_ROOT_DIR}/src/core/renderer/renderer.c
               ${FISCH_ROOT_DIR}/src/core/renderer/renderer.h
               ${FISCH_ROOT_DIR}/src/core/renderer/renderer_direct.c
               ${FISCH_ROOT_DIR}/src/core/renderer/renderer_direct.h
               ${FISCH_ROOT_DIR}/src/core/renderer/renderer_extra.c
               ${FISCH_ROOT_DIR}/src/core/renderer/renderer_extra.h

               # ${ROOT_DIR}/src/core/templates/entity_tags.h
               ${FISCH_ROOT_DIR}/src/core/templates/entity_template.c
               ${FISCH_ROOT_DIR}/src/core/templates/entity_template.h
               ${FISCH_ROOT_DIR}/src/core/templates/material_template.c
               ${FISCH_ROOT_DIR}/src/core/templates/material_template.h
               ${FISCH_ROOT_DIR}/src/core/templates/shader_template.c
               ${FISCH_ROOT_DIR}/src/core/templates/shader_template.h

               # ${ROOT_DIR}/src/core/text/text.c
               # ${ROOT_DIR}/src/core/text/text_inc.h
               
               ${FISCH_ROOT_DIR}/src/global/global.h
               ${FISCH_ROOT_DIR}/src/global/io_util.h
               ${FISCH_ROOT_DIR}/src/global/str_util.h

               ${FISCH_ROOT_DIR}/src/math/math_color.h
               ${FISCH_ROOT_DIR}/src/math/math_inc.h
               ${FISCH_ROOT_DIR}/src/math/math_ivec2.h
               ${FISCH_ROOT_DIR}/src/math/math_m.h
               ${FISCH_ROOT_DIR}/src/math/math_mat4.h
               ${FISCH_ROOT_DIR}/src/math/math_rand.h
               ${FISCH_ROOT_DIR}/src/math/math_space.h
               ${FISCH_ROOT_DIR}/src/math/math_test.h
               ${FISCH_ROOT_DIR}/src/math/math_vec2.h
               ${FISCH_ROOT_DIR}/src/math/math_vec3.h
               ${FISCH_ROOT_DIR}/src/math/math_vec4.h

               ${FISCH_ROOT_DIR}/src/serialization/serialization.c
               ${FISCH_ROOT_DIR}/src/serialization/serialization.h
               
               ${FISCH_ROOT_DIR}/src/phys/phys_collision.c
               ${FISCH_ROOT_DIR}/src/phys/phys_collision.h
               ${FISCH_ROOT_DIR}/src/phys/phys_debug_draw.c
               ${FISCH_ROOT_DIR}/src/phys/phys_debug_draw.h
               ${FISCH_ROOT_DIR}/src/phys/phys_dynamics.c
               ${FISCH_ROOT_DIR}/src/phys/phys_dynamics.h
               ${FISCH_ROOT_DIR}/src/phys/phys_resolution.c
               ${FISCH_ROOT_DIR}/src/phys/phys_resolution.h
               ${FISCH_ROOT_DIR}/src/phys/phys_ray.c
               ${FISCH_ROOT_DIR}/src/phys/phys_ray.h
               ${FISCH_ROOT_DIR}/src/phys/phys_types.h
               ${FISCH_ROOT_DIR}/src/phys/phys_world.c
               ${FISCH_ROOT_DIR}/src/phys/phys_world.h

               ${FISCH_ROOT_DIR}/src/mui/mui.c
               ${FISCH_ROOT_DIR}/src/mui/mui.h
               ${FISCH_ROOT_DIR}/src/text/text.c
               ${FISCH_ROOT_DIR}/src/text/text_codes.h
               ${FISCH_ROOT_DIR}/src/text/text_draw.c
               ${FISCH_ROOT_DIR}/src/text/text_draw.h
               ${FISCH_ROOT_DIR}/src/text/text_edit.h
               ${FISCH_ROOT_DIR}/src/text/text_inc.h
               )
