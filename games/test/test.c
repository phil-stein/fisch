#include "global/global.h"
#include "math/math_inc.h"
#include "games.h"  // ./games/games.h
#include "core/state.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "test/material_table.h"
#include "test/entity_table.h"
#include "test/test_comp.h"

#include "stb/stb_ds.h"

static core_data_t* core_data = NULL;
u8* buffer = NULL;

void __init__()
{
  core_data = core_data_get();
  
  const char scene_name[] = "test.scene";
  //TIMER_FUNC_STATIC(save_sys_load_scene_from_file(scene_name));
  save_sys_load_scene_from_file(scene_name);
  // TIMER_FUNC_STATIC(save_sys_load_terrain_from_file("test.terrain"));
  // TIMER_FUNC_STATIC(terrain_create(25));

  // in game will be done by camera-controller
  vec3_copy(VEC3_XYZ(0.0f,   6.0f,  10.0f), core_data->cam.pos);
  vec3_copy(VEC3_XYZ(0.0f,  -0.15f, -1.0f), core_data->cam.front);
}

void __update__()
{

}

void __cleanup__()
{
}
