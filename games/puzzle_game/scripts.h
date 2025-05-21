#ifndef GAME_TEST_SCRIPTS_H
#define GAME_TEST_SCRIPTS_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/state/state.h"
#include "core/state/state_scripts.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
  int  entity_id; // required
  bool is_dead;   // required
  bool is_active; // required
  bool activated;
  f32  turn_t;
} power_lever_script_t;
#define POWER_LEVER_SCRIPT_T_INIT .activated = false, .turn_t = 0.0f
typedef struct
{
  int  entity_id; // required
  bool is_dead;   // required
  bool is_active; // required
  int  health;
  int  ammo_max;
  int  ammo_count;
} fps_controller_script_t;
#define FPS_CONTROLLER_SCRIPT_T_INIT  \
.health = 100, .ammo_max   = 50, .ammo_count = 45


SCRIPT_DECL(fps_controller_script_t);
SCRIPT_DECL(power_lever_script_t);
void power_lever_script_t_set_activated( power_lever_script_t* script, bool act );




#ifdef __cplusplus
} // extern c
#endif

#endif  // SCRIPTS_H
