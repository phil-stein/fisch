#ifndef CORE_CORE_DATA_H
#define CORE_CORE_DATA_H


#include "core/audio/audio.h"
#include "global/global.h"
#include "core/io/input.h"
#include "core/types/types.h"
#include "core/camera.h"
#include "core/terrain.h"
#include "core/io/save_sys/save_sys.h"
#include "global/global_print.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

// @BUGG: including this gives error with input_ funcs
// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wparentheses"
// #include "flecs/flecs.h"
// #pragma GCC diagnostic pop

#ifdef __cplusplus
extern "C" {
#endif

// @TODO: define this in make and use in all of core
// #define INCLUDE_PLAY_MODE


// @DOC: flag containing the state set in opengl via glEnable(), glDisable() and glCullFace()
//       ! needs to be set manually, i.e. 
//         core_data->opengl_state |= OPENGL_BLEND;
//         REMOVE_FLAG(core_data->opengl_state, OPENGL_DEPTH_TEST);
typedef enum
{
  OPENGL_BLEND              = FLAG(0),
  OPENGL_WIREFRAME          = FLAG(1),
  OPENGL_DEPTH_TEST         = FLAG(2),
  OPENGL_CULL_FACE          = FLAG(3),
  OPENGL_CULL_FACE_BACK     = FLAG(4),
  OPENGL_CULL_FACE_FRONT    = FLAG(5),
  OPENGL_CUBE_MAP_SEAMLESS  = FLAG(6),

} opengl_state_flag;
INLINE void core_data_print_opengl_state_flag(opengl_state_flag state, const char* name, const char* _file, const char* _func, const int _line)
{
  PF_COLOR(PF_CYAN); _PF("%s", name); PF_STYLE_RESET(); _PF(":\n");
 
  PF_COLOR(HAS_FLAG(state, OPENGL_BLEND)             ? PF_GREEN : PF_RED); _PF("BLEND");             PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, OPENGL_WIREFRAME)         ? PF_GREEN : PF_RED); _PF("WIREFRAME");         PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, OPENGL_DEPTH_TEST)        ? PF_GREEN : PF_RED); _PF("DEPTH_TEST");        PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, OPENGL_CULL_FACE)         ? PF_GREEN : PF_RED); _PF("CULL_FACE");         PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, OPENGL_CULL_FACE_BACK)    ? PF_GREEN : PF_RED); _PF("CULL_FACE_BACK");    PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, OPENGL_CULL_FACE_FRONT)   ? PF_GREEN : PF_RED); _PF("CULL_FACE_FRONT");   PF_STYLE_RESET(); _PF(" | ");
  PF_COLOR(HAS_FLAG(state, OPENGL_CUBE_MAP_SEAMLESS) ? PF_GREEN : PF_RED); _PF("CUBE_MAP_SEAMLESS"); PF_STYLE_RESET();
  _PF("\n");
  _PF_IF_LOC(_file, _func, _line);
}
#define P_OPENGL_STATE_FLAG(v)  core_data_print_opengl_state_flag((v), #v, __FILE__, __func__, __LINE__)


// @TODO: replace phys_act & scripts_act with flag

// @DOC: maximun length of the ASSET_PATH macro defined in make
#define ASSET_PATH_MAX    256
// @DOC: maximun length of the path to the shaders aka. ASSET_PATH/shaders
#define SHADERS_PATH_MAX  ASSET_PATH_MAX
// @DOC: maximun length of the scene_name
#define SCENE_NAME_MAX    ASSET_PATH_MAX
// @DOC: maximun length of the terrain_name
#define TERRAIN_NAME_MAX  ASSET_PATH_MAX

typedef struct core_data_t
{
  bool program_quit;  // if true program shuts down
  bool is_running;    // false during init, then true 

  // -- memory --

  // defined in global/bump_alloc.h
  bump_alloc_t bump_frame;

  // -- window --

  GLFWmonitor* monitor;
  GLFWwindow*  window;

  // f32 t_last_frame;  // time it took to run the last frame
  f32  total_t;       // time since startup
  f32  time_scale;    // scale for core_data->delta_t
  f32  delta_t;       // how much time has passed since last frame, scaled by core_data->time_scale
  f32  delta_t_real;  // how much time has passed since last frame
  f32  cur_fps;       // frames per second

  // -- state --

  int* world_arr_len_ptr;        // pointer to length of world_arr_len in state.c
  int* world_dead_arr_len_ptr;   // pointer to length of world_dead_arr_len in state.c

  // -- assetm --
  
  bool use_async_asset_arrs;    // use assetm_register_...() and threadm_load_..._arr(), or load direct
  // @UNSURE: @BUGG: why isnt this an array anmore ???
  // char asset_path[ASSET_PATH_MAX];
  // char shaders_path[SHADERS_PATH_MAX];
  char* asset_path;
  char* shaders_path;
  shader_t equirect_shader;       // for rendering equirectangular images to cube maps
  shader_t irradiance_map_shader; // for rendering the irradiance map from the cube map
  shader_t prefilter_shader;      // for rendering the prefilter map from the cube map
   
  // -- camera --
  
  camera_t cam; 

  // -- save_sys --
  
  int save_sys_version;
  char scene_name[SCENE_NAME_MAX];
  char terrain_name[TERRAIN_NAME_MAX];
  // bool load_scene;
  // bool load_terrain;

  // -- renderer --

  opengl_state_flag opengl_state;

  // after renderer_update() has run this is all draw calls
  u32 draw_calls_total; 
  u32 draw_calls_shadow;     
  u32 draw_calls_deferred;
  u32 draw_calls_screen_quad;

  bool wireframe_mode_enabled;
  bool show_shadows;

  u32 brdf_lut;

  // created in core_data_init_renderer()
  framebuffer_t fb_deferred_msaa;
  framebuffer_t fb_deferred;
  framebuffer_t fb_lighting;
  framebuffer_t fb_shadow_pass;
  framebuffer_t fb_mouse_pick;
  framebuffer_t fb_outline;

  // created in core_data_init_renderer()
  shader_t basic_shader;
  shader_t shadow_shader;
  shader_t deferred_shader;
  shader_t lighting_shader;
  shader_t shadow_pass_shader;
  shader_t skybox_shader;
  shader_t post_fx_shader;
  shader_t brdf_lut_shader;       // for rendering the brdf look up texture
  shader_t mouse_pick_shader;
  shader_t fxaa_shader;
  
  cubemap_t cube_map;
  dir_light_t dir_light;
  
  
  // -- renderer_direct --
  
  // created in core_data_init_renderer()
  u32 quad_vao, quad_vbo;     // 
  int quad_mesh;
  mesh_t line_mesh;
  mesh_t triangle_mesh;

  // -- renderer_extra --
  // // @DOC: use CORE_DATA_SET_ENTITY_OUTLINE & CORE_DATA_RM_ENTITY_OUTLINE
  // //       dont write directly
  // bool                  has_outline_entity; // if outline entity is an entity
  // u64 outline_entity;     // used in renderer_extra_draw_outline()
  int outline_id; // used in renderer_extra_draw_outline()

  // -- input --

  f64 mouse_x;
  f64 mouse_y;
  f64 mouse_delta_x;
  f64 mouse_delta_y;
  
  f64 scroll_x;
  f64 scroll_y;
  f64 scroll_delta_x;
  f64 scroll_delta_y;

  // -- mui --

  struct
  {
    bool mouse_over_mui;
    u32  button_click_sound;
    f32  button_click_sound_volume;

  } mui;

  // -- audio --
  struct 
  {
    f32 clip_master_volume;
    f32 music_master_volume;
  } audio;

  // -- terrain --

#ifdef TERRAIN_ADDON
  shader_t          terrain_shader;
  int*              terrain_materials;     // idx's for assetm
  u32               terrain_materials_len; 
  terrain_chunk_t*  terrain_chunks;
  u32               terrain_chunks_len;
  f32               terrain_scl;
  f32               terrain_y_scl;
  u32               terrain_x_len;
  u32               terrain_z_len;
  u32               terrain_collider_positions_x_len;
  u32               terrain_collider_positions_z_len;
  terrain_layout_t* terrain_layout;
  u32               terrain_layout_len;
  u32               terrain_draw_dist;
  u32               terrain_cull_dist;
#endif // TERRAIN_ADDON


  // @TODO: stripping out when not defined 
  //        const doesnt work
#ifdef INCLUDE_PLAY_MODE
// #ifdef EDITOR
  bool phys_act;
  bool phys_debug_act;
  bool scripts_act;
  bool is_paused;
#else 
  bool phys_act;
  bool phys_debug_act;
  bool scripts_act;
  bool is_paused;
#endif

}core_data_t;
// #define CORE_DATA_SET_ENTITY_OUTLINE(_core_data, _e) { (_core_data)->outline_entity = (_e); (_core_data)->has_outline_entity = true; }
// #define CORE_DATA_RM_ENTITY_OUTLINE(_core_data) { (_core_data)->outline_entity = -1; (_core_data)->has_outline_entity = false; }

// @DOC: extern pointer to core_data_t in core_data.c
//       available everywhere that includes "core/core_data.h"
extern core_data_t* core_data;

// value for phys_act & scripts_act
#ifdef INCLUDE_PLAY_MODE
#define PLAY_ACT_VALUE false
#else
#define PLAY_ACT_VALUE true
#endif


#ifdef TERRAIN_ADDON
#define CORE_DATA_TERRAIN_INIT                    \
  .terrain_materials     = NULL,                  \
  .terrain_materials_len = 0,                     \
  .terrain_chunks        = NULL,                  \
  .terrain_chunks_len    = 0,                     \
  .terrain_scl           = 100,                   \
  .terrain_y_scl         = 0.02f / 256.0f,        \
  .terrain_x_len         = 100,                   \
  .terrain_z_len         = 100,                   \
  .terrain_layout        = NULL,                  \
  .terrain_layout_len    = 0,                     \
  .terrain_draw_dist     = 2,                     \
  .terrain_cull_dist     = 3,
#else  // TERRAIN_ADDON
#define CORE_DATA_TERRAIN_INIT
#endif // TERRAIN_ADDON

#define CORE_DATA_INIT()                          \
{                                                 \
  .program_quit = false,                          \
                                                  \
  .monitor = NULL,                                \
  .window  = NULL,                                \
                                                  \
  .total_t      = 0.0f,                           \
  .time_scale   = 1.0f,                           \
  .delta_t      = 0.0f,                           \
  .delta_t_real = 0.0f,                           \
  .cur_fps      = 0.0f,                           \
  .is_running   = false,                          \
                                                  \
  .world_arr_len_ptr = NULL,                      \
  .world_dead_arr_len_ptr = NULL,                 \
                                                  \
  .use_async_asset_arrs = false,                  \
  .asset_path   = "\0",                           \
  .shaders_path = "\0",                           \
                                                  \
  .save_sys_version = SAVE_SYS_VERSION,           \
  .scene_name   = "",                             \
  .terrain_name = "",                             \
                                                  \
  .cam.pos     = { 0, 0, 0 },                     \
  .cam.fov     = 45.0f,                           \
  .cam.fov_rad = 45.0f * M_PI_F / 180.0f,         \
  .cam.near_plane  = 0.1f,                        \
  .cam.far_plane   = 1000.0f,                     \
                                                  \
  .mouse_x = 0,                                   \
  .mouse_y = 0,                                   \
  .mouse_delta_x = 0,                             \
  .mouse_delta_y = 0,                             \
  .scroll_x = 0,                                  \
  .scroll_y = 0,                                  \
  .scroll_delta_x = 0,                            \
  .scroll_delta_y = 0,                            \
                                                  \
  .quad_mesh = -1,                                \
                                                  \
  .outline_id  = -1,                              \
                                                  \
  .draw_calls_total       = 0,                    \
  .draw_calls_screen_quad = 0,                    \
  .draw_calls_deferred    = 0,                    \
  .draw_calls_shadow      = 0,                    \
                                                  \
  .wireframe_mode_enabled = false,                \
  .show_shadows  = true,                          \
                                                  \
  .brdf_lut = 0,                                  \
                                                  \
  .mui.mouse_over_mui = false,                    \
  .mui.button_click_sound = AUDIO_INVALID_IDX,    \
  .mui.button_click_sound_volume = 1.0f,          \
                                                  \
  .audio.clip_master_volume  = 0.1f,              \
  .audio.music_master_volume = 0.1f,              \
                                                  \
  CORE_DATA_TERRAIN_INIT                          \
                                                  \
  .phys_act       = PLAY_ACT_VALUE,               \
  .phys_debug_act = false,                        \
  .scripts_act    = PLAY_ACT_VALUE,               \
  .is_paused      = false,                        \
                                                  \
}

/*
 * old cam stuff:
  .cam.front   = { 0, 0, -1 },                \
  .cam.up      = { 0, 1, 0 },                 \
  .cam.target  = { 0, 0, 0 },                 \
*/ 

/*  -- old stuff --
  .world_arr_len_ptr = NULL,                  \
  .world_dead_arr_len_ptr = NULL,             \
                                              \
  .t_last_frame = 0.0f,                       \
                                              \
                                              \
  .terrain_materials = NULL,                  \
  .terrain_materials_len = 0,                 \
  .terrain_chunks = NULL,                     \
  .terrain_chunks_len = 0,                    \
  .terrain_scl   = 100,                       \
  .terrain_y_scl = 0.02f / 256.0f,            \
  .terrain_x_len = 100,                       \
  .terrain_z_len = 100,                       \
  .terrain_layout = NULL,                     \
  .terrain_layout_len = 0,                    \
  .terrain_draw_dist = 2,                     \
  .terrain_cull_dist = 3,                     \
                                              \
  .phys_act       = PLAY_ACT_VALUE,           \
  .phys_debug_act = false,                    \
  .scripts_act    = PLAY_ACT_VALUE,           \
  .is_paused      = false,                    \

*/

// -- func decls --

// @DOC: sets up asset path pointers
void core_data_pre_init();
// @DOC: initialize core_data call this before any other calls to core_data
void core_data_init();

// @DOC: get pointer to core_data struct
// core_data_t* core_data_get();

typedef enum
{
  PLAY_STATE_PLAY,
  PLAY_STATE_PAUSED,
  PLAY_STATE_STOPPED

}play_state_type;

#ifdef INCLUDE_PLAY_MODE
// play or pause the game, also saving/restoring game state
void core_data_play_func();
void core_data_play_scripts_func();
void core_data_play_phys_func();
void core_data_pause_func();
void core_data_stop_func();



play_state_type core_data_get_play_state_func();
#define STR_PLAY_STATE(_ps) ((_ps) == PLAY_STATE_PLAY    ? "PLAY_STATE_PLAY"    : \
                             (_ps) == PLAY_STATE_PAUSED  ? "PLAY_STATE_PAUSED"  : \
                             (_ps) == PLAY_STATE_STOPPED ? "PLAY_STATE_STOPPED" : \
                             "unknown play_state_type")
#define P_PLAY_STATE_TYPE(_ps) PF_COLOR(PF_CYAN); _PF("%s", #_ps); PF_STYLE_RESET(); _PF(": %s\n", STR_PLAY_STATE(_ps));

// #define core_data_is_play()        core_data_is_play_func()
#define core_data_get_play_state() core_data_get_play_state_func()
#define core_data_play()           core_data_play_func()
#define core_data_play_scripts()   core_data_play_scripts_func()   
#define core_data_play_phys()      core_data_play_phys_func() 
#define core_data_pause()          core_data_pause_func()  
#define core_data_stop()           core_data_stop_func()  

#else

// #define core_data_is_play()        true 
#define core_data_get_play_state() PLAY_STATE_PLAY 
#define core_data_play()           
#define core_data_play_scripts()  
#define core_data_play_phys()     
#define core_data_pause()         
#define core_data_stop()             

#endif

#ifdef __cplusplus
} // extern c
#endif

#endif
