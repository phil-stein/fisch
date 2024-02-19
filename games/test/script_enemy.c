#include "core/types/entity.h"
#include "test/scripts.h"
#include "test/entity_tags.h"
#include "test/entity_table.h"

#include "core/templates/entity_template.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "core/camera.h"
#include "core/state/state.h"
#include "core/io/assetm.h"
#include "core/debug/debug_draw.h"

#include "math/math_inc.h"
#include "phys/phys_ray.h"  // raycasting
#include "mui/mui.h"

#include "games.h"  // includes bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid);

#include "stb/stb_ds.h"


// --- func-decls ---

void SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC(enemy_behaviour_script_t)  
{
  // PF("%d collided with trigger: %d\n", this->id, trigger->id);
}
void SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC(enemy_behaviour_script_t)
{
  // PF("%d collided with: %d\n", this->id, collider->id);
}

void SCRIPT_INIT(enemy_behaviour_script_t)
{
  script->health = 30;
  // entity_t* this = state_entity_get(script->entity_id);
  // SCRIPT_REGISTER_TRIGGER_CALLBACK(enemy_behaviour_script_t, script->entity_id);
  // SCRIPT_REGISTER_COLLISION_CALLBACK(enemy_behaviour_script_t, script->entity_id);
}
void SCRIPT_UPDATE(enemy_behaviour_script_t)
{
  entity_t* this = state_entity_get(script->entity_id);
  f32 dt = core_data->delta_t;

  if (script->health <= 0)
  { state_entity_remove(this); return; }

  if (game_data->player_id <= 0)
  {
    entity_t* player = state_entity_get(game_data->player_id);

    const f32 speed = 1.0f * core_data->delta_t;
    
    if (vec3_distance(this->pos, player->pos) > 2.0f)
    {
      vec3 dir;
      vec3_sub(player->pos, this->pos, dir);
      vec3_normalize(dir, dir);
      vec3_mul_f(dir, speed, dir);

      ENTITY_MOVE(this, dir);
    }
  }
}

