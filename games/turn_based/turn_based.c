#include "turn_based/turn_based.h"
#include "turn_based/controller.h"
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

  controller_init();
}

void __update__()
{
  controller_update();
}

void __editor_update__() {}

void __cleanup__()
{
  controller_cleanup();
}

