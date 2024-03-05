cmake_minimum_required(VERSION 3.20.1)

set(FISCH_SOURCE_FILES
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

               ${ROOT_DIR}/src/core/state/state.c
               ${ROOT_DIR}/src/core/state/state.h
               ${ROOT_DIR}/src/core/state/state_scripts.h

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
               # ${ROOT_DIR}/src/core/types/particle_sys.c
               # ${ROOT_DIR}/src/core/types/particle_sys.h

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

               ${ROOT_DIR}/src/core/templates/entity_template.c
               ${ROOT_DIR}/src/core/templates/entity_template.h
               ${ROOT_DIR}/src/core/templates/material_template.c
               ${ROOT_DIR}/src/core/templates/material_template.h
               ${ROOT_DIR}/src/core/templates/shader_template.c
               ${ROOT_DIR}/src/core/templates/shader_template.h

               ${ROOT_DIR}/src/core/mui/mui.c
               ${ROOT_DIR}/src/core/mui/mui.h

               ${ROOT_DIR}/src/core/audio/audio.c
               ${ROOT_DIR}/src/core/audio/audio.h

               # ${ROOT_DIR}/src/core/text/text.c
               # ${ROOT_DIR}/src/core/text/text_inc.h
               
               ${ROOT_DIR}/src/global/global.h
               ${ROOT_DIR}/src/global/global_types.h
               ${ROOT_DIR}/src/global/global_print.h
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
               ${ROOT_DIR}/src/phys/phys_ray.c
               ${ROOT_DIR}/src/phys/phys_ray.h
               ${ROOT_DIR}/src/phys/phys_types.h
               ${ROOT_DIR}/src/phys/phys_world.c
               ${ROOT_DIR}/src/phys/phys_world.h

               ${ROOT_DIR}/src/text/text.c
               ${ROOT_DIR}/src/text/text_codes.h
               ${ROOT_DIR}/src/text/text_draw.c
               ${ROOT_DIR}/src/text/text_draw.h
               ${ROOT_DIR}/src/text/text_edit.h
               ${ROOT_DIR}/src/text/text_inc.h
               )
