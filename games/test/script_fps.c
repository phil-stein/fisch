#include "core/state/state_scripts.h"
#include "core/types/entity.h"
#include "global/global_print.h"
#include "math/math_m.h"
#include "math/math_mat4.h"
#include "math/math_vec3.h"
#include "test/scripts.h"
#include "test/entity_tags.h"
#include "test/entity_table.h"
#include "test/test.h"

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


static f32 pitch, yaw;
static const f32 cam_y_offs = 4.5f;

static vec3 start_pos = { 0, 0, 0 }; // starting position of player char

static u32 sound_gun_idx     = -1;
static u32 sound_reload_idx  = -1;
static u32 sound_jump_idx    = -1;

// --- func-decls ---
static void script_fps_cam(entity_t* this);
static void script_fps_ui(entity_t* this, fps_controller_script_t* script);

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
  // P_INT(script->ammo_count);
  entity_t* this = state_entity_get(script->entity_id);
  vec3_copy(this->pos, start_pos);

  // pitch = core_data->cam.pitch_rad;
  // yaw   = core_data->cam.yaw_rad;
  pitch = 0;
  yaw   = 0;
  m_rad_to_deg(&yaw);
  m_rad_to_deg(&pitch);
  input_center_cursor_pos();
  input_set_cursor_visible(false);

  SCRIPT_REGISTER_TRIGGER_CALLBACK(fps_controller_script_t, script->entity_id);
  SCRIPT_REGISTER_COLLISION_CALLBACK(fps_controller_script_t, script->entity_id);

  game_data->player_id = this->id;
  
  sound_gun_idx    = audio_load_clip("gun_01_01.mp3",   SOUND_SPATIAL);
  sound_reload_idx = audio_load_clip("reload_01_01.mp3",   SOUND_SPATIAL);
  sound_jump_idx   = audio_load_clip("woosh_01_01.wav", SOUND_SPATIAL);  
}
void SCRIPT_CLEANUP(fps_controller_script_t)
{
  // audio ...
}
void SCRIPT_UPDATE(fps_controller_script_t)
{
  entity_t* this = state_entity_get(script->entity_id);
  entity_t* shotgun = state_entity_get(this->children[0]);

  script_fps_ui(this, script);

  //  @NOTE: moving object with physics
  f32 speed       = 36.0f;  
  f32 jump_force  = 1400.0f;
  f32 slide_force = 60.0f; 
  if (input_get_key_down(KEY_LEFT_SHIFT))
  { speed *= 2.0f; }  // 4.0f

  vec3 front, back, left, right;
  vec3 front_scaled, back_scaled, left_scaled, right_scaled;
  // @BUGG: @UNSURE: flipped for some reason
  //                 prob bc. of hardcoding camera rotation
  //                 and using it for this->rot
  // mat4_get_directions(this->model, front, back, left, right);
  mat4_get_directions(this->model, right, left, front, back);
 
  vec3_mul_f(front, speed, front_scaled);
  vec3_mul_f(back,  speed, back_scaled);
  vec3_mul_f(left,  speed, left_scaled);
  vec3_mul_f(right, speed, right_scaled);
  
  // @TMP:
  vec3 front_dbg_pos;
  vec3_add(this->pos, front, front_dbg_pos);
  debug_draw_line_register(this->pos, front_dbg_pos, RGB_F(0, 0, 1));
  vec3 right_dbg_pos;
  vec3_add(this->pos, right, right_dbg_pos);
  debug_draw_line_register(this->pos, right_dbg_pos, RGB_F(1, 0, 0));

  bool is_moving = false;
  if (input_get_key_down(KEY_LEFT_ARROW)  || input_get_key_down(KEY_A))
  { ENTITY_FORCE(this, left_scaled); is_moving = true; }
	if (input_get_key_down(KEY_RIGHT_ARROW) || input_get_key_down(KEY_D))
  { ENTITY_FORCE(this, right_scaled); is_moving = true; }
	if (input_get_key_down(KEY_UP_ARROW)    || input_get_key_down(KEY_W))
  { ENTITY_FORCE(this, front_scaled); is_moving = true; }
	if (input_get_key_down(KEY_DOWN_ARROW)  || input_get_key_down(KEY_S))
  { ENTITY_FORCE(this, back_scaled); is_moving = true; }
    
  // if (this->is_grounded && input_get_key_pressed(KEY_SPACE))
  if (input_get_key_pressed(KEY_SPACE))
  { 
    vec3 sound_pos = {0};
    // camera_get_front(sound_pos);
    vec3_add(core_data->cam.pos, sound_pos, sound_pos);
    audio_play_sound_spatial(sound_jump_idx, 2.0f, sound_pos);
    ENTITY_FORCE_Y(this, jump_force);
  }
  // slide
  static f32 slide_t     = 0.0f;
  const  f32 slide_t_max = 0.75f; 
  static f32 this_scl_y  = 0.0f;
  static vec3 slide_dir  = { 0.0f };
  if (slide_t > 0.0f)
  {
    slide_t -= core_data->delta_t;
    ENTITY_FORCE(this, slide_dir);
    P_VEC3(slide_dir);
    
    f32 perc = 1.0f - (slide_t / slide_t_max); 
    perc = (perc * 2.0f) - 1.0f;  // map to -1.0 <-> 1.0
    perc = fabsf(perc);
    // f32 perc = 1.0f - (slide_t / slide_t_max*2.0f); 
    ENTITY_SET_ROT_Z(shotgun, m_lerp(-60.0f, 0.0f, perc*perc)); 
    PF("%.2f -> %.2f\n", perc, shotgun->rot[2]);
    if (slide_t <= 0.0f)
    { 
      ENTITY_MOVE_Y(this, this_scl_y);
      ENTITY_SET_SCL_Y(this, this_scl_y); 
      ENTITY_SET_ROT_Z(shotgun, 0.0f);
    }
  }
  else if (is_moving && input_get_key_pressed(KEY_LEFT_CONTROL))
  {
    // vec3_mul_f(front, slide_force, front_scaled);
    // front_scaled[1] *= 0.8f;  // slighly down
    // ENTITY_FORCE(this, front_scaled);
    vec3_mul_f(front, slide_force, slide_dir);
    slide_dir[1] = -52.5f;  // slighly down
    ENTITY_FORCE(this, slide_dir);
    
    slide_t = slide_t_max;
    this_scl_y = this->scl[1];
    ENTITY_SET_SCL_Y(this, this_scl_y * 0.5f);
    // ENTITY_MOVE_Y(this, this_scl_y * -0.5f);
  }

  
  // @NOTE: reset when falling down
  if (this->pos[1] < -2.0f)
  { 
    P_INFO("player fell, reloading scene\n");
    P_VEC3(this->pos);

    save_sys_load_scene_from_state_buffer();
  }
  
  script_fps_cam(this);

  // reload
  const  f32 reload_t_max = 0.75f;
  static f32 reload_t = 0.0f;
  if (input_get_key_pressed(KEY_R))
  {
    script->ammo_count = script->ammo_max;
    vec3 sound_pos;
    camera_get_front(sound_pos);
    vec3_add(core_data->cam.pos, sound_pos, sound_pos);
    audio_play_sound_spatial(sound_reload_idx, 2.0f, sound_pos);

    reload_t = reload_t_max;
  }
  if (reload_t >= 0.0f)
  {
    reload_t -= core_data->delta_t;
    f32 perc =(reload_t / reload_t_max); 
    // ENTITY_SET_ROT_X(e, perc * 360.0f); 
    ENTITY_SET_ROT_X(shotgun, m_ease_in_cubic(0.0f, 360.0f, perc*perc)); 
  }
  // shoot ball
  // @TODO: mouse_pressed() doesnt work in game 
  #ifdef EDITOR
  else if (input_get_mouse_pressed(MOUSE_LEFT) && script->ammo_count > 0)
  #else
  if (input_get_key_pressed(KEY_ENTER) && script->ammo_count > 0)
  #endif
  {
    vec3 sound_pos;
    camera_get_front(sound_pos);
    vec3_add(core_data->cam.pos, sound_pos, sound_pos);
    audio_play_sound_spatial(sound_gun_idx, 2.0f, sound_pos);
    
    script->ammo_count--;

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
    
    { // from cam 
      ray_t ray = RAY_T_INIT_ZERO();
      ray.draw_debug = true;
      vec3 cam_front;
      camera_get_front(cam_front);
      vec3_mul_f(cam_front, 1.0f, ray.pos);
      vec3_add(core_data->cam.pos, ray.pos, ray.pos);
      vec3_copy(cam_front, ray.dir);  
      vec3_normalize(ray.dir, ray.dir); // prob. not necessary
      ray.mask_arr     = &this->id;
      ray.mask_arr_len = 1;
      ray_hit_t hit;
      // if ( phys_ray_cast_len(&ray, &hit, 20.0f) )
      // if ( phys_ray_cast(&ray, &hit) )
      if ( phys_ray_cast(&ray, &hit) ) // mask player
      {
        // PF("from cam hit: "); P_INT(hit.entity_idx);
        entity_t* e = state_entity_get(hit.entity_idx);
        // mix of hit-normal and vec towards hit
        vec3 f, dir;
        vec3_mul_f(hit.normal, -1.0f, dir);
        vec3_sub(hit.hit_point, this->pos, f);
        vec3_normalize(f, f);
        vec3_add(f, dir, f);
        if (HAS_FLAG(e->tags_flag, TAG_ENEMY)) 
        { vec3_mul_f(f, 2000.0f, f); }
        else 
        { vec3_mul_f(f, 600.0f, f); }
        ENTITY_FORCE(e, f);
        
        if (HAS_FLAG(e->tags_flag, TAG_ENEMY))
        {
          enemy_behaviour_script_t* enemy_behaviour = SCRIPT_ENTITY_GET(enemy_behaviour_script_t, e);
          enemy_behaviour->health     -= 10;
          enemy_behaviour->tint_t_max =  0.25f;
          enemy_behaviour->tint_t     =  enemy_behaviour->tint_t_max;
        }
      }
    }
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

	f32 yaw_rad   = yaw;   m_deg_to_rad(&yaw_rad);
	f32 pitch_rad = pitch; m_deg_to_rad(&pitch_rad);

	// dir[0] = (f32)cos(yaw_rad) * (f32)cos(pitch_rad);
	// dir[1] = (f32)sin(pitch_rad);
	// dir[2] = (f32)sin(yaw_rad) * (f32)cos(pitch_rad);
  // camera_set_front(dir);
  // camera_set_front(pitch_rad, yaw_rad);
  camera_set_pitch_yaw(pitch_rad, yaw_rad);

  // rotate children
  for (int i = 0; i < this->children_len; ++i)
  {
    int child_idx = this->children[i];
    entity_t* e = state_entity_get(child_idx);

    vec3 rot;
    vec3_copy(e->rot, rot);
    rot[1] += 180;
    camera_parent_entity_offset(e, VEC3_XYZ(-1.0f, -0.75, 2.5f), rot, e->scl);
  }
}

static void script_fps_ui(entity_t* this, fps_controller_script_t* script)
{
  // texture_t* circle_tex = assetm_get_texture("#internal/circle.png", false);
  texture_t* weapon_tex = assetm_get_texture("_icons/kriss_vector_01.png", false);
  
  // reticle
  // mui_img_tint(VEC2_XY(0.0f, 0.0f),   VEC2(0.05f), circle_tex, VEC3(1.0f));
  mui_circle(VEC2_XY(0.0f, 0.0f), VEC2(0.05f), VEC3(1.0f));
  
  // -- circle & ammo --
  { 
    // mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.80f), circle_tex, VEC3(0.75f));
    // mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.65f), circle_tex, VEC3(0.55f));
    // mui_img_tint(VEC2_XY(-0.8f, -0.8f),   VEC2(0.45f), weapon_tex, VEC3(1.00f));
    // mui_img_tint(VEC2_XY(-0.72f, -0.72f), VEC2(0.50f), circle_tex, VEC3(0.35f));
    mui_circle(VEC2_XY(-0.8f, -0.8f),   VEC2(0.80f), VEC3(0.75f));
    mui_circle(VEC2_XY(-0.8f, -0.8f),   VEC2(0.65f), VEC3(0.55f));
    mui_img(VEC2_XY(-0.8f, -0.8f),   VEC2(0.45f), weapon_tex);
    mui_circle(VEC2_XY(-0.72f, -0.72f), VEC2(0.50f), VEC3(0.35f));
  
    mui_textf(VEC2_XY(-0.72f, -0.72f), MUI_CENTER | MUI_MIDDLE, 
              "%d|%d", script->ammo_count, script->ammo_max);
    
    mui_textf(VEC2_XY(0.95f, 0.95f), MUI_LEFT| MUI_DOWN, 
              "score: %d/%d", game_data->score, game_data->enemy_count);
    
    f32 perc = script->health / 100.0f;
    mui_rect_oriented(VEC2_XY(0.75f, -0.9f), VEC2_XY(0.75f, 0.3f), VEC3(0.25f), MUI_MIDDLE | MUI_RIGHT);
    mui_rect_oriented(VEC2_XY(0.765f, -0.9f), VEC2_XY(0.65f*perc, 0.2f), RGB_F(1.0f, 0.3125f, 0.3125f), MUI_MIDDLE | MUI_RIGHT);
    mui_textf(VEC2_XY(0.77f, -0.9f), MUI_MIDDLE | MUI_RIGHT, 
              "%d%%", script->health);

  }
  // -- inventory --
  {
    mui_group_t item_bgs;
    mui_group_t items;
    {
      MUI_GROUP_T_INIT(&item_bgs, VEC2_XY(0.0f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_STATIC | MUI_CENTER | MUI_ROW, false);
      
      // MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_IMG_GROUP(circle_tex, 1.00f, 1.00f, 1.00f));
      // MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.75f, 0.75f, 0.75f));
      // MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.50f, 0.50f, 0.50f));
      // MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_IMG_GROUP(circle_tex, 0.25f, 0.25f, 0.25f));
      MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_SHAPE_GROUP(MUI_OBJ_SHAPE_CIRCLE, 1.00f, 1.00f, 1.00f));
      MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_SHAPE_GROUP(MUI_OBJ_SHAPE_CIRCLE, 0.75f, 0.75f, 0.75f));
      MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_SHAPE_GROUP(MUI_OBJ_SHAPE_CIRCLE, 0.50f, 0.50f, 0.50f));
      MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_SHAPE_GROUP(MUI_OBJ_SHAPE_CIRCLE, 0.25f, 0.25f, 0.25f));
      // MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, circle_tex, 1.00f, 1.00f, 1.00f));
      // MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, circle_tex, 0.75f, 0.75f, 0.75f));
      // MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, circle_tex, 0.50f, 0.50f, 0.50f));
      // MUI_GROUP_T_ADD(&item_bgs, MUI_OBJ_T_IMG(0.0f, 0.0f, 0.55f, 0.55f, circle_tex, 0.25f, 0.25f, 0.25f));
      
      mui_group(&item_bgs);
    }
    {
      MUI_GROUP_T_INIT(&items, VEC2_XY(0.2f, -0.8f), VEC2_XY(1.75f, 0.5f), 0.0f, MUI_STATIC | MUI_CENTER | MUI_ROW, false);
      mui_obj_t* slot_1, *slot_2, *slot_3, *slot_4;
      MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.225f, 0.0f, 0.55f, 0.55f, weapon_tex, 0.25f, 0.25f, 0.25f), slot_4);
      MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.15f,  0.0f, 0.55f, 0.55f, weapon_tex, 0.50f, 0.50f, 0.50f), slot_3);
      MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.075f, 0.0f, 0.55f, 0.55f, weapon_tex, 0.75f, 0.75f, 0.75f), slot_2);
      MUI_GROUP_T_ADD_PTR(&items, MUI_OBJ_T_IMG(0.0f,   0.0f, 0.55f, 0.55f, weapon_tex, 1.00f, 1.00f, 1.00f), slot_1);
      
      static int active_slot = 1;  
      if (input_get_key_pressed(KEY_ALPHA1) || input_get_key_pressed(KEY_NUMPAD1)) { active_slot = 1; }
      if (input_get_key_pressed(KEY_ALPHA2) || input_get_key_pressed(KEY_NUMPAD2)) { active_slot = 2; }
      if (input_get_key_pressed(KEY_ALPHA3) || input_get_key_pressed(KEY_NUMPAD3)) { active_slot = 3; }
      if (input_get_key_pressed(KEY_ALPHA4) || input_get_key_pressed(KEY_NUMPAD4)) { active_slot = 4; }
  
      slot_1->active = active_slot == 1;
      slot_2->active = active_slot == 2;
      slot_3->active = active_slot == 3;
      slot_4->active = active_slot == 4;
      
      mui_group(&items);
    }
  }
}
