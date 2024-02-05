#ifndef ECS_ECS_H
#define ECS_ECS_H
#ifdef __cplusplus

#include "global/global.h"
#include "math/math_inc.h"

extern "C" {
#endif

#define ENTITY_COMP_MAX 8


#define ENTITY_UID_ARR_IDX_MAX  1 << 16
#define ENTITY_UID_ARR_IDX_MASK 0b0000000000000000000000000000000000000000000000000000000000000000
// entity_uid:
// 0000000000000000000000000000000000000000000000000000000000000000
// ||--------------------47-----------------------||------16------|
// ||-------------------guid----------------------||---arr_idx----|
// |--1--
// |alive

#define ENTITY_COMP_MAX 8
typedef struct
{
  u64 uid;
  int  template_idx;  // idx for entity_template_get(idx)
  bool is_dead;       // instead of deleting the entity from array, its marked dead and overwritten with the next added entity
  // flag where individual flags can be set and checked by app, i.e. HAS_FLAG(e->tag_flag, TAG_ENEMY);
  // max flag is FLAG(64) on 64bit, generally its FLAG(sizeof(u64))
  u64 tags_flag;  

  // -- space ---
  vec3 pos;   // position, local space 
  vec3 rot;   // rotation, local space
  vec3 scl;   // scale,    local space 
  vec3 delta_pos;    // difference between last and current frame, used for physics sync 
  // vec3 delta_rot; // difference between last and current frame, used for physics sync, not needed as phys don't know what rotation is 
  vec3 delta_scl;    // difference between last and current frame, used for physics sync
  vec3 delta_force;  // difference between last and current frame, used for physics sync

  mat4 model;     // global space
  bool is_moved;  // specifies whether the entity has been moved since last model matrix update, used in 'state_entity_update_global_model()'

  // -- visuals --
  int mat;  // index for assetm, < 0 = no mat
  int mesh; // index for assetm, < 0 = no mesh
  
  // ...

  u32 comp_uids[ENTITY_COMP_MAX];
  // f.e. model, pointlight, physics, etc.

}entity_t;

typedef struct
{
  u64 entity_uid;
  // ...
}comp_a_t;

typedef struct 
{
  // ...
}collider_t;

typedef enum
{
  COMP_A = 0,
  COMP_B = 1,
}comp_type;

// @DOC: masks for extracting the values from uids
#define COMP_UID_TYPE_MASK    0b00000000000000000000000011111111
#define COMP_UID_ARR_IDX_MASK 0b01111111111111111111111100000000
#define COMP_UID_ACTIVE_MASK  0b10000000000000000000000000000000
#define COMP_UID_TYPE_MAX     1 << 8
#define COMP_UID_ARR_IDX_MAX  1 << 23
#define COMP_UID_ACTIVE_MAX   1
// comp_uids:
//  00000000000000000000000000000000
//  ||---------------------||--8----|
//  ||---------23----------||comp_id|
//  ||-----comp_arr_idx----| 
//  |--1---
//  |active
INLINE u32 ecs_comp_gen_uid(comp_type type, u32 arr_idx)
{
  ERR_CHECK(type < COMP_UID_TYPE_MAX, 
      "ecs_comp_gen_uid type too big, max is: %d\n", COMP_UID_TYPE_MAX);
  u32 uid = type; // comp_id
  
  ERR_CHECK(arr_idx < COMP_UID_ARR_IDX_MAX, 
      "ecs_comp_gen_uid arr_idx too big, max is: %d\n", COMP_UID_ARR_IDX_MAX);
  uid |= arr_idx << 8;

  uid |= 1 << 31;     // active
}
INLINE bool ecs_comp_active(u32 uid)
{
  uid &= COMP_UID_ACTIVE_MASK;
  return (bool)(uid >> 31);
}


entity_t* ecs_add();
entity_t* ecs_get();
bool      ecs_remove();

void*     ecs_comp_add()
void*     ecs_comp_get();
bool      ecs_comp_remove();


#ifdef __cplusplus
} // extern C
#endif

#endif // ECS_ECS_H
