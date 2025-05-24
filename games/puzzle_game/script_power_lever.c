#include "core/renderer/renderer_extra.h"
#include "core/state/state_scripts.h"
#include "core/types/entity.h"
#include "global/global_print.h"
#include "global/global_types.h"
#include "math/math_m.h"
#include "math/math_mat4.h"
#include "math/math_vec3.h"
#include "puzzle_game/scripts.h"

#include "core/core_data.h"
#include "core/state/state.h"


#include "stb/stb_ds.h"


const f32 turn_t_max = 1.0f;
static u32 sound_lever_idx = SOUND_INVALID_IDX;


void SCRIPT_INIT(power_lever_script_t)
{
  // @TODO: this geta called by SCRIPT_ADD()
  //        so when this function run the script has now yet loaded its data

  entity_t* this = state_entity_get(script->entity_id);
  ENTITY_SET_ROT_X( this, 90.0f );
  script->turn_t = turn_t_max;

  sound_lever_idx = audio_load_clip("lever_01.mp3", SOUND_SPATIAL);  
}
void SCRIPT_CLEANUP(power_lever_script_t)
{
  (void)script;
  // audio ...
}
void SCRIPT_UPDATE(power_lever_script_t)
{
  entity_t* this = state_entity_get(script->entity_id);

  if ( script->turn_t < turn_t_max )
  {
    script->turn_t += core_data->delta_t;

    f32 perc = (script->turn_t / turn_t_max); 

    f32 rot = m_lerp( script->activated ?  90.0f : -90.0f, 
                      script->activated ? -90.0f :  90.0f, perc );

    ENTITY_SET_ROT_X( this, rot );
    // PF( "perc: %.2f, rot: %.2f\n", perc, rot );
  }

  if ( script->action_t < script->action_max_t )
  {
    script->action_t += core_data->delta_t;

    f32 perc = ( script->action_t / script->action_max_t ); 
    
    vec3 pos;
    vec3_lerp( script->activated ? script->start : script->end, 
               script->activated ? script->end   : script->start, perc, pos );

    entity_t* interactable = state_entity_get( script->interactable_id );

    if (interactable != NULL)
    { ENTITY_SET_POS( interactable, pos ); }
  }
}

void power_lever_script_t_set_activated( power_lever_script_t* script, bool act )
{
  script->activated = act;
  if (script->turn_t >= turn_t_max)
  { script->turn_t = 0.0f; }
  else
  { script->turn_t = turn_t_max - script->turn_t; }

  if (script->action_t >= script->action_max_t)
  { script->action_t = 0.0f; }
  else
  { script->action_t = script->action_max_t - script->action_t; }
 
  bool err = false;
  entity_t* e = state_entity_get(script->entity_id);
  point_light_t* pl = state_point_light_get( e->point_light_idx, &err);
  if (err) { P_ERR("failed getting pointlight"); return; }
  vec3_copy(act ? VEC3_XYZ(0, 1, 0) : VEC3_XYZ(1, 0, 0), pl->color);

  vec3 e_pos;
  mat4_get_pos( e->model, e_pos );
  audio_play_sound_spatial(sound_lever_idx, 20.0f, e_pos);
}

