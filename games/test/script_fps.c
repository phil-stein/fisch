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

static core_data_t* core_data = NULL;

static bool cam_init; // gets set in init
static f32 pitch, yaw;
static const f32 cam_y_offs = 4.5f;

static vec3 start_pos = { 0, 0, 0 }; // starting position of player char

#define AMMO_MAX 30
static int ammo_count = AMMO_MAX;

// --- func-decls ---
static void script_fps_cam(entity_t* this);
static void scritp_fps_ui(entity_t* this);

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
  input_center_cursor_pos();
  input_set_cursor_visible(false);

  cam_init = false;

  SCRIPT_REGISTER_TRIGGER_CALLBACK(fps_controller_script_t, script->entity_id);
  SCRIPT_REGISTER_COLLISION_CALLBACK(fps_controller_script_t, script->entity_id);
}
void SCRIPT_UPDATE(fps_controller_script_t)
{
  core_data = core_data_get();
  entity_t* this = state_entity_get(script->entity_id);
  f32 dt = core_data->delta_t;

  scritp_fps_ui(this);

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
  
  script_fps_cam(this);

  // shoot ball
  // if (input_get_key_pressed(KEY_ENTER))
  if (input_get_mouse_pressed(MOUSE_LEFT) && ammo_count > 0)
  {
    ammo_count--;

    // // shoot ball
    // vec3 projectile_pos, projectile_force;
    // vec3_mul_f(core_data->cam.front, 4.0f, projectile_pos);
    // vec3_add(this->pos, projectile_pos, projectile_pos);
    // projectile_pos[1] += cam_y_offs; // - 1.5f;
    // int projectile_id = state_entity_add_from_template(projectile_pos, VEC3(0), VEC3(0.2f), ENTITY_TEMPLATE_PROJECTILE, false);
    // 
    // entity_t* projectile = state_entity_get(projectile_id);
    // vec3_mul_f(core_data->cam.front, 2000.0f, projectile_force); 
    // ENTITY_SET_FORCE(projectile, projectile_force);

    // // @TMP:
    // vec3 line_end;
    // vec3_mul_f(core_data->cam.front, 8.0f, line_end);
    // vec3_add(this->pos, line_end, line_end);
    // debug_draw_line_register_t(projectile_pos, line_end, RGB_F(1, 0, 1), 2.0f);
    
    ray_t ray;
    { // from cam 
      vec3_mul_f(core_data->cam.front, 2.0f, ray.pos);
      vec3_add(core_data->cam.pos, ray.pos, ray.pos);
      vec3_copy(core_data->cam.front, ray.dir);  
      vec3_normalize(ray.dir, ray.dir); // prob. not necessary
      ray_hit_t hit;
      // if ( phys_ray_cast_len(&ray, &hit, 20.0f) )
      if ( phys_ray_cast(&ray, &hit) )
      {
        // PF("from cam hit: "); P_INT(hit.entity_idx);
        entity_t* e = state_entity_get(hit.entity_idx);
        vec3 f;
        vec3_sub(hit.hit_point, this->pos, f);
        vec3_normalize(f, f);
        vec3_mul_f(f, 200.0f, f);
        ENTITY_FORCE(e, f);
      }
    }
  }
  // reload
  if (input_get_key_pressed(KEY_R))
  {
    ammo_count = AMMO_MAX;
  }
}

static void script_fps_cam(entity_t* this)
{
  // set cam pos to player pos
  vec3_copy(VEC3_XYZ(this->pos[0],
                     this->pos[1] + cam_y_offs,
                     this->pos[2]), 
            core_data->cam.pos);

  // @NOTE: set camera orientation 
  // -- mouse control --
  f32 x_offset = input_get_mouse_delta_x();
  f32 y_offset = input_get_mouse_delta_y();
  
  input_center_cursor_pos(); 
  input_set_cursor_visible(false);
  
  const f32 mouse_sensitivity = 0.5f;
  x_offset *= mouse_sensitivity;
  y_offset *= mouse_sensitivity;

  
  // ENTITY_ROTATE_Y(this, -x_offset * 0.75f);
  ENTITY_ROTATE_Y(this, -x_offset);
	
	yaw   += x_offset;
	pitch += y_offset;

  // clamp up & down
	if (pitch > 89.0f)
	{ pitch = 89.0f; }
	if (pitch < -89.0f)
	{ pitch = -89.0f; }

  // init pitch & yaw
	if (!cam_init)
	{
    vec3 front;
    vec3_copy(core_data->cam.front, front);
		pitch = front[1] * 90; // -30.375f;
		yaw	  =	front[2] * 90; // -90.875;
		cam_init = true;
	}

	vec3 dir;
	f32 yaw_rad   = yaw;   m_deg_to_rad(&yaw_rad);
	f32 pitch_rad = pitch; m_deg_to_rad(&pitch_rad);

	dir[0] = (f32)cos(yaw_rad) * (f32)cos(pitch_rad);
	dir[1] = (f32)sin(pitch_rad);
	dir[2] = (f32)sin(yaw_rad) * (f32)cos(pitch_rad);
  camera_set_front(dir);

  // rotate children
  for (int i = 0; i < this->children_len; ++i)
  {
    int child_idx = this->children[i];
    entity_t* e = state_entity_get(child_idx);

    vec3 e_pos, e_rot;
    vec3_copy(e->pos, e_pos);
    e_pos[1] -= cam_y_offs;   // cause were using cam_pos not this->pos for 'parent' model
    vec3_copy(e->rot, e_rot);
    // e_rot[0] += 45;
    mat4_make_model(e_pos, e_rot, e->scl, e->model);  // parent indipendent
    // mat4_mul(this->model, e->model, e->model);
    
    mat4 rot_mat;
    // mat4_rotate_make(rot_mat, x_offset, this->rot);
    // mat4_make_model(this->pos, VEC3_XYZ(0, this->rot[1], 0), VEC3(1), rot_mat); 
    vec3 rot = VEC3_INIT(0);
    // vec3_copy(this->rot, rot);
    // rot[0] += 15;  // sway or something
    // rot[0] += (f32)sin(core_data->total_t) * 90.0f;
    rot[0] = pitch + 25;
    rot[1] = this->rot[1];
    mat4_make_model(core_data->cam.pos, rot, VEC3(1), rot_mat);  
    mat4_mul(rot_mat, e->model, e->model);
    // f32 rot_y_rad = this->rot[1]; 
    // m_deg_to_rad(&rot_y_rad);
    // vec3 axis;
    // // camera_get_right_axis(axis);
    // // camera_get_up_axis(axis);
    // // vec3_mul_f(core_data->cam.front, -1.0f, axis);
    // vec3_copy(core_data->cam.front, axis);
    // mat4_rotate_make(rot_mat, rot_y_rad, axis);
    // mat4_mul(rot_mat, e->model, e->model);
    
    e->skip_model_update = true;  // explicitly not update model, cause we do it here
  }
}

static void scritp_fps_ui(entity_t* this)
{
  texture_t* circle_tex = assetm_get_texture("#internal/circle.png", false);
  texture_t* weapon_tex = assetm_get_texture("_icons/kriss_vector_01.png", false);
  
  // reticle
  mui_img_tint(VEC2_XY(0.0f, 0.0f),   VEC2(0.05f), circle_tex, VEC3(1.0f));
  
  // -- circle & ammo --
  { 
    mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.80f), circle_tex, VEC3(0.75f));
    mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.65f), circle_tex, VEC3(0.55f));
  
    mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.45f), weapon_tex, VEC3(1.00f));
  
    mui_img_tint(VEC2_XY(-0.72f, -0.72f), VEC2(0.50f), circle_tex, VEC3(0.35f));
  
    char txt[64];
    SPRINTF(64, txt, "%d|%d", ammo_count, AMMO_MAX);
    mui_text(VEC2_XY(-0.70f, -0.65f), txt, MUI_CENTER | MUI_UP);
  }
  // -- inventory --
  {
    mui_group_t group;
    mui_group_t items;
    // MUI_GROUP_T_INIT(&group, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_DYNAMIC | MUI_CENTER | MUI_ROW, false);
    // MUI_GROUP_T_INIT(&items, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_DYNAMIC | MUI_CENTER | MUI_ROW, false);
    // 
    // MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 1.00f, 1.00f, 1.00f));
    // MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.75f, 0.75f, 0.75f));
    // MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.50f, 0.50f, 0.50f));
    // MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.25f, 0.25f, 0.25f));
    // 
    // mui_group(&group);
    {
      MUI_GROUP_T_INIT(&group, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_STATIC | MUI_CENTER | MUI_ROW, false);
      
      MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 1.00f, 1.00f, 1.00f));
      MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.75f, 0.75f, 0.75f));
      MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.50f, 0.50f, 0.50f));
      MUI_GROUP_T_ADD(&group, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.25f, 0.25f, 0.25f));
      
      mui_group(&group);
    }
    {
      MUI_GROUP_T_INIT(&items, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_STATIC | MUI_CENTER | MUI_ROW, false);
      mui_obj_t* slot_1, *slot_2, *slot_3, *slot_4;
      MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, weapon_tex, 1.00f, 1.00f, 1.00f), slot_1);
      MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, weapon_tex, 0.75f, 0.75f, 0.75f), slot_2);
      MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, weapon_tex, 0.50f, 0.50f, 0.50f), slot_3);
      MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, weapon_tex, 0.25f, 0.25f, 0.25f), slot_4);
      
      static int active_slot = 1;
      slot_1->active = active_slot == 1;
      slot_2->active = active_slot == 2;
      slot_3->active = active_slot == 3;
      slot_4->active = active_slot == 4;
  
      if (input_get_key_pressed(KEY_ALPHA1) || input_get_key_pressed(KEY_NUMPAD1)) { active_slot = 1; }
      if (input_get_key_pressed(KEY_ALPHA2) || input_get_key_pressed(KEY_NUMPAD2)) { active_slot = 2; }
      if (input_get_key_pressed(KEY_ALPHA3) || input_get_key_pressed(KEY_NUMPAD3)) { active_slot = 3; }
      if (input_get_key_pressed(KEY_ALPHA4) || input_get_key_pressed(KEY_NUMPAD4)) { active_slot = 4; }
  
      
      mui_group(&items);
    }
  }
}
