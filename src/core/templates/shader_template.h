#ifndef DATA_SHADER_TEMPLATE_H 
#define DATA_SHADER_TEMPLATE_H 

#ifdef __cplusplus
extern "C"
{
#endif

#include "global/global.h"
#include "core/types/shader.h"

// typedef int shader_template_type;         // defined in game shader_table.h
#define SHADER_TEMPLATE_NONE        -1    // used for custom shader on material system

#define SHADER_TEMPLATE_BASIC       -2
#define SHADER_TEMPLATE_SHADOW_MAP  -3
#define SHADER_TEMPLATE_DEFERRED    -4
#define SHADER_TEMPLATE_SKYBOX      -5
#define SHADER_TEMPLATE_SHADOW_PASS -6
#define SHADER_TEMPLATE_LIGHTING    -7
#define SHADER_TEMPLATE_POST_FX     -8
#define SHADER_TEMPLATE_BRDF_LUT    -9
#define SHADER_TEMPLATE_MOUSE_PICK  -10
#define SHADER_TEMPLATE_TERRAIN     -11
#define SHADER_TEMPLATE_FXAA        -12
#define SHADER_TEMPLATE_MIN         -12



#define SHADER_TEMPLATE_T_SET_UNIFORMS_F_NAME_MAX 128
// @DOC: template for shader
//       specifies all data needed to make shader
typedef struct shader_template_t
{
  const char* name; 
  const char* vert; 
  const char* frag;
  bool unlit;
  uniforms_callback* set_uniforms_f;
  #ifdef EDITOR
  char set_uniforms_f_name[SHADER_TEMPLATE_T_SET_UNIFORMS_F_NAME_MAX];
  #endif // EDITOR
}shader_template_t;

#define SHADER_TEMPLATE_SET_UNIFORMS_F(_f) .set_uniforms_f = (_f), .set_uniforms_f_name = #_f,


// @DOC: set in games/.../shader_table.c
extern shader_template_t shader_table[];
extern const int shader_table_len;

// @DOC: set in games/.../shader_table.c
void shader_table_init();

// @DOC: get pointer to shader template by its index, see shader_template_type
//       idx: index for template, use shader_template_type
const shader_template_t* shader_template_get(int idx);

// @DOC: get all templates in arr
//       len: gets set to arr's length
const shader_template_t* shader_template_get_all(int* len);

#ifdef __cplusplus
} // extern C
#endif

#endif
