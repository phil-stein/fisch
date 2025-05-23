#ifndef GAME_TEST_SCRIPTS_H
#define GAME_TEST_SCRIPTS_H

#ifdef EDITOR
#include "editor/gui/gui.h"
#endif // EDITOR
#include "global/global.h"
#include "core/types/types.h"
#include "core/state/state.h"
#include "core/state/state_scripts.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum lever_action_type { LEVER_ACTION_MOVE, LEVER_ACTION_ }lever_action_type;
typedef struct
{
  int  entity_id; // required
  bool is_dead;   // required
  bool is_active; // required
  bool activated;
  f32  turn_t;
  
  // -- lever action --
  lever_action_type action_type; 
  int interactable_id;
  f32 action_t;
  f32 action_max_t;
  vec3 start;
  vec3 end;

} power_lever_script_t;
#define POWER_LEVER_SCRIPT_T_INIT .activated = false, .turn_t = 0.0f, .action_t = 0.0f,
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

#ifdef EDITOR
typedef struct nk_context ui_context;
void editor_entity_ui_callback( ui_context* ctx, entity_t* e );
#endif // EDITOR

void scripts_save_data_serialize(u8** buffer, power_lever_script_t* lever_scripts, u32 lever_scripts_len );
void scripts_write_save_data();
void scripts_write_save_data_to_buffer();
void scripts_save_data_deserialize( u8* buffer, u32* offset, power_lever_script_t* lever_scripts, u32 lever_scripts_len );
void scripts_load_save_data();
void scripts_load_save_data_from_buffer();


#ifdef __cplusplus
} // extern c
#endif

#endif  // SCRIPTS_H
