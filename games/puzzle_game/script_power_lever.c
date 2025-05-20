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


const f32 turn_t_max = 2.0f;


void SCRIPT_INIT(power_lever_script_t)
{
  entity_t* this = state_entity_get(script->entity_id);
  ENTITY_SET_ROT_X( this, 90.0f );
  script->turn_t = turn_t_max;
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
  // if ( script->activated )
  // {
  //   f32 rot = (f32)sin(core_data->total_t) * 90.0f;
  //   ENTITY_SET_ROT_X( this, rot );
  // }

  if ( script->turn_t < turn_t_max )
  {
    script->turn_t += core_data->delta_t;

    f32 perc = 1.0f - (script->turn_t / turn_t_max); 
    // perc = (perc * 2.0f) - 1.0f;  // map to -1.0 <-> 1.0
    // perc = fabsf(perc);
    f32 rot = m_lerp( script->activated ? -90.0f : 90.0f, 
                      script->activated ? 90.0f : -90.0f, perc);
    ENTITY_SET_ROT_X( this, rot );
    PF( "perc: %.2f, rot: %.2f\n", perc, rot );
  }
}

