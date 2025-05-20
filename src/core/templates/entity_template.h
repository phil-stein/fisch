#ifndef DATA_ENTITY_TEMPLATE_H
#define DATA_ENTITY_TEMPLATE_H

#include "global/global.h"
#include "core/types/types.h"
#include "math/math_inc.h"
#include "core/state/state.h"

#ifdef __cplusplus
extern "C"
{
#endif


// enum is in games/./entity_table.h
// typedef int entity_template_type;

typedef void (script_callback)(int entity_id);

#define ENTITY_TEMPLATE_NAME_MAX 128
// @DOC: template for entity
//       specifies all data needed to make entity except transform
typedef struct entity_template_t
{
  char name[ENTITY_TEMPLATE_NAME_MAX];
  s64  tags_flag;  // 0 is no tags

  char* mesh;   // name for assetm, "-" means no mesh
  int   mat;    // idx for material_template.c, -1 means no mesh

  vec3 pos, rot, scl; // added as offset if set in state_entity_add_from_template()

  // pointlight
  struct 
  {
    bool add;
    vec3 offset; 
    rgbf color;
    f32  intensity;
  }pointlight;
  
  script_callback* script_00_f;
  script_callback* script_01_f;
  script_callback* script_02_f;
  script_callback* script_03_f;

  entity_phys_flag phys_flag;
  f32 mass;
  f32 friction;
  union
  {
    f32  radius;
    vec3 aabb_size;   // total aabb size from min to max
  };
  bool is_trigger;
  vec3 collider_offset; // offset from objects position

}entity_template_t;

// @DOC: default values given to all templates in entity_template.c 
//       and then individually overwritten if needed 
#define ENTITY_TEMPLATE_T_SET_DEFAULTS(_type)   \
  .name = #_type,                               \
  .tags_flag = 0,                               \
  .pos = { 0, 0, 0 },                           \
  .rot = { 0, 0, 0 },                           \
  .scl = { 1, 1, 1 },                           \
  .pointlight.add = false,                      \
  .pointlight.offset    = { 0, 0, 0 },          \
  .pointlight.color     = { 1, 1, 1 },          \
  .pointlight.intensity = 1.0f,                 \
  .mesh = "cube",                               \
  .mat  = MATERIAL_TEMPLATE_DEFAULT,            \
  .script_00_f = NULL,                          \
  .script_01_f = NULL,                          \
  .script_02_f = NULL,                          \
  .script_03_f = NULL,                          \
  .phys_flag   = 0,                             \
  .mass        = 1.0f,                          \
  .friction    = 0.1f,                          \
  .aabb_size   = { 1, 1, 1 },                   \
  .collider_offset = { 0, 0, 0 },               \
  .is_trigger  = false                   

#define ENTITY_TEMPLATE_ENTRY(_table, _type)                  \
  (_table)[ENTITY_TEMPLATE_##_type] = (entity_template_t)     \
  { ENTITY_TEMPLATE_T_SET_DEFAULTS(_type),

// old
//   .init_f      = NULL,                    \x
//   .update_f    = NULL,                    \x
//   .cleanup_f   = NULL,                    \x
//   .collision_f = NULL,                    \x
//   .trigger_f   = NULL,                    \x

// @DOC: values for an empty entity
#define ENTITY_TEMPLATE_T_EMPTY()         \
{                                         \
  .name = "empty",                        \
 .tags_flag = 0,                         \
  .pointlight.add = false,                \
  .pointlight.offset    = { 0, 0, 0 },    \
  .pointlight.color     = { 1, 1, 1 },    \
  .pointlight.intensity = 1.0f,           \
  .mesh = "-",                            \
  .mat  = 0,                              \
  .script_00_f = NULL,                    \
  .script_01_f = NULL,                    \
  .script_02_f = NULL,                    \
  .script_03_f = NULL,                    \
  .phys_flag   = 0,                       \
  .mass        = 1.0f,                    \
  .friction    = 0.1f,                    \
  .aabb_size   = { 1, 1, 1 },             \
  .collider_offset = { 0, 0, 0 },         \
  .is_trigger  = false                    \
}

// @DOC: defined in games/.../entity_table.c
// extern const entity_template_t entity_template_table[]; 
// extern const int entity_template_table_len;
extern entity_template_t entity_table[]; 
extern const int entity_table_len;

// @DOC: defined in games/.../entity_table.c
void entity_table_init();

// @DOC: get pointer to entity template by its index, see entity_template_type
//       idx: index for template, use entity_template_type
const entity_template_t* entity_template_get(int idx);

// @DOC: get all templates in arr
//       len: gets set to arr's length
const entity_template_t* entity_template_get_all(int* len);

#ifdef __cplusplus
} // extern C
#endif

#endif
