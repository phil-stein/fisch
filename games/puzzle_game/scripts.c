#include "puzzle_game/scripts.h"
#include "puzzle_game/entity_tags.h"
#include "puzzle_game/entity_table.h"

#include "core/templates/entity_template.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "core/camera.h"
#include "core/state/state.h"
#include "core/debug/debug_draw.h"

#include "phys/phys_ray.h"  // raycasting

// includes bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid);
#include "games.h" // needed to include __script_update__(), etc. 

#include "stb/stb_ds.h"
#include "puzzle_game/puzzle_game.h"


// -- register --
// SCRIPT_REGISTER(projectile_script_t, PROJECTILE_SCRIPT_T_INIT)
// SCRIPT_REGISTER(player_controller_script_t, 0)
SCRIPT_REGISTER(fps_controller_script_t,  FPS_CONTROLLER_SCRIPT_T_INIT)
// SCRIPT_REGISTER(enemy_behaviour_script_t, ENEMY_BEHAVIOUR_SCRIPT_T_INIT)

#if defined(_MSC_VER) & !defined(__clang__)
// -- clear arrays --
SCRIPTS_CLEAR_FUNC_START()
  // SCRIPTS_CLEAR_FUNC_SCRIPT(projectile_script_t);
  // SCRIPTS_CLEAR_FUNC_SCRIPT(player_controller_script_t);
  SCRIPTS_CLEAR_FUNC_SCRIPT(fps_controller_script_t);
  // SCRIPTS_CLEAR_FUNC_SCRIPT(enemy_behaviour_script_t);
SCRIPTS_CLEAR_FUNC_END()
// -- generic remove func --
SCRIPT_REMOVE_FUNC_GENERIC_START()
  // SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(projectile_script_t);
  // SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(player_controller_script_t);
  SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(fps_controller_script_t);
  // SCRIPT_REMOVE_FUNC_GENERIC_SCRIPT(enemy_behaviour_script_t);
SCRIPT_REMOVE_FUNC_GENERIC_END()
// -- generic get func --
SCRIPT_GET_FUNC_GENERIC_START()
  // SCRIPT_GET_FUNC_GENERIC_SCRIPT(projectile_script_t);
  // SCRIPT_GET_FUNC_GENERIC_SCRIPT(player_controller_script_t);
  SCRIPT_GET_FUNC_GENERIC_SCRIPT(fps_controller_script_t);
  // SCRIPT_GET_FUNC_GENERIC_SCRIPT(enemy_behaviour_script_t);
SCRIPT_GET_FUNC_GENERIC_END()
// -- get type str func --
SCRIPT_GET_TYPE_STR_FUNC_START()
  // SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(projectile_script_t);
  // SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(player_controller_script_t);
  SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(fps_controller_script_t);
  // SCRIPT_GET_TYPE_STR_FUNC_SCRIPT(enemy_behaviour_script_t);
SCRIPT_GET_TYPE_STR_FUNC_END()
INLINE void SCRIPT_RUN_UPDATE_ALL()                           
{ 
  // SCRIPT_RUN_UPDATE(projectile_script_t);
  // SCRIPT_RUN_UPDATE(player_controller_script_t);
  SCRIPT_RUN_UPDATE(fps_controller_script_t);
  // SCRIPT_RUN_UPDATE(enemy_behaviour_script_t);
}                 
                                                              
INLINE void SCRIPT_RUN_CLEANUP_ALL()                          
{ 
  // SCRIPT_RUN_CLEANUP(projectile_script_t);
  // SCRIPT_RUN_CLEANUP(player_controller_script_t);
  SCRIPT_RUN_CLEANUP(fps_controller_script_t);
  // SCRIPT_RUN_CLEANUP(enemy_behaviour_script_t);
} 

#else // _MSC_VER && !__clang__
  // projectile_script_t,
  // player_controller_script_t,
  // enemy_behaviour_script_t
SCRIPT_FUNCS(
  fps_controller_script_t,
  )
#endif // _MSC_VER && !__clang__


// @NOTE: SCRIPT_INIT(...) called in SCRIPT_ADD(...)

void __scripts_init__() {}

void __scripts_update__()
{
  if (!game_data->run_scripts) { return; }

  // // P_INT(player_controller_script_t_arr_len);
  // SCRIPT_RUN_UPDATE(projectile_script_t);
  // SCRIPT_RUN_UPDATE(player_controller_script_t);
  // SCRIPT_RUN_UPDATE(fps_controller_script_t);
  // SCRIPT_RUN_UPDATE(enemy_behaviour_script_t);
  SCRIPT_RUN_UPDATE_ALL();
}

void __scripts_cleanup__()
{
  // SCRIPT_RUN_CLEANUP(projectile_script_t);
  // SCRIPT_RUN_CLEANUP(player_controller_script_t);
  // SCRIPT_RUN_CLEANUP(fps_controller_script_t);
  // SCRIPT_RUN_CLEANUP(enemy_behaviour_script_t);
  SCRIPT_RUN_CLEANUP_ALL();
}

// void SCRIPT_INIT(projectile_script_t)
// {
//   (void)script;
//   // script->alive_t = 2.0f; // gets set in SCRIPT_REGISTER()
// }
// void SCRIPT_CLEANUP(projectile_script_t) 
// {
//   (void)script;
// }
// void SCRIPT_UPDATE(projectile_script_t)
// {
//   // P_LINE_STR("projectile update ");
//   // entity_t* e = state_entity_get(script->entity_id);
//   script->alive_t -= core_data->delta_t;
//   
//   // P_F32(script->alive_t);
//   if (script->alive_t <= 0)
//   {
//     // PF("removing projectile: %d, is_dead: %s\n", script->entity_id, STR_BOOL(e->is_dead));
//     state_entity_remove_id(script->entity_id);
//   }
// }

// void player_camera(entity_t* this, f32 dt);
// void player_ui(entity_t* this);

