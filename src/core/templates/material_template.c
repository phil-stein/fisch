#include "core/templates/material_template.h"
#include "core/io/assetm.h"

// #ifdef EDITOR
#define TEMPLATE_STR_MAX 1024
char template_str[TEMPLATE_STR_MAX];
// #endif

const material_template_t mat_empty =     
    {
      MATERIAL_TEMPLATE_DEFAULT_INIT(),
      .albedo = NULL,
      .normal = NULL,
      .roughn = NULL,
      .metall = NULL,
      .tint   = { 1, 1, 1 },
      .roughn_f = 0.5f,
      .metall_f = 0.0f,
      .shader_template = SHADER_TEMPLATE_NONE,
    };
const material_template_t mat_default = 
    {
      MATERIAL_TEMPLATE_DEFAULT_INIT(),
      .albedo = NULL,
      .normal = NULL,
      .roughn = NULL,
      .metall = NULL,
      .tint   = { 1, 1, 1 },
      .roughn_f = 0.5f,
      .metall_f = 0.0f,
      .shader_template = SHADER_TEMPLATE_NONE,
    };


// @NOTE: figure out a way to f.e. ASSERT(table_idx == MATERIAL_TEMPLATE_...)
//        basically to check at compile-time if the enums reference the right position in the array
//        macros dont work


void material_template_init_internal()
{
  assetm_get_material_idx(MATERIAL_TEMPLATE_EMPTY);
  assetm_get_material_idx(MATERIAL_TEMPLATE_DEFAULT);
}

const material_template_t* material_template_get(int idx)
{
  if (idx == MATERIAL_TEMPLATE_EMPTY)
  { return &mat_empty; }
  if (idx == MATERIAL_TEMPLATE_DEFAULT)
  { return &mat_default; }
  
  ASSERT(idx < material_table_len);
  ASSERT(idx >= 0);
  return &material_table[idx]; 
}

const material_template_t* material_template_get_all(int* len)
{
  *len = material_table_len;
  return material_table;
}


// #ifdef EDITOR
const char* material_template_generate_string(material_template_t* m) // , const char* albedo, const char* normal, const char* roughness, const char* metall)
{
  SPRINTF(TEMPLATE_STR_MAX, template_str, 
  "\
   {                                  \n\
   MATERIAL_TEMPLATE_DEFAULT_INIT(),  \n\
   .albedo = %s,                      \n\
   .normal = %s,                      \n\
   .roughn = %s,                      \n\
   .metall = %s,                      \n\
   .tint   = { %f, %f, %f },          \n\
   .roughn_f = %f,                    \n\
   .metall_f = %f,                    \n\
   .shader_template = %s              \n\
   .tile_scl = %f,                    \n\
   .tile_by_scl = %s,                 \n\
   .tile = { %f, %f },                \n\
   },\n",
  // albedo, normal, roughness, metall,
  m->albedo, m->normal, m->roughn, m->metall,
  m->tint[0], m->tint[1], m->tint[2],
  m->roughn_f,
  m->metall_f,
  "SHADER_TEMPLATE_NONE",
  m->tile_scl,
  STR_BOOL(m->tile_by_scl),
  m->tile[0], m->tile[1]
  );

  return template_str;
}
// #endif
