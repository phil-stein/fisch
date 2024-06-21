
#include "core/renderer/renderer_extra.h"
#include "core/state/state_scripts.h"
#include "core/types/entity.h"
#include "global/global_print.h"
#include "math/math_m.h"
#include "math/math_mat4.h"
#include "math/math_vec3.h"

#include "turn_based/scripts.h"
#include "turn_based/entity_tags.h"
#include "turn_based/entity_table.h"
#include "turn_based/turn_based.h"

#include "core/templates/entity_template.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "core/camera.h"
#include "core/state/state.h"
#include "core/io/assetm.h"
#include "core/debug/debug_draw.h"
#include "core/mui/mui.h"

#include "math/math_inc.h"
#include "phys/phys_ray.h"  // raycasting

#include "stb/stb_ds.h"


const vec3 hover_col = { 1.0f, 0.0f, 0.0f };

// static u32 sound_jump_idx    = SOUND_INVALID_IDX;

// --- func-decls ---

// void SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC(tile_script_t)  
// {
// }
// void SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC(tile_script_t)
// {
// }

void SCRIPT_INIT( tile_script_t )
{
  // entity_t* this = 
  state_entity_get( script->entity_id );
  
  // sound_jump_idx   = audio_load_clip("woosh_01_01.wav", SOUND_SPATIAL);  
  
  game_data->grid_ids[game_data->grid_ids_pos++] = script->entity_id;
}
void SCRIPT_CLEANUP( tile_script_t )
{
  (void)script;
  // audio ...
}
void SCRIPT_UPDATE( tile_script_t )
{
  (void)script;
  // entity_t* this = state_entity_get( script->entity_id );
  // 
  // if ( renderer_extra_mouse_position_mouse_pick_id() == this->id )
  // {
  //   vec3_copy( (f32*)hover_col, this->tint );
  // }
  // else { vec3_copy( VEC3(1.0f), this->tint ); }
}
