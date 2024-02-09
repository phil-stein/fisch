#ifndef MATERIAL_TABLE_H
#define MATERIAL_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

// @DOC: act as index for shader_template_get()
//       ! if out of order shaders will be loaded as the wrong shader
typedef enum shader_template_type
{
  
  SHADER_TEMPLATE_TEST,
  SHADER_TEMPLATE_MAX,

}shader_template_type;

#ifdef __cplusplus
} // extern C
#endif


#endif  // ENTITY_TABLE_H
