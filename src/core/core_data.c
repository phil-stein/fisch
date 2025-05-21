#include "core/core_data.h"
#include "core/audio/audio.h"
#include "core/io/input.h"
#include "core/templates/shader_template.h"
#include "core/window.h"
#include "core/event_sys.h"
#include "core/renderer/renderer_extra.h"
#include "core/io/assetm.h"
#include "core/io/save_sys/save_sys.h"
#include "core/types/cubemap.h"
#include "core/debug/debug_opengl.h"
#include "core/debug/debug_timer.h"

#if EDITOR
#include "games.h"  // needed in _plax() / _stop() for __init__() / __cleanup__()
#endif


// -- func decls --
INLINE void core_data_init_renderer();


// ---- vars ----
core_data_t  core_data_data = CORE_DATA_INIT();
core_data_t* core_data = &core_data_data;

char _asset_path[ASSET_PATH_MAX];
char _shaders_path[SHADERS_PATH_MAX];

// core_data_t* core_data_get()
// {
//   TRACE();
//   return &core_data;
// }

void core_data_pre_init()
{
  TRACE();

  // initialize bump allocator
  bump_init(&core_data->bump_frame, 1024);

  core_data->asset_path = _asset_path;
  core_data->shaders_path = _shaders_path;
}
void core_data_init()
{
  TRACE();

  // // @NOTE: asset_path gets set in program_start()
  
  // // -- assetm --

  char vert_path[ASSET_PATH_MAX + 64];
  char frag_path[ASSET_PATH_MAX + 64];
  SPRINTF(ASSET_PATH_MAX + 64, vert_path, "%sshaders/cubemap/render_equirect.vert", core_data->asset_path);
  SPRINTF(ASSET_PATH_MAX + 64, frag_path, "%sshaders/cubemap/render_equirect.frag", core_data->asset_path);
  core_data->equirect_shader = shader_create_from_file(vert_path, frag_path, NULL, "equirect_render_shader");
 
  SPRINTF(ASSET_PATH_MAX + 64, frag_path, "%sshaders/cubemap/irradiance_map.frag", core_data->asset_path);
  core_data->irradiance_map_shader = shader_create_from_file(vert_path, 
					      frag_path, NULL, "irradiance_map_shader");
 
  SPRINTF(ASSET_PATH_MAX + 64, frag_path, "%sshaders/cubemap/prefilter_map.frag", core_data->asset_path);
  core_data->prefilter_shader = shader_create_from_file(vert_path, 
					      frag_path, NULL, "prefilter_shader");


  TIMER_FUNC_STATIC(core_data->cube_map = cubemap_load("#cubemaps/gothic_manor_01_2k.hdr"));
  core_data->cube_map.intensity = 1.9f;
 
  vec3_copy(VEC3_XYZ(0, 0, 0), core_data->dir_light.pos);
  vec3_copy(VEC3_XYZ(1, 1, 0), core_data->dir_light.dir);
  vec3_copy(VEC3_XYZ(1, 1, 1), core_data->dir_light.color);
  core_data->dir_light.intensity = 2.0f;
  core_data->dir_light.cast_shadow = false;
  core_data->dir_light.shadow_map_x = 0;
  core_data->dir_light.shadow_map_y = 0;

  core_data_init_renderer();

  // -- terrain --
  #ifdef TERRAIN_ADDON
  core_data->terrain_shader = assetm_create_shader_from_template(SHADER_TEMPLATE_TERRAIN);
  #endif // TERRAIN_ADDON
}

#ifdef INCLUDE_PLAY_MODE

void core_data_play_func()
{
  TRACE();

  // if (core_data_is_play()) { return; }
  if (core_data_get_play_state() == PLAY_STATE_PLAY) { return; }
  
  core_data->scripts_act = true;
  core_data->phys_act    = true;
  core_data->is_paused   = false;

#if EDITOR
  save_sys_write_scene_to_state_buffer();
  input_update(); // reset input state
  TIMER_FUNC_STATIC(__init__());  // in ./games/game.h, depends on macro wich functzioon gets called
#endif

  event_sys_trigger_play_state(PLAY_STATE_PLAY);
}

void core_data_play_scripts_func()
{
  TRACE();

  // if (core_data_is_play()) { return; }
  if (core_data_get_play_state() == PLAY_STATE_PLAY) { return; }

  core_data->scripts_act = true;
  core_data->phys_act    = false;
  core_data->is_paused   = false;

#if EDITOR
  save_sys_write_scene_to_state_buffer();
  input_update(); // reset input state
  TIMER_FUNC_STATIC(__init__());  // in ./games/game.h, depends on macro wich functzioon gets called
#endif
  
  event_sys_trigger_play_state(PLAY_STATE_PLAY);
}

void core_data_play_phys_func()
{
  TRACE();

  // if (core_data_is_play()) { return; }
  if (core_data_get_play_state() == PLAY_STATE_PLAY) { return; }
  
  core_data->scripts_act = false;
  core_data->phys_act    = true;
  core_data->is_paused   = false;

#if EDITOR
  save_sys_write_scene_to_state_buffer();
#endif

  event_sys_trigger_play_state(PLAY_STATE_PLAY);
}

void core_data_pause_func()
{
  TRACE();

  // if (core_data->is_paused || !core_data_is_play()) 
  // { return; }
  if (core_data_get_play_state() == PLAY_STATE_PAUSED) { return; }
  
  core_data->scripts_act = false;
  core_data->phys_act    = false;
  core_data->is_paused   = true;
  audio_music_queue_stop();

  event_sys_trigger_play_state(PLAY_STATE_PAUSED);
}

void core_data_stop_func()
{
  TRACE();

  // if ( !(core_data_is_play() || core_data->is_paused) ) { return; }
  if (core_data_get_play_state() == PLAY_STATE_STOPPED) { return; }

  core_data->scripts_act = false;
  core_data->phys_act    = false;
  core_data->is_paused   = false;
  audio_music_queue_stop();

  #if EDITOR
  save_sys_load_scene_from_state_buffer();
  TIMER_FUNC_STATIC(__cleanup__());  // in ./games/game.h, depends on macro wich functzioon gets called
  #endif

  event_sys_trigger_play_state(PLAY_STATE_STOPPED);
}

// bool core_data_is_play_func() { return !core_data->is_paused && (core_data->phys_act || core_data->scripts_act); }

play_state_type core_data_get_play_state_func()
{
  TRACE();

  bool is_play = !core_data->is_paused && (core_data->phys_act || core_data->scripts_act);
  return is_play ? PLAY_STATE_PLAY : 
         ( core_data->is_paused ? PLAY_STATE_PAUSED : PLAY_STATE_STOPPED );
}

#endif  // #ifdef INCLUDE_PLAY_MODE


INLINE void core_data_init_renderer()
{
  // -- primitives --
  	
  // screen quad 
	f32 quad_verts[] = 
	{ 
	// positions   // tex coords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
	};

	// screen quad VAO
	_glGenVertexArrays(1, &core_data->quad_vao);
	_glGenBuffers(1, &core_data->quad_vbo);
	_glBindVertexArray(core_data->quad_vao);
	_glBindBuffer(GL_ARRAY_BUFFER, core_data->quad_vbo);
	_glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(f32), &quad_verts, GL_STATIC_DRAW); // quad_verts is 24 long
	_glEnableVertexAttribArray(0);
	_glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)0);
	_glEnableVertexAttribArray(1);
	_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void*)(2 * sizeof(f32)));

  // quad
  core_data->quad_mesh = assetm_get_mesh_idx("quad");
  // core_data->quad_mesh = assetm_get_mesh_idx("quad_rotated");

#ifdef DEBUG_DRAW
  // line
  const f32 verts_01[] = 
  {
    // pos    normals   uvs    tangents
    0, 0, 0,  0, 0, 0,  0, 0,  0, 0, 0, 
    0, 1, 0,  0, 0, 0,  0, 0,  0, 0, 0, 
  };
  const int verts_01_len = 2 * FLOATS_PER_VERT;
  mesh_make((f32*)verts_01, (int)verts_01_len, &core_data->line_mesh);
  // triangle 
  const f32 verts_02[] = 
  {
    // pos    normals   uvs    tangents
    0, 0, 0,  0, 0, 0,  0, 0,  0, 0, 0,   
    1, 0, 0,  0, 0, 0,  0, 0,  0, 0, 0, 
    0, 1, 0,  0, 0, 0,  0, 0,  0, 0, 0, 
    0, 0, 0,  0, 0, 0,  0, 0,  0, 0, 0, 
  };
  const int verts_02_len = 4 * FLOATS_PER_VERT;
  mesh_make((f32*)verts_02, (int)verts_02_len, &core_data->triangle_mesh);
#endif // DEBUG_DRAW

  // -- framebuffers --
  // core_data->fb_deferred_msaa.type = FRAMEBUFFER_DEFERRED;
  // core_data->fb_deferred_msaa.is_msaa = true;
  // core_data->fb_deferred_msaa.samples = 4;
  // core_data->fb_deferred_msaa.width  = 0;
  // core_data->fb_deferred_msaa.height = 0;
  // core_data->fb_deferred_msaa.size_divisor = 1;
  // framebuffer_create(&core_data->fb_deferred_msaa);
  // window_set_texturebuffer_to_update_to_screen_size(&core_data->fb_deferred_msaa);

  core_data->fb_deferred.type = FRAMEBUFFER_DEFERRED;
  core_data->fb_deferred.is_msaa = false;
  core_data->fb_deferred.width  = 0;
  core_data->fb_deferred.height = 0;
  core_data->fb_deferred.size_divisor = 1;
  framebuffer_create(&core_data->fb_deferred);
  window_set_texturebuffer_to_update_to_screen_size(&core_data->fb_deferred);
   
  // core_data->fb_shadow_pass.type = FRAMEBUFFER_SINGLE_CHANNEL_F;
  // core_data->fb_shadow_pass.is_msaa = false;
  // core_data->fb_shadow_pass.width  = 0;
  // core_data->fb_shadow_pass.height = 0;
  // core_data->fb_shadow_pass.size_divisor = 1;
  // framebuffer_create(&core_data->fb_shadow_pass);
  // window_set_texturebuffer_to_update_to_screen_size(&core_data->fb_shadow_pass);  

  core_data->fb_lighting.type = FRAMEBUFFER_RGB16F;
  core_data->fb_lighting.is_msaa = false;
  core_data->fb_lighting.width  = 0;
  core_data->fb_lighting.height = 0;
  core_data->fb_lighting.size_divisor = 1;
  framebuffer_create(&core_data->fb_lighting);
  window_set_texturebuffer_to_update_to_screen_size(&core_data->fb_lighting);
 
#if defined EDITOR || defined RENDERER_EXTRA
  core_data->fb_mouse_pick.type    = FRAMEBUFFER_SINGLE_CHANNEL_F;
  core_data->fb_mouse_pick.is_msaa = false;
  core_data->fb_mouse_pick.width   = 0;
  core_data->fb_mouse_pick.height  = 0;
  core_data->fb_mouse_pick.size_divisor = 4;
  framebuffer_create(&core_data->fb_mouse_pick);
  window_set_texturebuffer_to_update_to_screen_size(&core_data->fb_mouse_pick);
	 
  core_data->fb_outline.type = FRAMEBUFFER_SINGLE_CHANNEL_F;
	core_data->fb_outline.is_msaa = false;
	core_data->fb_outline.width = 0;
	core_data->fb_outline.height = 0;
	core_data->fb_outline.size_divisor = 1;
	framebuffer_create(&core_data->fb_outline);
	window_set_texturebuffer_to_update_to_screen_size(&core_data->fb_outline);  // updates framebuffer on window resize
#endif // EDITOR || RENDERER_EXTRA

  // -- shaders --
  
  TIMER_START("shaders");

  TIMER_FUNC_STATIC(core_data->basic_shader          = assetm_create_shader_from_template(SHADER_TEMPLATE_BASIC));
  
  // TIMER_FUNC_STATIC(core_data->shadow_shader         = assetm_create_shader_from_template(SHADER_TEMPLATE_SHADOW_MAP));

  TIMER_FUNC_STATIC(core_data->deferred_shader       = assetm_create_shader_from_template(SHADER_TEMPLATE_DEFERRED));
  TIMER_FUNC_STATIC(core_data->deferred_shader_unlit = assetm_create_shader_from_template(SHADER_TEMPLATE_DEFERRED_UNLIT));
	// 
  TIMER_FUNC_STATIC(core_data->skybox_shader         = assetm_create_shader_from_template(SHADER_TEMPLATE_SKYBOX));
   
  // TIMER_FUNC_STATIC(core_data->shadow_pass_shader = assetm_create_shader_from_template(SHADER_TEMPLATE_SHADOW_PASS));
   
  TIMER_FUNC_STATIC(core_data->lighting_shader       = assetm_create_shader_from_template(SHADER_TEMPLATE_LIGHTING));

  TIMER_FUNC_STATIC(core_data->post_fx_shader        = assetm_create_shader_from_template(SHADER_TEMPLATE_POST_FX));


#if defined EDITOR || defined RENDERER_EXTRA
  TIMER_FUNC_STATIC(core_data->mouse_pick_shader     = assetm_create_shader_from_template(SHADER_TEMPLATE_MOUSE_PICK));
#endif // EDITOR || RENDERER_EXTRA
  
  // TIMER_FUNC_STATIC(core_data->fxaa_shader        = assetm_create_shader_from_template(SHADER_TEMPLATE_FXAA));

  

  // @NOTE: not needed unless calling renderer_extra_gen_brdf_lut()
  //        takes very long
  TIMER_FUNC_STATIC(core_data->brdf_lut_shader    = assetm_create_shader_from_template(SHADER_TEMPLATE_BRDF_LUT));
  char brdf_path[ASSET_PATH_MAX + 64];
  SPRINTF(ASSET_PATH_MAX + 64, brdf_path, "%stextures/#internal/brdf_lut_gen_01.tex", core_data->asset_path);
  P_STR(brdf_path); 
  core_data->brdf_lut = renderer_extra_gen_brdf_lut(brdf_path); 
  
  // core_data->brdf_lut = assetm_get_texture("#internal/brdf_lut.png", false)->handle; 
  // core_data->brdf_lut = assetm_get_texture("#internal/brdf_lut_02.png", false)->handle;
  // core_data->brdf_lut = assetm_get_texture("#internal/brdf_lut_gen_01.png", false)->handle;

  TIMER_STOP_STATIC();
}
