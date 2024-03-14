#include "template/template.h"
#include "phys/phys_collision.h"
#include "phys/phys_debug_draw.h"
#include "phys/phys_types.h"
#include "phys/phys_world.h"
#include "template/scripts.h"
#include "template/material_table.h"

#include "core/audio/audio.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "core/debug/debug_timer.h"
#include "core/mui/mui.h"
#include "core/io/assetm.h"
#include "core/debug/debug_draw.h"

#include "math/math_vec2.h"

#include "games.h"  // ./games/games.h

#include "stb/stb_ds.h"
#include <stdlib.h>

game_data_t  game_data_data = GAME_DATA_INIT();
game_data_t* game_data      = &game_data_data;

void __pre_init__()
{
  save_sys_load_scene_terrain("empty.scene", NULL);  
}
void __init__()
{
  // -- music queue --
  // audio_music_queue_start();

  // -- mui button sounds --
  core_data->mui.button_click_sound = audio_load_clip("click_01.wav", SOUND_TYPE_CLIP);
  core_data->mui.button_click_sound_volume = 1.0f;
}

void __update__()
{
  TIMER_FUNC(scripts_update());

  if (input_get_key_pressed(KEY_BACKSPACE))
  { 
    game_data->pause_menu_act = !game_data->pause_menu_act; 
    if (game_data->pause_menu_act)
    { template_pause(); }
    else
    { template_play(); }
  }

  if (game_data->pause_menu_act)
  { template_ui_pause_menu(); }
  
  mui_textf(VEC2_XY(0.8f, 0.8f), MUI_CENTER | MUI_MIDDLE, "fps: %.1f f", core_data->cur_fps); 
}

void __cleanup__()
{
  TIMER_FUNC(scripts_cleanup());
}

void __editor_update__()
{
  const f32 s = 4;
  vec3 p0 = { 0,   0, 0   };
  vec3 p1 = { 1*s, 0, 0   };
  vec3 p2 = { 2*s, 0, 0   };
  vec3 p3 = { 0,   0, 1*s };
  vec3 p4 = { 1*s, 0, 1*s };
  vec3 p5 = { 2*s, 0, 1*s };
  vec3 p6 = { 0,   0, 2*s };
  vec3 p7 = { 1*s, 0, 2*s };
  vec3 p8 = { 2*s, 0, 2*s };
  
  bool tri00_collision = false;
  bool tri01_collision = false;
  bool tri02_collision = false;
  bool tri03_collision = false;
  bool tri04_collision = false;
  bool tri05_collision = false;
  bool tri06_collision = false;
  bool tri07_collision = false;

  f32* tri00_p0 = p0;
  f32* tri00_p1 = p3;
  f32* tri00_p2 = p4;
  f32* tri01_p0 = p0;
  f32* tri01_p1 = p4;
  f32* tri01_p2 = p1;
  f32* tri02_p0 = p1;
  f32* tri02_p1 = p4;
  f32* tri02_p2 = p5;
  f32* tri03_p0 = p1;
  f32* tri03_p1 = p5;
  f32* tri03_p2 = p2;

  f32* tri04_p0 = p3;
  f32* tri04_p1 = p6;
  f32* tri04_p2 = p7;
  f32* tri05_p0 = p3;
  f32* tri05_p1 = p7;
  f32* tri05_p2 = p4;
  f32* tri06_p0 = p4;
  f32* tri06_p1 = p7;
  f32* tri06_p2 = p8;
  f32* tri07_p0 = p4;
  f32* tri07_p1 = p8;
  f32* tri07_p2 = p5;

  TIMER_START("aabb v. triangle");
  u32 obj_arr_len = 0;
  phys_obj_t* obj_arr = phys_get_obj_arr(&obj_arr_len);
  for (int i = 0; i < (int)obj_arr_len; ++i)
  {
    phys_obj_t* obj = &obj_arr[i];
    // if (PHYS_OBJ_HAS_COLLIDER(obj) && obj->collider.type == PHYS_COLLIDER_BOX)
    if (HAS_FLAG(obj->flags, PHYS_HAS_BOX))
    {
      tri00_collision = phys_collision_check_aabb_v_triangle_obj(obj, tri00_p0, tri00_p1, tri00_p2);
      tri01_collision = phys_collision_check_aabb_v_triangle_obj(obj, tri01_p0, tri01_p1, tri01_p2);
      tri02_collision = phys_collision_check_aabb_v_triangle_obj(obj, tri02_p0, tri02_p1, tri02_p2);
      tri03_collision = phys_collision_check_aabb_v_triangle_obj(obj, tri03_p0, tri03_p1, tri03_p2);
      tri04_collision = phys_collision_check_aabb_v_triangle_obj(obj, tri04_p0, tri04_p1, tri04_p2);
      tri05_collision = phys_collision_check_aabb_v_triangle_obj(obj, tri05_p0, tri05_p1, tri05_p2);
      tri06_collision = phys_collision_check_aabb_v_triangle_obj(obj, tri06_p0, tri06_p1, tri06_p2);
      tri07_collision = phys_collision_check_aabb_v_triangle_obj(obj, tri07_p0, tri07_p1, tri07_p2);

      if (tri00_collision || tri01_collision ||
          tri02_collision || tri03_collision ||
          tri04_collision || tri05_collision ||
          tri06_collision || tri07_collision )
      { 
        phys_debug_draw_collider_col(obj, RGBF(1, 0, 0)); 
        // ray v. triangle
      }
      // vec3 closest_p;
      // phys_util_closest_point_aabb_obj(obj, core_data->cam.pos, closest_p);
      // debug_draw_sphere(closest_p, 0.2f, RGBF(0, 0, 1));
    }
  }
  TIMER_STOP();

  debug_draw_triangle(tri00_p0, tri00_p1, tri00_p2, tri00_collision ? RGBF(1, 0, 0) : RGBF(0, 1, 1));
  debug_draw_triangle(tri01_p0, tri01_p1, tri01_p2, tri01_collision ? RGBF(1, 0, 0) : RGBF(0, 1, 1));
  debug_draw_triangle(tri02_p0, tri02_p1, tri02_p2, tri02_collision ? RGBF(1, 0, 0) : RGBF(0, 1, 1));
  debug_draw_triangle(tri03_p0, tri03_p1, tri03_p2, tri03_collision ? RGBF(1, 0, 0) : RGBF(0, 1, 1));
  debug_draw_triangle(tri04_p0, tri04_p1, tri04_p2, tri04_collision ? RGBF(1, 0, 0) : RGBF(0, 1, 1));
  debug_draw_triangle(tri05_p0, tri05_p1, tri05_p2, tri05_collision ? RGBF(1, 0, 0) : RGBF(0, 1, 1));
  debug_draw_triangle(tri06_p0, tri06_p1, tri06_p2, tri06_collision ? RGBF(1, 0, 0) : RGBF(0, 1, 1));
  debug_draw_triangle(tri07_p0, tri07_p1, tri07_p2, tri07_collision ? RGBF(1, 0, 0) : RGBF(0, 1, 1));

  if (input_get_mouse_pressed(MOUSE_LEFT))
  {
    ray_t ray = RAY_T_INIT_ZERO();
    ray.draw_debug = true;
    vec3 cam_front;
    camera_get_front(cam_front);
    vec3_mul_f(cam_front, 1.0f, ray.pos);
    vec3_add(core_data->cam.pos, ray.pos, ray.pos);
    vec3_copy(cam_front, ray.dir);  
    vec3_normalize(ray.dir, ray.dir); // prob. not necessary
    ray_hit_t hit;
    if ( phys_collision_ray_v_triangle(&ray, tri00_p0, tri00_p1, tri00_p2, &hit) ) 
    {
      debug_draw_line_t(core_data->cam.pos, hit.hit_point, RGBF(0, 1, 0), 4.0f);
      debug_draw_sphere_t(hit.hit_point, 0.25f, RGBF(0, 1, 0), 4.0f);
    }
    else if ( phys_collision_ray_v_triangle(&ray, tri01_p0, tri01_p1, tri01_p2, &hit) ) 
    {
      debug_draw_line_t(core_data->cam.pos, hit.hit_point, RGBF(0, 1, 0), 4.0f);
      debug_draw_sphere_t(hit.hit_point, 0.25f, RGBF(0, 1, 0), 4.0f);
    }
  }
}

void template_play()
{
  game_data->pause_menu_act = false; 
  core_data->time_scale  = 1.0f;
  game_data->run_scripts = true;
  input_center_cursor_pos();
  input_set_cursor_visible(false);
}
void template_pause()
{
  game_data->pause_menu_act = true; 
  core_data->time_scale  = 0.0f;
  game_data->run_scripts = false;
  input_center_cursor_pos();
  input_set_cursor_visible(true);
}

void template_ui_pause_menu()
{
  // -- bg --
  mui_rect_round(VEC2(0), VEC2_XY(5.0f,  3.0f),  VEC3(0.75f));
  mui_rect_round(VEC2(0), VEC2_XY(4.75f, 2.75f), VEC3(0.25f));

  // -- title --
  mui_text(VEC2_XY(0, 0.55f), MUI_CENTER | MUI_MIDDLE, "PAUSE");

  if (mui_button(VEC2_XY(0.0f, 0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "continue"))
  { template_play(); }
 
  // -- audio controls --

  u32 idx;
  char* song_path = audio_music_queue_get_current(&idx);
  char* song_name = NULL;
  u32 song_path_len = (u32)strlen(song_path);
  for (int i = (int)song_path_len -1; i >= 0; --i) 
  { if (song_path[i] == '\\' || song_path[i] == '/') { song_name = song_path +i +1; break; } }
  ASSERT(song_name != NULL);

  mui_text(VEC2_XY(0, 0.0f), MUI_CENTER | MUI_MIDDLE, song_name);
  
  static bool is_playing = true;
  if (mui_button_icon(VEC2_XY(0.0f, -0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), 
                      is_playing ? assetm_get_mesh_idx("icons/pause_icon") : assetm_get_mesh_idx("icons/play_icon")) )
  { is_playing = audio_music_queue_toggle(); }
  
  if (mui_button(VEC2_XY(-0.35f, -0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "previous song"))
  { audio_music_queue_prev(); }
  
  if (mui_button(VEC2_XY(0.35f, -0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "next song"))
  { audio_music_queue_next(); }
  
}
