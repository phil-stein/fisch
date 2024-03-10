#include "test/test.h"
#include "core/audio/audio.h"
#include "core/io/input.h"
#include "math/math_vec2.h"
#include "test/scripts.h"

#include "core/io/save_sys/save_sys.h"
#include "core/debug/debug_timer.h"
#include "core/mui/mui.h"
#include "core/io/assetm.h"

#include "games.h"  // ./games/games.h

#include "stb/stb_ds.h"

game_data_t  game_data_data = GAME_DATA_INIT();
game_data_t* game_data      = &game_data_data;


void __pre_init__()
{
  save_sys_load_scene_terrain("test_fps.scene", NULL);  
  // save_sys_load_scene_terrain("phys_test.scene", NULL);  
  // // @TODO: this should be safed in .terrain
  // terrain_add_material(MATERIAL_TEMPLATE_GRASS);
  // terrain_add_material(MATERIAL_TEMPLATE_PATH);
}
void __init__()
{
  // // in game will be done by camera-controller
  // vec3_copy(VEC3_XYZ(0.0f,   6.0f,  10.0f), core_data->cam.pos);
  // vec3_copy(VEC3_XYZ(0.0f,  -0.15f, -1.0f), core_data->cam.front);

  // @NOTE: scripts_init() replaced by calling scripts_init...() in SCRIPT_ADD()
  // TIMER_FUNC_STATIC(scripts_init());

  
  // P_V(core_data->mui.button_click_sound);
  // core_data->mui.button_click_sound = audio_load_audio("click_01.wav", SOUND_TYPE_CLIP, 1.0f);
  // core_data->mui.button_click_sound_volume = 1.0f;
  // P_V(core_data->mui.button_click_sound);
  
  // -- music queue --
  audio_load_music("Godspeed.mp3",            0.5f);
  audio_load_music("Folsom Prison Blues.mp3", 0.5f);
  audio_music_queue_start();

  P_V(core_data->mui.button_click_sound);
  core_data->mui.button_click_sound = audio_load_clip("click_01.wav", SOUND_TYPE_CLIP);
  core_data->mui.button_click_sound_volume = 1.0f;
  P_V(core_data->mui.button_click_sound);
}

void __update__()
{
  TIMER_FUNC(scripts_update());

  if (input_get_key_pressed(KEY_BACKSPACE))
  { 
    game_data->pause_menu_act = !game_data->pause_menu_act; 
    if (game_data->pause_menu_act)
    { test_pause(); }
    else
    { test_play(); }
  }

  if (game_data->pause_menu_act)
  { test_ui_pause_menu(); }
}

void __cleanup__()
{
}

void test_play()
{
  game_data->pause_menu_act = false; 
  core_data->time_scale  = 1.0f;
  game_data->run_scripts = true;
  input_center_cursor_pos();
  input_set_cursor_visible(false);
}
void test_pause()
{
  game_data->pause_menu_act = true; 
  core_data->time_scale  = 0.0f;
  game_data->run_scripts = false;
  input_center_cursor_pos();
  input_set_cursor_visible(true);
}

void test_ui_pause_menu()
{
  // bg
  // mui_rect(VEC2(0), VEC2(5), VEC3(0.75f));
  mui_shape(VEC2(0), VEC2_XY(5.0f,  3.0f),  VEC3(0.75f), MUI_OBJ_SHAPE_RECT_ROUND, true);
  mui_shape(VEC2(0), VEC2_XY(4.75f, 2.75f), VEC3(0.25f), MUI_OBJ_SHAPE_RECT_ROUND, true);

  // title
  mui_text(VEC2_XY(0, 0.55f), "PAUSE", MUI_CENTER | MUI_MIDDLE);

  if (mui_button(VEC2_XY(0.0f, 0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "continue"))
  { test_play(); }
  
  u32 idx;
  char* song_path = audio_music_queue_get_current(&idx);
  char* song_name = NULL;
  u32 song_path_len = strlen(song_path);
  for (int i = song_path_len -1; i >= 0; --i) 
  { if (song_path[i] == '\\' || song_path[i] == '/') { song_name = song_path +i +1; break; } }
  ASSERT(song_name != NULL);
  
  mui_text(VEC2_XY(0, 0.0f), song_name, MUI_CENTER | MUI_MIDDLE);
  
  static bool is_playing = true;
  // if (mui_button(VEC2_XY(0.0f, -0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), is_playing ? "pause" : "play"))
  // { is_playing = audio_music_queue_toggle(); }
  // // mui_icon(VEC2_XY(0.0f, -0.25f), VEC2(0.3f), VEC3(0.85f), 
  // //          is_playing ? assetm_get_mesh_idx("icons/play_icon") : assetm_get_mesh_idx("icons/pause_icon"));
  if (mui_button_icon(VEC2_XY(0.0f, -0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), 
                      is_playing ? assetm_get_mesh_idx("icons/pause_icon") : assetm_get_mesh_idx("icons/play_icon")) )
  { is_playing = audio_music_queue_toggle(); }
  
  if (mui_button(VEC2_XY(-0.35f, -0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "previous song"))
  { audio_music_queue_prev(); }
  
  if (mui_button(VEC2_XY(0.35f, -0.25f), VEC2_XY(1.0f, 0.5f), VEC3(0.5f), "next song"))
  { audio_music_queue_next(); }
  
}
