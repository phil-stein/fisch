#include "core/camera.h"
#include "core/core_data.h"
#include "math/math_vec3.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "global/global.h"
#include "math/math_inc.h"


// ---- vars ----
// vec3 position = { 0.0f,   6.0f,  10.0f };
// vec3 front	  = { 0.0f,  -0.15f, -1.0f };

// vec3 position = { 0.0f,   .0f,  0.0f };
// vec3 front	  = { 0.0f,  0.0f, -1.0f };
// vec3 up		    = { 0.0f,   1.0f,   0.0f };
// 
// vec3 target   = { 0.0, 0.0, 0.0 };
// 
// const float near_plane = 0.1f;
// const float far_plane  = 1000.0f; // 100.0f;
// const float fov        = 45.0f;
// const float fov_rad    = 45.0f * M_PI_F / 180.0f; 

vec3 target   = { 0.0, 0.0, 0.0 };


// editor camera -------------------------------------

void camera_init()
{
  TRACE();
}

// ---- func ----
void camera_move(vec3 dist)
{
  TRACE();

	vec3_add(core_data->cam.pos, dist, core_data->cam.pos);
}
// void camera_set_pos(vec3 pos)
// {
//   TRACE();
// 
// 	vec3_copy(pos, core_data->cam.pos);
// }
void camera_parent_entity_offset(entity_t* e, vec3 pos, vec3 rot, vec3 scl)
{
  mat4 lookat;
  vec3 current, target;
  
  camera_get_front(current);
  vec3_copy(VEC3(0), target);
  mat4_lookat(current, target, VEC3_Y(1), lookat);
  mat4_inverse(lookat, lookat);

  mat4_make_model(VEC3(0), VEC3(0), VEC3(1), e->model);  
  mat4_mul(e->model, lookat, e->model);
  
  mat4 trans;
  mat4_make_model(VEC3(0), rot, scl, trans);  
  mat4_mul(e->model, trans, e->model);
  
  vec3 e_pos;
  vec3_copy(core_data->cam.pos, e_pos);
  vec3 front, right, up;
  camera_get_front(front);
  vec3_mul_f(front, pos[2], front);
  camera_get_right(right);
  vec3_mul_f(right, pos[0], right);
  camera_get_up(up);
  vec3_mul_f(up, pos[1], up);
  vec3_add(e_pos, front, e_pos);
  vec3_add(e_pos, right,  e_pos);
  vec3_add(e_pos, up,  e_pos);
  mat4_set_pos_vec3(e_pos, e->model);

  e->skip_model_update = true;  // explicitly not update model, cause we do it here
}

// ---- set ----
// void camera_set_front(f32 pitch_rad, f32 yaw_rad)
void camera_set_pitch_yaw(f32 pitch_rad, f32 yaw_rad)
{
  TRACE();

  // old:
  // vec3_copy(dir, core_data->cam.front);

  core_data->cam.pitch_rad = pitch_rad;
  core_data->cam.yaw_rad   = yaw_rad;
  
  // // https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles
	// core_data->cam.front[0] = (f32)cos(yaw_rad) * (f32)cos(pitch_rad);
	// core_data->cam.front[1] = (f32)sin(pitch_rad);
	// core_data->cam.front[2] = (f32)sin(yaw_rad) * (f32)cos(pitch_rad);
  // camera_get_up_axis(core_data->cam.up);
 
  // target is where we set the dir + pos
  camera_get_front(target);
  vec3_add(core_data->cam.pos, target, target);
}
// void camera_set_up(vec3 dir)
// {
//   TRACE();
//   ERR("doesnt work\n");
// 
// 	vec3_copy(dir, core_data->cam.up);
// }

// ---- get ----

// void camera_get_inv_direction(vec3 dir)
// {
//   TRACE();
//   // ERR("doesnt work\n");
// 
// 	// same as: dir = position - target; 
// 	// vec3_sub(core_data->cam.pos, core_data->cam.target, dir);
// 	vec3_sub(core_data->cam.pos, target, dir);
// 	vec3_normalize(dir, dir);
// }

void camera_get_front(vec3 front)
{
	front[0] = (f32)cos(core_data->cam.yaw_rad) * (f32)cos(core_data->cam.pitch_rad);
	front[1] = (f32)sin(core_data->cam.pitch_rad);
	front[2] = (f32)sin(core_data->cam.yaw_rad) * (f32)cos(core_data->cam.pitch_rad);
}

void camera_get_right(vec3 right)
{
  TRACE();

  // // old:
	// vec3 up = { 0.0, 1.0, 0.0 };
	// vec3 dir = { 0.0, 0.0, 0.0 };
	// camera_get_inv_direction(dir);
	// // axis = { 0.0, 0.0, 0.0 };
	// vec3_cross(up, dir, right);
	// vec3_normalize(right, right);

  vec3 front, up;
  vec3_copy(VEC3_Y(1), up);
  camera_get_front(front);
  // vec3_cross(front, up, right);
  vec3_cross(up, front, right);
  vec3_normalize(right, right);
}
void camera_get_right_sway(vec3 right)
{
  TRACE();
  
  vec3 front, up;
  camera_get_up_sway(up);
  camera_get_front(front);
  vec3_cross(front, up, right);
}

void camera_get_up(vec3 up)
{
  TRACE();

	vec3 dir = { 0.0, 0.0, 0.0 }; 
	// camera_get_inv_direction(dir);
	camera_get_front(dir);
	// vec3_mul_f(dir, -1.0f, dir);
	vec3 right = { 0.0, 0.0, 0.0 }; 
	camera_get_right(right);
	vec3_cross(dir, right, up);
  vec3_normalize(up, up);
}

void camera_get_up_sway(vec3 up)
{
  TRACE();
  // https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles
  up[0] = sin(core_data->cam.pitch_rad) * sin(core_data->cam.yaw_rad);
  up[1] = cos(core_data->cam.pitch_rad);
  up[2] = sin(core_data->cam.pitch_rad) * cos(core_data->cam.yaw_rad);
}

void camera_get_turntable_view_mat(const float radius, mat4 view)
{
  TRACE();

	f32 cam_x = (f32)sin(glfwGetTime()) * radius;
	f32 cam_z = (f32)cos(glfwGetTime()) * radius;

	vec3 pos = { cam_x, 0.0, cam_z };
	vec3 center = { 0.0, 0.0, 0.0 };
	vec3 up = { 0.0, 1.0, 0.0 };
	mat4_lookat(pos, center, up, view);
}

void camera_get_view_mat(mat4 view)
{
  TRACE();

	vec3 center, front, up;
  vec3_copy(VEC3_Y(1), up);
  camera_get_front(front);
	vec3_add(core_data->cam.pos, front, center);
	mat4_lookat(core_data->cam.pos, center, up, view);
}
void camera_get_view_mat_sway(mat4 view)
{
  TRACE();

	vec3 center, front, up;
  camera_get_up(up);
  camera_get_front(front);
	vec3_add(core_data->cam.pos, front, center);
	mat4_lookat(core_data->cam.pos, center, up, view);
}
void camera_get_proj_mat(int width, int height, mat4 proj)
{
  TRACE();

  mat4_perspective(core_data->cam.fov_rad, ((f32)width / (f32)height), core_data->cam.near_plane, core_data->cam.far_plane, proj);
}

