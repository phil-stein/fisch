#include "core/ecs/ecs.h"

#include "stb/stb/ds.h"


entity_t* world_arr = NULL;
u32       world_arr_len = 0;
entity_t* world_arr_dead = NULL;
u32       world_arr_dead_len = 0;


// @NOTE: what about interlaced componets,
//        i.e. having transform and model
//        one after the other to avoid
//        cache misses

#define ECS_COMP_ARR_INIT(type, arr_name) \
type* arr_name##_arr = NULL;              \
u32   arr_name##_arr_len = 0;             \
u32   arr_name##_dead_arr = NULL;         \
u32   arr_name##_dead_arr_len = 0;        
#define ECS_COMP_ARR_FREE(arr_name) \
arrfree(arr_name##_arr);            \
arr_name##_arr = NULL;              \
arr_name##_arr_len = 0;             \
arrfree(arr_name##_dead_arr);       \
arr_name##_dead_arr = NULL;         \
arr_name##_dead_arr_len = 0;        

ECS_COMP_ARR_INIT(comp_a_t, comp_a);
ECS_COMP_ARR_INIT(comp_b_t, comp_b);

void ecs_init()
{
}
void ecs_update()
{
  // @TODO: update transform matrix
}
void ecs_cleanup()
{
  ECS_COMP_ARR_FREE(comp_a);
  ECS_COMP_ARR_FREE(comp_b);
}

void* ecs_comp_get(u32 uid)
{
  u32 val = uid;
  val &= COMP_UID_TYPE_MASK;
  comp_type type = val;
  
  val = uid;
  val &= COMP_UID_ARR_IDX_MASK;
  u32 idx = val >> 8;
  
  val = uid;
  val &= COMP_UID_ACTIVE_MASK;
  bool active = val >> 8+23;

  switch (type)
  {
    case COMP_A:
      return &comp_a_arr[idx];
    case COMP_B:
      return &comp_b_arr[idx];

    default:
      return NULL;
  }
}

