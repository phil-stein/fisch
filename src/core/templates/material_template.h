#ifndef DATA_MATERIAL_TEMPLATE_H 
#define DATA_MATERIAL_TEMPLATE_H 

#ifdef __cplusplus
extern "C"
{
#endif


#include "global/global.h"
#include "core/templates/shader_template.h"
#include "math/math_inc.h"


typedef int material_template_type;
#define MATERIAL_TEMPLATE_EMPTY    -1
#define MATERIAL_TEMPLATE_DEFAULT  -2 
#define MATERIAL_TEMPLATE_MIN      -2

// @DOC: template for material
//       specifies all data needed to make material
typedef struct material_template_t
{
  char* albedo;
  char* normal;
  char* roughn;
  char* metall;
  char* emissive;
  rgbf  tint;
  f32   metall_f;
  f32   roughn_f;
  f32   emissive_f;

  f32   tile_scl;    // getts mul with tile
  bool  tile_by_scl; // scale uv by ent scl
  vec2  tile;        // tile/scale uv coords
  
  shader_template_type shader_template;  // SHADER_TEMPLATE_NONE for default

}material_template_t;
// @DOC: default values given to all templates in material_template.c 
//       and then individually overwritten if needed 
#define MATERIAL_TEMPLATE_DEFAULT_INIT()        \
    .albedo   = NULL,                           \
    .normal   = NULL,                           \
    .roughn   = NULL,                           \
    .metall   = NULL,                           \
    .emissive = NULL,                           \
    .tint   = { 1, 1, 1 },                      \
    .metall_f   = 0.0f,                         \
    .roughn_f   = 0.5f,                         \
    .emissive_f = 0.0f,                         \
    .tile_scl = 1.0f,                           \
    .tile_by_scl = false,                       \
    .tile = { 1, 1 },                           \
    .shader_template = SHADER_TEMPLATE_NONE


extern const material_template_t material_table[];
extern const int material_table_len;

// DOC: register/ create default materials with assetm
//      i.e. MATERIAL_TEMPLATE_EMPTY, MATERIAL_TEMPLATE_DEFAULT
void material_template_init_default();

// @DOC: get pointer to material template by its index, see material_template_type
//       idx: index for template, use material_template_type
const material_template_t* material_template_get(int idx);

// @DOC: get all templates in arr
//       len: gets set to arr's length
const material_template_t* material_template_get_all(int* len);

// #ifdef EDITOR
const char* material_template_generate_string(material_template_t* m); 
// #endif

#ifdef __cplusplus
} // extern C
#endif

#endif
