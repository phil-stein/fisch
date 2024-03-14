#include "template/scripts.h"
#include "template/entity_tags.h"
#include "template/entity_table.h"

#include "core/templates/entity_template.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "core/camera.h"
#include "core/state/state.h"
#include "core/debug/debug_draw.h"

#include "phys/phys_ray.h"  // raycasting

#include "games.h"  // includes bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid);

#include "stb/stb_ds.h"
#include "test/test.h"


// -- register --
SCRIPT_REGISTER(test_script_t, TEST_SCRIPT_T_INIT)
// -- clear arrays --
SCRIPTS_CLEAR_FUNC_START()
  SCRIPTS_CLEAR_FUNC_SCRIPT(test_script_t);
SCRIPTS_CLEAR_FUNC_END()
// -- generic remove func --
SCRIPT_REMOVE_FUNC_GENERIC_START()
  SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(test_script_t);
SCRIPT_REMOVE_FUNC_GENERIC_END()
// -- get type str func --
SCRIPT_GET_TYPE_STR_FUNC_START()
  SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(test_script_t);
SCRIPT_GET_TYPE_STR_FUNC_END()


// @NOTE: SCRIPT_INIT(...) called in SCRIPT_ADD(...)

void scripts_update()
{
  if (!game_data->run_scripts) { return; }

  // P_INT(player_controller_script_t_arr_len);
  SCRIPT_RUN_UPDATE(test_script_t);
}

void scripts_cleanup()
{
  SCRIPT_RUN_CLEANUP(test_script_t);
}

void SCRIPT_INIT(test_script_t)
{
  (void)script;
  // script->alive_t = 2.0f; // gets set in SCRIPT_REGISTER()
}
void SCRIPT_CLEANUP(test_script_t) 
{
  (void)script;
}
void SCRIPT_UPDATE(test_script_t)
{
  // P_LINE_STR("projectile update ");
  // entity_t* e = state_entity_get(script->entity_id);
  script->alive_t -= core_data->delta_t;
  
  // P_F32(script->alive_t);
  if (script->alive_t <= 0)
  {
    // PF("removing projectile: %d, is_dead: %s\n", script->entity_id, STR_BOOL(e->is_dead));
    state_entity_remove_id(script->entity_id);
  }
}

// void player_camera(entity_t* this, f32 dt);
// void player_ui(entity_t* this);

