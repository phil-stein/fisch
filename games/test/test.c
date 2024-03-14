#include "test/test.h"
#include "core/audio/audio.h"
#include "core/io/input.h"
#include "math/math_vec2.h"
#include "test/scripts.h"
#include "test/material_table.h"

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
  save_sys_load_scene_terrain("test_fps.scene", "test_fps.terrain");  
  // save_sys_load_scene_terrain("phys_test.scene", NULL);  
  // // @TODO: this should be safed in .terrain
  terrain_add_material(MATERIAL_TEMPLATE_GRASS);
  terrain_add_material(MATERIAL_TEMPLATE_PATH);
  
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
