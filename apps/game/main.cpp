#include <iostream>

#include "global/global.h"
#include "core/program.h"
#include "core/window.h"
#include "core/input.h"
#include "core/core_data.h"
#include "core/camera.h"
#include "core/ecs_manager.h"
#include "core/io/assetm.h"
#include "core/types/cubemap.h"
#include "core/renderer/renderer_direct.h"
#include "core/debug/debug_timer.h"
#include "math/math_inc.h"

// #include "hello_world.h"

void main_init();
void main_update();

void move_cam_by_keys();
void rotate_cam_by_mouse();
#define CAM_SPEED             5
#define CAM_SPEED_SHIFT_MULT  5
#define KEY_MOUSE_MOVE_START  MOUSE_RIGHT
#define KEY_MOVE_FORWARD      KEY_W 
#define KEY_MOVE_BACKWARD     KEY_S
#define KEY_MOVE_LEFT         KEY_A
#define KEY_MOVE_RIGHT        KEY_D 
#define KEY_MOVE_UP           KEY_E 
#define KEY_MOVE_DOWN         KEY_Q 
f32 mouse_sensitivity = 0.6f;

static core_data_t* core_data = NULL;

int main(void)
{
  // loop(0, NULL);
  program_start(1000, 800, "window", WINDOW_MIN, main_init, main_update, ASSET_PATH);

  return 0;
}

void main_init()
{
  // std::cout << "init\n";
  core_data = core_data_get();

  TIMER_FUNC_STATIC(core_data->cube_map = cubemap_load("#cubemaps/gothic_manor_01_2k.hdr"));
  core_data->cube_map.intensity = 1.9f;
  
  vec3_copy(VEC3_XYZ(0, 0, 0), core_data->dir_light.pos);
  vec3_copy(VEC3_XYZ(1, 1, 0), core_data->dir_light.dir);
  vec3_copy(VEC3_XYZ(1, 1, 1), core_data->dir_light.color);
  core_data->dir_light.intensity = 2.0f;
  core_data->dir_light.cast_shadow = false;
  core_data->dir_light.shadow_map_x = 0;
  core_data->dir_light.shadow_map_y = 0;

}
void main_update()
{
  // f32 cam_pos[]  = {0, 0};
  // f32 pos[]      = {0, 0};
  // f32 size[]     = {1, 1};
  // f32 color[]    = {1, 0, 1};
  // renderer_direct_draw_quad(cam_pos, 10.0f, pos, size, color);
 
  // cam_pos[0] += 2.0f;
  // color[1]   =  1.0f;
  // texture_t* tex0 = assetm_get_texture("#internal/preview_bg.png", false); 
  // renderer_direct_draw_quad_textured(cam_pos, 10.0f, pos, size, tex0, color);
  // 
  // color[0]   =  0.5f;
  // color[1]   =  0.5f;
  // vec3 pos0 = { 0, 2, -10 };
  // vec3 pos1 = { 2, 4, -10 };
  // renderer_direct_draw_line(pos0, pos1, color, 10.0f);

  // vec3 pos2 = { -1.75f, -4, -5 };
  // vec3 rot  = { 0, 0, 0 };
  // vec3 scl  = { 1, 1, 1 };
  // mesh_t*    m   = assetm_get_mesh("demon01");  
  // texture_t* tex1 = assetm_get_texture("demon01/demon01_albedo.png", false); 
  // renderer_direct_draw_mesh_textured(pos2, rot, scl, m, tex1, scl);

  ecs_iter_t it = ecs_query_iter((ecs_world_t*)core_data->ecs_world, (ecs_query_t*)core_data->ecs_query_point_light);
  while (ecs_query_next(&it))
  {
    // PF(" -> iterated over point light\n");
    transform_comp_t* trans   = ecs_field(&it, transform_comp_t, 1);
    point_light_comp_t* light = ecs_field(&it, point_light_comp_t, 2);

    for (int i = 0; i < it.count; i++) 
    {
      vec3 rot  = { 90, 0, 0 };
      vec3 scl  = { 1, 1, 1 };
      vec3 l_pos; vec3_add(trans[i].pos, light[i].offset, l_pos);
      mesh_t*    m    = assetm_get_mesh("gizmos/lightbulb");  
      texture_t* tex1 = assetm_get_texture("#internal/blank.png", false); 
      renderer_direct_draw_mesh_textured(l_pos, rot, scl, m, tex1, light[i].color);
    }
  }

  input_set_cursor_visible(false);
  rotate_cam_by_mouse(); 
  move_cam_by_keys();

  // toggle wireframe
  if (input_get_key_pressed(KEY_WIREFRAME_TOGGLE))
  {
    core_data->wireframe_mode_enabled = !core_data->wireframe_mode_enabled;
    P_BOOL(core_data->wireframe_mode_enabled);
    // core_data->wireframe_mode_enabled = app_data.wireframe_act;
    // app_data.wireframe_act = !app_data.wireframe_act;
  }

  // std::cout << "update\n";
  if (input_get_key_pressed(KEY_TOGGLE_FULLSCREEN))
  {
    window_type type = window_get_mode();
    
    P_WINDOW_TYPE(type); 
    
    // @NOTE: min -> max -> full
    type = type == WINDOW_MIN ? WINDOW_MAX : type == WINDOW_MAX ? WINDOW_FULL : WINDOW_MAX;
    
    P_WINDOW_TYPE(type); 
    P("------------");

    window_set_mode(type);
  }

  if (input_get_key_pressed(KEY_EXIT))
  {
    program_quit();
  }
}

// --- cam fly logic ---

void move_cam_by_keys()
{
  // -- move the cam --
	float cam_speed = CAM_SPEED * core_data->delta_t;
	if (input_get_key_down(KEY_LEFT_SHIFT))
	{ cam_speed *= CAM_SPEED_SHIFT_MULT; }
	if (input_get_key_down(KEY_MOVE_FORWARD))
	{
		vec3 front; // camera_get_front(front);
		vec3_mul_f(core_data->cam.front, cam_speed, front);
		camera_move(front);
	}
	if (input_get_key_down(KEY_MOVE_BACKWARD))
	{
		vec3 front; // camera_get_front(front);
		vec3_mul_f(core_data->cam.front, -cam_speed, front);
		camera_move(front);
	}
	if (input_get_key_down(KEY_MOVE_LEFT))
	{
		vec3 dist;
		vec3_cross(core_data->cam.front, core_data->cam.up, dist);
		vec3_normalize(dist, dist);
		vec3_mul_f(dist, -cam_speed, dist);
		camera_move(dist);
	}
	if (input_get_key_down(KEY_MOVE_RIGHT))
	{
		vec3 dist;
		vec3_cross(core_data->cam.front, core_data->cam.up, dist);
		vec3_normalize(dist, dist);
		vec3_mul_f(dist, cam_speed, dist);
		camera_move(dist);
	}
	if (input_get_key_down(KEY_MOVE_DOWN))
	{
		vec3 up;	// camera_get_up(up);
		vec3_mul_f(core_data->cam.up, -cam_speed, up);
		camera_move(up);
	}
	if (input_get_key_down(KEY_MOVE_UP))
	{
		vec3 up; // camera_get_up(up);
		vec3_mul_f(core_data->cam.up, cam_speed, up);
		camera_move(up);
	}
}

// rotates the camera accoding to the mouse-movement
void rotate_cam_by_mouse()
{
	static bool init = false;
	static f32 pitch, yaw;

	f32 xoffset = input_get_mouse_delta_x();
	f32 yoffset = input_get_mouse_delta_y();

	xoffset *= mouse_sensitivity;
	yoffset *= mouse_sensitivity;

	
	yaw += xoffset;
	pitch += yoffset;

	// printf("pitch: %f, yaw: %f\n", pitch, yaw);

	if (pitch > 89.0f)
	{ pitch = 89.0f; }
	if (pitch < -89.0f)
	{ pitch = -89.0f; }

	if (!init)
	{
    vec3 front;
    // camera_get_front(front);
    vec3_copy(core_data->cam.front, front);
		pitch = front[1] * 90; // -30.375f;
		yaw	  =	front[2] * 90; // -90.875;
		init = true;
	}

	vec3 dir;
	f32 yaw_rad = yaw;     m_deg_to_rad(&yaw_rad);
	f32 pitch_rad = pitch; m_deg_to_rad(&pitch_rad);

	dir[0] = (f32)cos(yaw_rad) * (f32)cos(pitch_rad);
	dir[1] = (f32)sin(pitch_rad);
	dir[2] = (f32)sin(yaw_rad) * (f32)cos(pitch_rad);
	camera_set_front(dir);
}
