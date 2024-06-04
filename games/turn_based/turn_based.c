#include "turn_based/turn_based.h"
#include "core/audio/audio.h"
#include "core/camera.h"
#include "core/io/input.h"
#include "math/math_vec2.h"
#include "math/math_vec3.h"
#include "turn_based/scripts.h"
#include "turn_based/material_table.h"

#include "core/io/save_sys/save_sys.h"
#include "core/debug/debug_timer.h"
#include "core/mui/mui.h"
#include "core/io/assetm.h"

#include "games.h"  // ./games/games.h

#include "stb/stb_ds.h"
#include <stdlib.h>

game_data_t  game_data_data = GAME_DATA_INIT();
game_data_t* game_data      = &game_data_data;

void __pre_init__()
{
  save_sys_load_scene_terrain("turn_based.scene", NULL);  // fps_01.scene

  // save_sys_load_scene_terrain("test_fps.scene", "test_fps.terrain");  
  // // // @TODO: this should be safed in .terrain
  // terrain_add_material(MATERIAL_TEMPLATE_GRASS);
  // terrain_add_material(MATERIAL_TEMPLATE_PATH);
  
  // save_sys_load_scene_terrain("phys_test.scene", NULL);  
  
  audio_load_music("Godspeed.mp3",            0.5f);
  audio_load_music("Folsom Prison Blues.mp3", 0.5f);
}
void __init__()
{
  // -- music queue --
  // audio_music_queue_start();

  // -- mui button sounds --
  core_data->mui.button_click_sound = audio_load_clip("click_01.wav", SOUND_TYPE_CLIP);
  core_data->mui.button_click_sound_volume = 1.0f;

  // -- cam position --
  vec3_copy(VEC3_XYZ(-7.85f, 18.80f, 7.90f), core_data->cam.pos);
  camera_set_pitch_yaw(-0.89f, 11.85f);
}

void __update__()
{
  // -- fps counter --
  mui_textf(VEC2_XY(0.8f, 0.8f), MUI_CENTER | MUI_MIDDLE, "fps: %.1f f", core_data->cur_fps); 

  // -- camera-controller --
  // vec3_copy(VEC3_XYZ(-7.85f, 18.80f, 7.90f), core_data->cam.pos);
  camera_set_pitch_yaw(-0.89f, 11.85f);

  const f32 speed = 10.0f * core_data->delta_t;

  vec3 front, back, left, right;
  
  camera_get_front(front);
  front[1] = 0.0f;
  vec3_normalize(front, front);
  vec3_mul_f(front, speed, front);
  vec3_mul_f(front, -1.0f, back);

  camera_get_right(left);
  vec3_mul_f(left, speed, left);
  vec3_mul_f(left, -1.0f, right);

  if ( input_get_key_down(KEY_W) ) { vec3_add(core_data->cam.pos, front, core_data->cam.pos); }
  if ( input_get_key_down(KEY_S) ) { vec3_add(core_data->cam.pos, back,  core_data->cam.pos); }
  if ( input_get_key_down(KEY_A) ) { vec3_add(core_data->cam.pos, left,  core_data->cam.pos); }
  if ( input_get_key_down(KEY_D) ) { vec3_add(core_data->cam.pos, right, core_data->cam.pos); }
}

void __editor_update__() {}

void __cleanup__()
{
}

