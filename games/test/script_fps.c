#include "test/scripts.h"
#include "test/entity_tags.h"
#include "test/entity_table.h"

#include "core/templates/entity_template.h"
#include "core/core_data.h"
#include "core/io/input.h"
#include "core/io/save_sys/save_sys.h"
#include "core/camera.h"
#include "core/state/state.h"
#include "core/debug/debug_draw.h"

#include "phys/phys_ray.h"  // raycasting

#include "games.h"  // includes bool SCRIPT_REMOVE_FUNC_GENERIC_NAME(u32 uid);

#include "stb/stb_ds.h"

static core_data_t* core_data = NULL;

static vec3 start_pos = { 0, 0, 0 }; // starting position of player char
#define AMMO_MAX 30

void SCRIPT_REGISTER_TRIGGER_CALLBACK_FUNC(fps_controller_script_t)  
{
  // PF("%d collided with trigger: %d\n", this->id, trigger->id);
  
  if (HAS_FLAG(trigger->tags_flag, TAG_UP_FORCE))
  {
    ENTITY_FORCE_Y(this, 50.0f);
  }
}
void SCRIPT_REGISTER_COLLISION_CALLBACK_FUNC(fps_controller_script_t)
{
  // PF("%d collided with: %d\n", this->id, collider->id);
}

void SCRIPT_INIT(fps_controller_script_t)
{
  core_data = core_data_get();
  entity_t* this = state_entity_get(script->entity_id);
  vec3_copy(this->pos, start_pos);
  input_center_cursor_pos(0, 0);
  input_set_cursor_visible(false);

  SCRIPT_REGISTER_TRIGGER_CALLBACK(fps_controller_script_t, script->entity_id);
  SCRIPT_REGISTER_COLLISION_CALLBACK(fps_controller_script_t, script->entity_id);
}
void SCRIPT_UPDATE(fps_controller_script_t)
{
  core_data = core_data_get();
  entity_t* this = state_entity_get(script->entity_id);
  f32 dt = core_data->delta_t;



  //  @NOTE: moving object with physics
  f32 speed      = 500.0f * dt;
  f32 jump_force = 600.0f * 80.0f * dt;
  if (input_get_key_down(KEY_LEFT_SHIFT))
  { speed *= 5.0f; }  // 4.0f

  vec3 front;
  vec3 back;
  vec3 left;
  vec3 right;
  mat4_get_directions(this->model, front, back, left, right);
 
  // flip dirs cause wrong
  vec3_mul_f(front, -speed, front);
  vec3_mul_f(back,  -speed, back);
  vec3_mul_f(left,  -speed, left);
  vec3_mul_f(right, -speed, right);

  if (input_get_key_down(KEY_LEFT_ARROW)  || input_get_key_down(KEY_A))
  { ENTITY_FORCE(this, left); }
	if (input_get_key_down(KEY_RIGHT_ARROW) || input_get_key_down(KEY_D))
  { ENTITY_FORCE(this, right); }
	if (input_get_key_down(KEY_UP_ARROW)    || input_get_key_down(KEY_W))
  { ENTITY_FORCE(this, front); }
	if (input_get_key_down(KEY_DOWN_ARROW)  || input_get_key_down(KEY_S))
  { ENTITY_FORCE(this, back); }
	
  // if (this->is_grounded && input_get_key_pressed(KEY_SPACE))
  if (input_get_key_pressed(KEY_SPACE))
  { ENTITY_FORCE_Y(this, jump_force); }
  
  // @NOTE: reset when falling down
  if (this->pos[1] < -2.0f)
  { 
    P_INFO("player fell, reloading scene");
    P_VEC3(this->pos);

    save_sys_load_scene_from_state_buffer();
  }
  
  // set cam pos to player pos
  const f32 cam_y_offs = 4.5f;
  vec3_copy(VEC3_XYZ(this->pos[0],
                     this->pos[1] + cam_y_offs,
                     this->pos[2]), 
            core_data->cam.pos);

  { // @NOTE: set camera orientation 
    // -- mouse control --
    f32 x_offset = input_get_mouse_delta_x();
    f32 y_offset = input_get_mouse_delta_y();
    
    input_center_cursor_pos(); 
    input_set_cursor_visible(false);
    
    const f32 mouse_sensitivity = 0.5f;
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

	  static f32 pitch, yaw;
    
    // ENTITY_ROTATE_Y(this, -x_offset * 0.75f);
    ENTITY_ROTATE_Y(this, -x_offset);
	  
	  yaw   += x_offset;
	  pitch += y_offset;

    // clamp up & down
	  if (pitch > 89.0f)
	  { pitch = 89.0f; }
	  if (pitch < -89.0f)
	  { pitch = -89.0f; }

    { // init pitch & yaw
      static bool init = false;
	    if (!init)
	    {
        vec3 front;
        vec3_copy(core_data->cam.front, front);
	    	pitch = front[1] * 90; // -30.375f;
	    	yaw	  =	front[2] * 90; // -90.875;
	    	init = true;
	    }
    }

	  vec3 dir;
	  f32 yaw_rad   = yaw;   m_deg_to_rad(&yaw_rad);
	  f32 pitch_rad = pitch; m_deg_to_rad(&pitch_rad);

	  dir[0] = (f32)cos(yaw_rad) * (f32)cos(pitch_rad);
	  dir[1] = (f32)sin(pitch_rad);
	  dir[2] = (f32)sin(yaw_rad) * (f32)cos(pitch_rad);
    camera_set_front(dir);
  }

  // // @NOTE: set camera orientation
  // vec3 target, orientation;
  // vec3_add(core_data->cam.pos, front, target);
  // vec3_sub(target, core_data->cam.pos, orientation);
  // vec3_normalize(orientation, orientation);
  // camera_set_front(orientation);


  // shoot ball
  if (input_get_key_pressed(KEY_ENTER))
  {
    // shoot ball
    vec3 projectile_pos, projectile_force;
    vec3_mul_f(core_data->cam.front, 4.0f, projectile_pos);
    vec3_add(this->pos, projectile_pos, projectile_pos);
    projectile_pos[1] += cam_y_offs; // - 1.5f;
    int projectile_id = state_entity_add_from_template(projectile_pos, VEC3(0), VEC3(0.2f), ENTITY_TEMPLATE_PROJECTILE, false);
    
    entity_t* projectile = state_entity_get(projectile_id);
    vec3_mul_f(core_data->cam.front, 2000.0f, projectile_force); 
    ENTITY_SET_FORCE(projectile, projectile_force);

    // @TMP:
    vec3 line_end;
    vec3_mul_f(core_data->cam.front, 8.0f, line_end);
    vec3_add(this->pos, line_end, line_end);
    debug_draw_line_register_t(projectile_pos, line_end, RGB_F(1, 0, 1), 2.0f);
    
    // // @TMP: test ray v sphere
    // ray_t ray;
    // { // from player
    //   vec3_mul_f(front, 2.0f, ray.pos);
    //   vec3_add(this->pos, ray.pos, ray.pos);
    //   ray.pos[1] += 1.0f;
    //   vec3_copy(front, ray.dir);
    //   vec3_normalize(ray.dir, ray.dir); // prob. not necessary
    //   ray_hit_t hit;
    //   if ( phys_ray_cast(&ray, &hit) )
    //   {
    //     PF("from player hit: "); P_INT(hit.entity_idx);
    //   }
    // }
    // { // from cam 
    //   vec3_mul_f(core_data->cam.front, 2.0f, ray.pos);
    //   vec3_add(core_data->cam.pos, ray.pos, ray.pos);
    //   vec3_copy(core_data->cam.front, ray.dir);  
    //   vec3_normalize(ray.dir, ray.dir); // prob. not necessary
    //   ray_hit_t hit;
    //   if ( phys_ray_cast(&ray, &hit) )
    //   {
    //     PF("from cam hit: "); P_INT(hit.entity_idx);
    //   }
    // }
  }
}
