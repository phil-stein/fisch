#include "core/state/state_scripts.h"
#include "test/test.h"
#include "test/scripts.h"

#include "core/core_data.h"
#include "core/state/state.h"
#include "core/debug/debug_draw.h"
#include "core/types/entity.h"

#include "math/math_inc.h"
#include "phys/phys_ray.h"  // raycasting
#include "phys/phys_types.h"
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
  game_data->enemy_count++;
}
void SCRIPT_UPDATE(enemy_behaviour_script_t)
{
  entity_t* this = state_entity_get(script->entity_id);

  if (script->health <= 0)
  { 
    game_data->score++;
    state_entity_remove(this); 
    return; 
  }

  if (game_data->player_id >= 0)
  {
    entity_t* player = state_entity_get(game_data->player_id);

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
    // this->is_moved = false;

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

      // check where enemy can go via raycasts
      ray_t forward_ray = RAY_T_INIT_LEN(this->pos, VEC3_XYZ(0, -1, 0), 2.5f);
      forward_ray.pos[1] += 4.0f;
      vec3_add(forward_ray.pos, front, forward_ray.pos);
      forward_ray.draw_debug = true;
      ray_hit_t forward_hit;
      phys_ray_cast(&forward_ray, &forward_hit);
      ray_t left_ray = forward_ray;
      ray_hit_t left_hit;
      vec3_add(left_ray.pos, left, left_ray.pos);
      phys_ray_cast(&left_ray, &left_hit);
      ray_t right_ray = forward_ray;
      ray_hit_t right_hit;
      vec3_add(right_ray.pos, right, right_ray.pos);
      phys_ray_cast(&right_ray, &right_hit);
    
      if (forward_hit.hit)
      {
        f32 left_dist  = vec3_distance(player->pos, left_ray.pos);
        f32 right_dist = vec3_distance(player->pos, right_ray.pos);
        if (!left_hit.hit && left_dist <= right_dist)
        { vec3_copy(left, dir); }
        else if(!right_hit.hit)
        { vec3_copy(right, dir); }
        else  // fuck it just fly lol
        {
          vec3_copy(VEC3_Y(500 * core_data->delta_t), dir);
        }
      }

      // hit player
      ray_t hit_ray = RAY_T_INIT(this->pos, front, 2.5f, &this->id, 1);
      hit_ray.pos[1] += 3.0f;
      // hit_ray.draw_debug = true;
      ray_hit_t hit_hit;
      if ( phys_ray_cast(&hit_ray, &hit_hit) && hit_hit.entity_idx == game_data->player_id )
      {
        fps_controller_script_t* fps_controller = SCRIPT_ENTITY_GET(fps_controller_script_t, player);
        fps_controller->health -= 1;
      }

      const f32 speed = 750.0f * core_data->delta_t;
      vec3_mul_f(dir, speed, dir);
      // ENTITY_MOVE(this, dir);
      ENTITY_FORCE(this, dir);
    }
    // else { P("player too close"); }
  }
  else { PF("player_id not set: %d\n", game_data->player_id); }
}

