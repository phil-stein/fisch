#include "global/global.h"
#include "math/math_inc.h"
#include "games.h"  // ./games/games.h
#include "core/state/state.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/assetm.h"
#include "core/io/save_sys/save_sys.h"
#include "core/debug/debug_timer.h"
#include "test/material_table.h"
#include "test/entity_table.h"
// #include "test/test_comp.h"
#include "test/scripts.h"

#include "stb/stb_ds.h"

game_data_t  game_data_data = GAME_DATA_INIT();
game_data_t* game_data = &game_data_data;


u8* buffer = NULL;

// @UNSURE: gets called before main loop regardless if editor or game
void __init__()
{
  save_sys_load_scene_terrain("test_fps.scene", NULL);  
  // save_sys_load_scene_terrain("phys_test.scene", NULL);  
  // // @TODO: this should be safed in .terrain
  // terrain_add_material(MATERIAL_TEMPLATE_GRASS);
  // terrain_add_material(MATERIAL_TEMPLATE_PATH);

  // // in game will be done by camera-controller
  // vec3_copy(VEC3_XYZ(0.0f,   6.0f,  10.0f), core_data->cam.pos);
  // vec3_copy(VEC3_XYZ(0.0f,  -0.15f, -1.0f), core_data->cam.front);

  // @NOTE: scripts_init() replaced by calling scripts_init...() in SCRIPT_ADD()
  // TIMER_FUNC_STATIC(scripts_init());
}

void __update__()
{
  TIMER_FUNC(scripts_update());
}

void __cleanup__()
{
}
