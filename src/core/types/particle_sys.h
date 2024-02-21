#ifndef CORE_TYPES_PARTICLE_SYS_H
#define CORE_TYPES_PARTICLE_SYS_H

#include "global/global.h"
#include "math/math_inc.h"
// #include "core/types/types.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum
{
  PARTICLE_SYS_POINT,
  PARTICLE_SYS_SPHERE,
  PARTICLE_SYS_HALF_SPHERE,
  PARTICLE_SYS_RECT,
  PARTICLE_SYS_CIRCLE,
  // PARTICLE_SYS_BOX,
  // PARTICLE_SYS_CONE,

} particle_sys_emitter_type;

typedef enum
{
  PARTICLE_SYS_OBJ_BILLBOARD,
  PARTICLE_SYS_OBJ_PLANE,
  PARTICLE_SYS_OBJ_SPHERE,
  PARTICLE_SYS_OBJ_BOX,
  PARTICLE_SYS_OBJ_MESH,

} particle_sys_obj_type;


typedef struct
{
  vec3 pos;                   
  vec3 dir;                         // forward direction

  f32  lifetime;                    // total lifetime, if <= -100 is infinite
  f32  lifetime_variation;          // randomness for lifetime, MAX( rand_f32(), (1-this) ) * lifetime

  u32  start_particle_count;        // spawed at start, when created
  u32  particle_per_sec;            // spawed during lifetime
  f32  particle_per_sec_variation;  // randomness in spawed partticles, MAX( rand_f32(), (1-this) ) * particle_per_sec

  u32 particle_max;                 // max amount of particles that fit in bump_alloc_t

  struct 
  {
    particle_sys_emitter_type type;
    union
    {
      f32 radius;   // SPHERE & HALF_SPHERE & CIRCLE
      f32 x_scl;    // RECT
    }; 
    f32 y_scl;      // RECT

  }emitter;

  struct
  {
    particle_sys_obj_type type;
    vec3 scl; 
    vec3 rot;
    int  mesh;                  // asetm idx, -1, no mesh
    int  material;              // asetm idx, -1, no material
    rgbf tint_start;            // interpolates between this and tint_end
    rgbf tint_end;              // interpolates between tint_start and this
  }obj;

  struct
  {
    vec3 start_velocity;
    f32  start_velocity_variation;  // randomness for start velocity, MAX( rand_f32(), (1-this) ) * velocity
    f32  min_y_pos;                 // clamp pos.y to min val, 'fall on floor'
    
  } phys;

} particle_sys_t;

typedef struct 
{
  bool is_alive;

  vec3 pos;                   
  vec3 scl; 
  vec3 rot;

  vec3 velocity;

  f32  lifetime;              // total lifetime, <= -100 is infinite

} particle_t;

#ifdef __cplusplus
} // extern C
#endif

#endif // CORE_PARTICLE_SYS_H
