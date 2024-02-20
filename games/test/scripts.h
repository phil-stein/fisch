#ifndef SCRIPTS_H
#define SCRIPTS_H

#include "global/global.h"
#include "core/types/types.h"
#include "core/state/state.h"

#ifdef __cplusplus
extern "C" {
#endif


// @TODO: move game_data to test.h
// @DOC: houses all publicly accessible data for app (editor)
//       instance of app_data_t is in app.c and accesible via app_data_get()
typedef struct 
{
  int player_id;
}game_data_t;

#define GAME_DATA_INIT()                                \
{                                                       \
  .player_id = -1,                                      \
}

// @DOC: extern pointer to app_data_t in game/app.c
//       available everywhere that includes "game/app.h"
extern game_data_t* game_data;


typedef struct
{
  u32  entity_id; // required
  bool is_dead;   // required
  f32  alive_t;
} projectile_script_t;
#define PROJECTILE_SCRIPT_T_INIT .alive_t = 2.0f

typedef struct
{
  u32  entity_id; // required
  bool is_dead;   // required
} player_controller_script_t;

typedef struct
{
  u32  entity_id; // required
  bool is_dead;   // required
  int  health;
  int  ammo_max;
  int  ammo_count;
} fps_controller_script_t;
#define FPS_CONTROLLER_SCRIPT_T_INIT  \
.health = 100, .ammo_max   = 50, .ammo_count = 45
// {                                     \x
//   .health     = 100,                  \x
//   .ammo_max   = 50,                   \x
//   .ammo_count = 45                    \x
// }

typedef struct
{
  u32  entity_id; // required
  bool is_dead;   // required
  int  health;
} enemy_behaviour_script_t;
#define ENEMY_BEHAVIOUR_SCRIPT_T_INIT .health = 30


SCRIPT_DECL(projectile_script_t);
SCRIPT_DECL(player_controller_script_t);
SCRIPT_DECL(fps_controller_script_t);
SCRIPT_DECL(enemy_behaviour_script_t);


void scripts_update();


#ifdef __cplusplus
} // extern c
#endif

#endif  // SCRIPTS_H
