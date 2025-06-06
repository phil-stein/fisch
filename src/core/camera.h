#ifndef CORE_CAMERA_H
#define CORE_CAMERA_H

#include "core/types/entity.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct
{
  const f32 fov; 
  const f32 fov_rad;     
  const f32 near_plane; 
  const f32 far_plane;

  vec3 pos;
  // vec3 target;
  // vec3 up;
  // vec3 front;
  f32  pitch_rad;
  f32  yaw_rad;
  f32  pitch_shake_rad;
  f32  yaw_shake_rad;

}camera_t;

#define CAMERA_SHAKE_MAX 12
typedef struct
{
  f32 total_t;
  f32 current_t;
  f32 intensity_pitch;
  f32 intensity_yaw;
  f32 speed_pitch;
  f32 speed_yaw;
} camera_shake_t;

// @DOC: // initialize the camera, call this before any other calls to cam_...()
void camera_init();
void camera_update();

void camera_shake(camera_shake_t s);

// @DOC: override entity model matrix to be like
//       it is a child of camera
void camera_parent_entity_offset(entity_t* e, vec3 pos, vec3 rot, vec3 scl);
#define camera_parent_entity(e) camera_parent_entity_offset((e), (e)->pos, (e)->rot, (e)->scl)
// @DOC: override the cams front vector, aka. forward dir
// void camera_set_front(vec3 dir);
// void camera_set_front(f32 pitch_rad, f32 yaw_rad);
void camera_set_pitch_yaw(f32 pitch_rad, f32 yaw_rad);
// @DOC: override the cams up vector
// void camera_set_up(vec3 dir);


// // @DOC: get the direction from the taget towards the camera (normalized)
// void camera_get_inv_direction(vec3 dir);

// @DOC: get the front axis of cam
void camera_get_front(vec3 front);

// @DOC: get the cameras right axis (normalized)
void camera_get_right(vec3 axis);
// @DOC: get the cameras true right axis, based on pitch & yaw
void camera_get_right_sway(vec3 right);

// @DOC: get the cameras up axis (normalized)
void camera_get_up(vec3 axis);
// @DOC: get teh cameras true up axis, based on pitch & yaw
void camera_get_up_sway(vec3 up);

// @DOC: get view mat for a turntable style camera 
void camera_get_turntable_view_mat(const float radius, mat4 dest);

// @DOC: get cam normal view matrix
void camera_get_view_mat(mat4 view);
// @DOC: get cam view matrix, oriented at true cam.up
//       sways on cam.right axis, bc. of that
void camera_get_view_mat_sway(mat4 view);

// @DOC: get cam projection matrix
void camera_get_proj_mat(int width, int height, mat4 proj);

#ifdef __cplusplus
} // extern C
#endif

#endif  // CORE_CAMERA_H
