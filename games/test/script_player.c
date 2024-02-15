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

void SCRIPT_INIT(player_controller_script_t)
{
  core_data = core_data_get();
  entity_t* this = state_entity_get(script->entity_id);
  vec3_copy(this->pos, start_pos);
  input_center_cursor_pos(0, 0);
  input_set_cursor_visible(false);
}
void SCRIPT_UPDATE(player_controller_script_t)
{
  core_data = core_data_get();
  entity_t* this = state_entity_get(script->entity_id);
  f32 dt = core_data->delta_t;

  // player_ui(this);

  // - physics test -

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
  // vec3 test;
  // vec3_mul_f(front, 2.0f, test);
  // vec3_add(this->pos, test, test);
  // debug_draw_line_register(this->pos, test, RGB_F(1, 0, 1));

  // shoot ball
  if (input_get_key_pressed(KEY_ENTER))
  {
    // shoot ball
    vec3 projectile_pos, projectile_force;
    vec3_mul_f(front, 4.0f, projectile_pos);
    vec3_add(this->pos, projectile_pos, projectile_pos);
    projectile_pos[1] += 2.0f;
    int projectile_id = state_entity_add_from_template(projectile_pos, VEC3(0), VEC3(0.2f), ENTITY_TEMPLATE_PROJECTILE, false);
    
    entity_t* projectile = state_entity_get(projectile_id);
    vec3_mul_f(front, 3000.0f, projectile_force);  // 2000.0f
    ENTITY_SET_FORCE(projectile, projectile_force);

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
  
  vec3_mul_f(front, speed, front);
  vec3_mul_f(back, speed, back);
  vec3_mul_f(left, speed, left);
  vec3_mul_f(right, speed, right);

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
  
  // // @TMP: dont fall
  // ENTITY_SET_POS_Y(this, 0.0f);
  
  // @NOTE: reset when falling down
  if (this->pos[1] < -2.0f)
  { 
    P_INFO("player fell, reloading scene");
    P_VEC3(this->pos);

    save_sys_load_scene_from_state_buffer();
  }

  // @NOTE: set camera orientation
  vec3 cam_pos; 
  vec3_copy(core_data->cam.pos, cam_pos); // camera_get_pos(cam_pos);
  vec3 orientation = { 0.0f, -0.3f, -1.0f };
  vec3 target;
  vec3_add(this->pos, VEC3_Y(3), target); // so not looking at feet
  vec3_sub(target, cam_pos, orientation);
  vec3_normalize(orientation, orientation);
  camera_set_front(orientation);

  // -- mouse control --
  f32 x_offset = input_get_mouse_delta_x();
  f32 y_offset = input_get_mouse_delta_y();
  
  input_center_cursor_pos(); 
  input_set_cursor_visible(false);
  
  f32 mouse_sensitivity = 0.5f;
  x_offset *= mouse_sensitivity;
  y_offset *= mouse_sensitivity;

  ENTITY_ROTATE_Y(this, -x_offset * 0.75f);

  // @NOTE: rotate cam around player x-axis
  // - get player model
  // - rotate by x degree on x-axis
  // - get forward vector
  // - use vec to position cam
  
  static f32 camera_rot = 0.0f;
  camera_rot = CLAMP(camera_rot + y_offset, -80, 10);
  vec3 pos, rot;
  vec3_copy(this->pos, pos);
  vec3_copy(this->rot, rot);
  rot[0] -= camera_rot;

  
  mat4 player_model;
  mat4_make_model(pos, rot, VEC3(1), player_model);
  
  vec3_copy(VEC3(0), cam_pos);
  cam_pos[2] -= 10;  // offset to the back
  mat4 cam_model;
  mat4_make_model(cam_pos, VEC3(0), VEC3(1), cam_model);
  mat4_mul(player_model, cam_model, cam_model);

  // re-assign front & back as the player_model is different from the this->model
  mat4_get_forward(player_model, front);
  vec3_mul_f(front, -1.0f, back);

  vec3_mul_f(back, 10, pos);
  vec3_add(pos, this->pos, pos);
  
  // debug_draw_sphere_register(pos, 0.35f, RGB_F(0, 1, 1));
  vec3_copy(core_data->cam.pos, cam_pos); // camera_get_pos(cam_pos);
  vec3 dif;
  vec3_sub(pos, cam_pos, dif);
  // @NOTE: speed of cam-smoothing framerate dependent, but gets yittery when * dt
  vec3_mul_f(dif, 0.1f, dif); // CLAMP(8.0f * dt, 0.08f, 0.2f)
  // P_F32(8.0f * dt);
  // P_F32(CLAMP(8.0f * dt, 0.08f, 0.2f));
  vec3_add(cam_pos, dif, cam_pos);
  camera_set_pos(cam_pos);

}
