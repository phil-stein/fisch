#include "core/types/entity.h"
#include "math/math_m.h"
#include "math/math_mat4.h"
#include "phys/phys_types.h"
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
  // script->health = 30; // gets set in SCRIPT_REGISTER()
  // P_INT(script->health);
  // entity_t* this = state_entity_get(script->entity_id);
  // SCRIPT_REGISTER_TRIGGER_CALLBACK(enemy_behaviour_script_t, script->entity_id);
  // SCRIPT_REGISTER_COLLISION_CALLBACK(enemy_behaviour_script_t, script->entity_id);
}
void SCRIPT_UPDATE(enemy_behaviour_script_t)
{
  entity_t* this = state_entity_get(script->entity_id);

  if (script->health <= 0)
  { state_entity_remove(this); return; }

  if (game_data->player_id >= 0)
  {
    entity_t* player = state_entity_get(game_data->player_id);

    const f32 speed = 1.0f * core_data->delta_t;

    // rotate towards player
    mat4 lookat;
    vec3 current, target;
    vec3_copy(player->pos, current);
    vec3_copy(this->pos, target);
    current[1] = 0.0f;
    target[1]  = 0.0f;
    mat4_lookat(current, target, VEC3_Y(1), lookat);
    mat4_inverse(lookat, lookat);
    mat4_set_pos_vec3(this->pos, lookat);
    mat4_copy(lookat, this->model);
    this->skip_model_update = true;  // explicitly not update model, cause we do it here
   
    // get directions
    vec3 front, back, left, right;
    mat4_get_directions(this->model, front, back, left, right);

    // draw front as debug-line
    vec3 front_pos;
    vec3_add(front, this->pos, front_pos);
    debug_draw_line_register(this->pos, front_pos, RGB_F(0, 1, 0));
   
    // movement 
    if (vec3_distance(this->pos, player->pos) > 2.0f)
    {
      vec3 dir;
      // vec3_sub(player->pos, this->pos, dir);
      // vec3_normalize(dir, dir);
      vec3_copy(front, dir);

      // ray_t ray = { .dir = { 0, -1, 0} };
      // vec3_copy(this->pos, ray.pos);
      ray_t ray = RAY_T_INIT_LEN(this->pos, VEC3_XYZ(0, -1, 0), 2.5f);
      ray.pos[1] += 4.0f;
      vec3_add(ray.pos, dir, ray.pos);
      ray_hit_t hit;
      // front
      if ( phys_ray_cast(&ray, &hit) )
      {
        // left
        // ray.pos[0] += 1.0f;
        vec3_add(ray.pos, left, ray.pos);
        if ( !phys_ray_cast(&ray, &hit) )
        {
            dir[0] -= 1.0f;
        }
        else 
        {
          // right 
          // ray.pos[0] -= 2.0f;
          vec3_sub(ray.pos, left, ray.pos);
          vec3_add(ray.pos, right, ray.pos);
          if ( !phys_ray_cast(&ray, &hit) )
          {
            dir[0] -= 1.0f;
          }
          else 
          {
            // no way found
            vec3_copy(VEC3(0), dir);
          }
        }
      }
      vec3_mul_f(dir, speed, dir);
      ENTITY_MOVE(this, dir);
    }
    else { P("player too close"); }
  }
  else { PF("player_id not set: %d\n", game_data->player_id); }
}

