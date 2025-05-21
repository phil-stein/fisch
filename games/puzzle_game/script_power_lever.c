#include "core/renderer/renderer_extra.h"
#include "core/state/state_scripts.h"
#include "core/types/entity.h"
#include "global/global_print.h"
#include "global/global_types.h"
#include "math/math_m.h"
#include "math/math_mat4.h"
#include "math/math_vec3.h"
#include "puzzle_game/scripts.h"
#include "puzzle_game/entity_tags.h"
#include "puzzle_game/entity_table.h"
#include "puzzle_game/puzzle_game.h"

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


const f32 turn_t_max = 1.0f;
static u32 sound_lever_idx = SOUND_INVALID_IDX;


void SCRIPT_INIT(power_lever_script_t)
{
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

  // --- power lever ---

  // @TODO: turn on pointlight when activated

  if ( script->turn_t < turn_t_max )
  {
    script->turn_t += core_data->delta_t;

    f32 perc = 1.0f - (script->turn_t / turn_t_max); 
    f32 rot = m_lerp( script->activated ? -90.0f : 90.0f, 
                      script->activated ? 90.0f : -90.0f, perc);
    ENTITY_SET_ROT_X( this, rot );
    // PF( "perc: %.2f, rot: %.2f\n", perc, rot );
  }
}

void power_lever_script_t_set_activated( power_lever_script_t* script, bool act )
{
  script->activated = act;
  script->turn_t = 0.0f;
 
  bool err = false;
  entity_t* e = state_entity_get(script->entity_id);
  point_light_t* pl = state_point_light_get( e->point_light_idx, &err);
  if (err) { P_ERR("failed getting pointlight"); return; }
  vec3_copy(act ? VEC3_XYZ(0, 1, 0) : VEC3_XYZ(1, 0, 0), pl->color);

  vec3 e_pos;
  mat4_get_pos( e->model, e_pos );
  audio_play_sound_spatial(sound_lever_idx, 20.0f, e_pos);
}

